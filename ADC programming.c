#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#define HIGH GPIO_SetBits // Команда установки бита в 1
#define LOW GPIO_ResetBits // Команда сброса бита в 0
#define READ GPIO_ReadInputDataBit // Команда считывающая состояние пина
#define IND_PORT GPIOA
#define LB_PORT GPIOB
// К какой ноге какой сегмент подключен
#define SEG_A GPIO_Pin_5
#define SEG_B GPIO_Pin_7
#define SEG_C GPIO_Pin_10
#define SEG_D GPIO_Pin_9
#define SEG_E GPIO_Pin_4
#define SEG_F GPIO_Pin_6
#define SEG_G GPIO_Pin_8
//Собираем цифры из сегментов
#define DIG0 ( SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F )
#define DIG1 ( SEG_B | SEG_C )
#define DIG2 ( SEG_A | SEG_B | SEG_G | SEG_E | SEG_D )
#define DIG3 ( SEG_A | SEG_B | SEG_G | SEG_C | SEG_D )
#define DIG4 ( SEG_F | SEG_G | SEG_B | SEG_C)
#define DIG5 ( SEG_A | SEG_F | SEG_G | SEG_C | SEG_D )
#define DIG6 ( SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G )
#define DIG7 ( SEG_A | SEG_B | SEG_C )
#define DIG8 ( SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define DIG9 ( SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)
#define A ( SEG_A | SEG_B | SEG_C | SEG_E | SEG_F| SEG_G)
#define B ( SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define NONE (SEG_G)// прочерк
//Функция формирующая небольшую задержку
int n = 0; int count = 0; int res1 = 0; int res2 = 0; int a = 0;
int res3 = 0; int result = 0; int nn=1;
int x=350; int f=1;
void delay1(int k)
{

	int i=0;
	for (i=0; i != k*4000; i++);

}
void delay(void)
{
	int i=0;
	for (i=0; i != 0x5000; i++);
}
void initGPIO(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB , ENABLE);
	GPIO_InitTypeDef PORTA; //Структура, содержащая настройки порта
	PORTA.GPIO_Pin = GPIO_Pin_All; //Указываем, какие выводы нужно настроить
	PORTA.GPIO_Mode = GPIO_Mode_Out_PP; // Настраиваем как выход Push-pull
	PORTA.GPIO_Speed = GPIO_Speed_2MHz; // Частота 2 МГц
	GPIO_Init(GPIOA, &PORTA); //Вызываем функцию настройки порта
	GPIO_InitTypeDef ROWS; //Структура, содержащая настройки порта
	ROWS.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //Указываем, какие выводы нужно настроить
	ROWS.GPIO_Mode = GPIO_Mode_IPU; // Настраиваем как вход с подтяжкой к лог 1
	ROWS.GPIO_Speed = GPIO_Speed_2MHz; // Частота 2 МГц
	GPIO_Init(GPIOB, &ROWS); //Вызываем функцию настройки порта
}
void indication(int chislo)
{
	// объявляем массив с элементами соответствующими цифрам 0-9
	unsigned int digit[]={DIG0,DIG1,DIG2,DIG3,DIG4,DIG5,DIG6,DIG7,DIG8,DIG9,A,B,NONE};
	unsigned char desyatki = 0, sotni = 0;

	while(chislo>=100)
	{
	chislo-=100;
	sotni++;
	}

	while(chislo>=10)
	{
		chislo-=10;
		desyatki++;
	}
	IND_PORT->ODR=0;
	IND_PORT->ODR|=GPIO_Pin_12;
	IND_PORT->ODR|=digit[sotni];
	delay();
	IND_PORT->ODR=0;

	IND_PORT->ODR=0;
	IND_PORT->ODR|=GPIO_Pin_11;
	IND_PORT->ODR|=digit[desyatki];
	delay();
	IND_PORT->ODR=0;

	IND_PORT->ODR=0;
	IND_PORT->ODR|=GPIO_Pin_3;
	IND_PORT->ODR|=digit[chislo];
	delay();
	IND_PORT->ODR=0;
}

