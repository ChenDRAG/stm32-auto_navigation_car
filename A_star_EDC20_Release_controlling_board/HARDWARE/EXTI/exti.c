#include "exti.h"

/**************************************************************************
�������ܣ��ⲿ�жϳ�ʼ��
��ڲ�������
����  ֵ���� 
**************************************************************************/
void MiniBalance_EXTI_Init(void)
{
	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��	   	 
	GPIOB->CRH&=0X0FFFFFFF; 
	GPIOB->CRH|=0X80000000;//PB15��������
  GPIOB->ODR|=1<<15;      //PB15����	
	Ex_NVIC_Config(GPIO_B,15,FTIR);		//�½��ش���
	MY_NVIC_Init(2,1,EXTI15_10_IRQn,2);  	//��ռ2�������ȼ�1����2
}









