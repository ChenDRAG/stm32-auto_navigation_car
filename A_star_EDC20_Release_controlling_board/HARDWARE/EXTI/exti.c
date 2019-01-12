#include "exti.h"

/**************************************************************************
函数功能：外部中断初始化
入口参数：无
返回  值：无 
**************************************************************************/
void MiniBalance_EXTI_Init(void)
{
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 
	GPIOB->CRH&=0X0FFFFFFF; 
	GPIOB->CRH|=0X80000000;//PB15上拉输入
  GPIOB->ODR|=1<<15;      //PB15上拉	
	Ex_NVIC_Config(GPIO_B,15,FTIR);		//下降沿触发
	MY_NVIC_Init(2,1,EXTI15_10_IRQn,2);  	//抢占2，子优先级1，组2
}