void read_keyboard(void)
{
	// установка всех столбцов в лог. 1
	HIGH (GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	// опрашиваем первый столбец
	LOW (GPIOA,GPIO_Pin_3);
	if (!READ(GPIOB,GPIO_Pin_5)){
		a = 1;
		count += 1;
	}
	if (!READ(GPIOB,GPIO_Pin_6)){
		a = 2;
		count += 1;
	}
	if (!READ(GPIOB,GPIO_Pin_7)){
		a = 3;
		count += 1;
	}
	//возвращаем как было
	HIGH (GPIOA,GPIO_Pin_3);
	//------------------------------------
	// опрашиваем второй столбец
	LOW (GPIOA,GPIO_Pin_2);
	if (!READ(GPIOB,GPIO_Pin_5)){
		a = 4;
		count += 1;
	}
	if (!READ(GPIOB,GPIO_Pin_6)){
		a = 5;
		count += 1;
	}
	if (!READ(GPIOB,GPIO_Pin_7)){
		a = 6;
		count += 1;
	}
	HIGH (GPIOA,GPIO_Pin_2);
	//------------------------------------
	// опрашиваем третий столбец
	LOW (GPIOA,GPIO_Pin_1);
	if (!READ(GPIOB,GPIO_Pin_5)){
		a = 7;
		count += 1;
	}
	if (!READ(GPIOB,GPIO_Pin_6)){
		a = 8;
		count += 1;
	}
	if (!READ(GPIOB,GPIO_Pin_7)){
		a = 9;
		count += 1;
	}
	HIGH (GPIOA,GPIO_Pin_1);
	//------------------------------------
	// опрашиваем четвертый столбец
	LOW (GPIOA,GPIO_Pin_0);
	if (!READ(GPIOB,GPIO_Pin_6)){
		a = 0;
		count += 1;
	}
	if (!READ(GPIOB,GPIO_Pin_7))
	{
		res1 = 0;
		res2 = 0;
		res2 = 0;
		result = 0;
		count = 0;
		n = 0;
		LB_PORT->BSRR=(GPIO_BSRR_BR8|GPIO_BSRR_BR9|GPIO_BSRR_BR10|GPIO_BSRR_BR11);
		LB_PORT->BSRR=(GPIO_BSRR_BR12|GPIO_BSRR_BR13|GPIO_BSRR_BR14|GPIO_BSRR_BR15);
		LB_PORT->BSRR=GPIO_BSRR_BS0;
	}
	HIGH (GPIOA,GPIO_Pin_0);
	if(count == 50){
		res1 = a;
		n = res1;
	}
	else if(count == 100){
		res2 = (res1 * 10) + a;
		n = res2;
	}
	else if(count == 150){
		res3 = a;
		n = (res2 * 10) + a;
	}
}
	// Основная программа
int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
	GPIOB->CRH = 0b00110011001100110011001100110011;
	initGPIO();
	while(1)
	{
	read_keyboard();

	for (f=0; f != 0x10; f++) indication(x);


	if (n == 1)

	{
        nn=1;
	LB_PORT->BSRR=GPIO_BSRR_BS0;
	LB_PORT->BSRR=GPIO_BSRR_BS8; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR8;
	LB_PORT->BSRR=GPIO_BSRR_BS9; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR9;
	LB_PORT->BSRR=GPIO_BSRR_BS10; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR10;
	LB_PORT->BSRR=GPIO_BSRR_BS11; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR11;
	LB_PORT->BSRR=GPIO_BSRR_BS12; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR12;
	LB_PORT->BSRR=GPIO_BSRR_BS13; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR13;
	LB_PORT->BSRR=GPIO_BSRR_BS14; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR14;
	LB_PORT->BSRR=GPIO_BSRR_BS15; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR15;

	}

		if (n == 2)
	{
        nn=2;
	LB_PORT->BSRR=GPIO_BSRR_BS0;
	LB_PORT->BSRR=GPIO_BSRR_BS15; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR15;
	LB_PORT->BSRR=GPIO_BSRR_BS14; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR14;
	LB_PORT->BSRR=GPIO_BSRR_BS13; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR13;
	LB_PORT->BSRR=GPIO_BSRR_BS12; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR12;
	LB_PORT->BSRR=GPIO_BSRR_BS11; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR11;
	LB_PORT->BSRR=GPIO_BSRR_BS10; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR10;
	LB_PORT->BSRR=GPIO_BSRR_BS9; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR9;
	LB_PORT->BSRR=GPIO_BSRR_BS8; delay1(x);
	LB_PORT->BSRR=GPIO_BSRR_BR8;
	}


if (n == 3)

			{

			x=x+100;
			n=nn;
			}

if (n == 4)

			{

			x=x-100;
            if (x<=50) x=50;
			n=nn;
			}


	}
}