#include "led.h"

void LED_Init(void)//pa4
{

  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能端口时钟
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	            //端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
  GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	            //端口配置
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA 
	
}

void Indicate(u16 on)
{
	static int temp;
	if(0==on) LED_wrong=0,temp=0;
	else if(on==1) LED_wrong=1,temp=0;
	else		if(++temp==on)	LED_wrong=~LED_wrong,temp=0;
}

void Led_Flash(u16 time)  
{
	  static int temp;
	  if(0==time) LED=0,temp=0;
	  else		if(++temp==time)	LED=~LED,temp=0;
}
