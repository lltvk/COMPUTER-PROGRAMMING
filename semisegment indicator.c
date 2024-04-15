#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#define IND_PORT GPIOA
#define D1 GPIO_Pin_7
#define D2 GPIO_Pin_8
#define D3 GPIO_Pin_9
#define SEG_A GPIO_Pin_0
#define SEG_B GPIO_Pin_1
#define SEG_C GPIO_Pin_2
#define SEG_D GPIO_Pin_3
#define SEG_E GPIO_Pin_4
#define SEG_F GPIO_Pin_5
#define SEG_G GPIO_Pin_6
#define SEG_DOT GPIO_Pin_10
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
#define DIG10()
#define DIG_DOT (SEG_DOT)
#define ALL_PINS (DIG8 | D1 | D2 | D3 | DIG_DOT)
void delay(void)
{
int i=0;
for (i=0; i != 0x500000; i++);
}
void indication (int chislo)
{
}
int main(void)
{
//Настраиваем на выход все выводы, подключенные к индикатору
RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);
//Структура, содержащая настройки порта
GPIO_InitTypeDef PORT;
//Указываем какие выводы нужно настроить
PORT.GPIO_Pin = ALL_PINS;
// Настраиваем как выход Push-pull
PORT.GPIO_Mode = GPIO_Mode_Out_PP;
// Частота 2 МГц
PORT.GPIO_Speed = GPIO_Speed_2MHz;
//Вызываем функцию настройки порта
GPIO_Init(IND_PORT, &PORT);
int i=0;
int n=0;
while (1)
{
IND_PORT->ODR=0;
IND_PORT->ODR|=D1;
IND_PORT->ODR|=DIG1;
delay();
IND_PORT->ODR=0; //на 2 разряде включаем цифру 1
IND_PORT->ODR|=D2;
IND_PORT->ODR|=DIG1;
delay();
IND_PORT->ODR=0;//на 2 разряде включаем цифру 2
IND_PORT->ODR|=D2;
IND_PORT->ODR|=DIG2;
delay();
IND_PORT->ODR=0; //на 2 разряде включаем цифру 4
IND_PORT->ODR|=D2;
IND_PORT->ODR|=DIG4;
delay();
IND_PORT->ODR=0;//на 2 разряде включаем цифру 8
IND_PORT->ODR|=D2;
IND_PORT->ODR|=DIG8;
delay();
//двузначные числа
IND_PORT->ODR=0;//на 2 разряде включаем цифру 1
IND_PORT->ODR|=D2;
IND_PORT->ODR|=DIG1;
IND_PORT->ODR=0;//на 3 разряде включаем цифру 6
IND_PORT->ODR|=D3;
IND_PORT->ODR|=DIG6;		
delay();
IND_PORT->ODR=0;//на 2 разряде включаем цифру 3
IND_PORT->ODR|=D2;
IND_PORT->ODR|=DIG3;
IND_PORT->ODR=0;//на 3 разряде включаем цифру 2
IND_PORT->ODR|=D3;
IND_PORT->ODR|=DIG2;		
delay();
IND_PORT->ODR=0;//на 2 разряде включаем цифру 6
IND_PORT->ODR|=D2;
IND_PORT->ODR|=DIG6;
IND_PORT->ODR=0;//на 3 разряде включаем цифру 4
IND_PORT->ODR|=D3;
IND_PORT->ODR|=DIG4;		
delay();
for (i=0; i != 0x500000; i++)
{
indication(n);
}
n++;
}
}
