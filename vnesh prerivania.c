#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
// В качестве порта для управления светодиодной линейкой выберем PORTB
#define LB_PORT GPIOB
// Порт к которому подключен индикатор
#define IND_PORT GPIOA
// Общие выводы индикатора
#define D1 GPIO_Pin_7
#define D2 GPIO_Pin_8
#define D3 GPIO_Pin_9
// К какому выводу какой сегмент подключен
#define SEG_A GPIO_Pin_0
#define SEG_B GPIO_Pin_1
#define SEG_C GPIO_Pin_2
#define SEG_D GPIO_Pin_3
#define SEG_E GPIO_Pin_4
#define SEG_F GPIO_Pin_5
#define SEG_G GPIO_Pin_6
//Собираем цифры из сегментов
#define DIG0 ( SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F )
#define DIG1 ( SEG_B | SEG_C )
#define DIG2 ( SEG_A | SEG_B | SEG_G | SEG_E | SEG_D )
#define DIG3 ( SEG_A | SEG_B | SEG_G | SEG_C | SEG_D )
#define DIG4 ( SEG_F | SEG_G | SEG_B | SEG_C )
#define DIG5 ( SEG_A | SEG_F | SEG_G | SEG_C | SEG_D )
#define DIG6 ( SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G )
#define DIG7 ( SEG_A | SEG_B | SEG_C )
#define DIG8 ( SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)
#define DIG9 ( SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)
#define ALL_PINS (DIG8 | D1 | D2  )
// Функция, осуществялющая временную задержку заданной длительности
void delay(int k)
{
int i=0;
for (i=0; i != k*4000; i++);
}
// Функция, осуществялющая динамическую индикацию
void indication (int chislo)
{
// объявляем массив с элементами, соответствующими цифрам 0-9
unsigned int digit[]={DIG0,DIG1,DIG2,DIG3,DIG4,DIG5,DIG6,DIG7,DIG8,DIG9};
// далее определим значение цифр в каждом из разрядов
unsigned char desyatki=0,sotni=0;
// первый разряд — «сотни»
while(chislo>=100)
{
chislo-=100;
desyatki++;
}
//второй разряд — «десятки»
while(chislo>=10)
{
chislo-=10;
desyatki++;
}
// то что осталось — «единицы»
IND_PORT->ODR|=D1;// Включаем первый разряд
IND_PORT->ODR|=digit[desyatki];//Выводим число в разряд "сотни"
// Небольшое время задержки в течении которого горит разряд
delay(1);
// Выключаем первый разряд
IND_PORT->ODR=0;
// Включаем второй разряд
IND_PORT->ODR|=D2;
//Выводим число в разряд "десятки"
IND_PORT->ODR|=digit[chislo];
delay(1);
// Выключаем второй разряд
IND_PORT->ODR=0;
}
int main()
{
//Включим тактирование PORTA и PORTB
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN , ENABLE);
// старшая группы порта B работает как выход
LB_PORT->CRH = 0b00110011001100110011001100110011;
// младшая группа порта B работает как вход с подтяжкой
LB_PORT->CRL = 0b1000;
// младшая группа порта A работает как выход
IND_PORT->CRL = 0b00110011001100110011001100110011;
// старшая группа порта A работает как выход
IND_PORT->CRH = 0b00110011001100110011001100110011;
// выбираем в качестве вывода для внешнего прерывания EXTI0 вывод GPIOB_0
AFIO->EXTICR[0]|=AFIO_EXTICR1_EXTI0_PB;
// разрешаем прерывание EXTI0
EXTI->IMR|=EXTI_IMR_MR0;
//Прерывания по спадающему фронту
EXTI->FTSR|=EXTI_FTSR_TR0;
//Разрешаем прерывание в контроллере прерываний
NVIC_EnableIRQ (EXTI0_IRQn);
// включаем подтяжку к лог. "1" на выводе GPIOB_0
LB_PORT->BSRR=GPIO_BSRR_BS0;
// служебные переменные для организации обратного отсчета
int n=0; int a=0;
while(1)//26
{
n=99;
while(n>0)
{
for (a=0; a != 0x200; a++)
{
indication(n);
}
n--;
}
}
}
// Обработчик прерывания EXTI0
void EXTI0_IRQHandler(void)
{
// отключаем ССИ
IND_PORT->ODR=0;
//установка состояния выводов PORTB
LB_PORT->BSRR=GPIO_BSRR_BS8; //delay(1000);
LB_PORT->BSRR=GPIO_BSRR_BS9; //delay(1000);
LB_PORT->BSRR=GPIO_BSRR_BS10;// delay(1000);
LB_PORT->BSRR=GPIO_BSRR_BS11; //delay(1000);
delay(1000);
LB_PORT->BSRR=(GPIO_BSRR_BR8|GPIO_BSRR_BR9|GPIO_BSRR_BR10|GPIO_BSRR_BR11);
delay(1000);

LB_PORT->BSRR=GPIO_BSRR_BS8;
LB_PORT->BSRR=GPIO_BSRR_BS9;
LB_PORT->BSRR=GPIO_BSRR_BS14;
LB_PORT->BSRR=GPIO_BSRR_BS15;
delay(1000);
LB_PORT->BSRR=GPIO_BSRR_BS12;
LB_PORT->BSRR=GPIO_BSRR_BS13;
LB_PORT->BSRR=GPIO_BSRR_BS14;
LB_PORT->BSRR=GPIO_BSRR_BS15;
LB_PORT->BSRR=(GPIO_BSRR_BR8|GPIO_BSRR_BR9|GPIO_BSRR_BR10|GPIO_BSRR_BR11);

//Сбрасываем флаг прерывания
EXTI->PR|=0x02;
}
