
#include "usartx.h"

/**************************实现函数**********************************************
*功    能:		usart3发送一个字节
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
		while((USART3->SR&0x40)==0);	
}
/**************************************************************************
函数功能：串口3初始化
入口参数：pclk2:PCLK2 时钟频率(Mhz)    bound:波特率
返回  值：无
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
  mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->APB2ENR|=1<<0;    //开启辅助时钟
	RCC->APB2ENR|=1<<4;   //使能PORTC口时钟  
	RCC->APB1ENR|=1<<18;  //使能串口时钟 
	GPIOC->CRH&=0XFFFF00FF; 
	GPIOC->CRH|=0X00008B00;//IO状态设�    � 8b--pc11 10   rx  tx
	GPIOC->ODR|=1<<10;	 
  AFIO->MAPR|=1<<4;      //部分重映射

	RCC->APB1RSTR|=1<<18;   //复位串口1
	RCC->APB1RSTR&=~(1<<18);//停止复位	   	   
	//波特率设置
 	USART3->BRR=mantissa; // 波特率设置	 
	USART3->CR1|=0X200C;  //1位停止,无校验位.
	//使能接收中断
	USART3->CR1|=1<<8;    //PE中断使能
	USART3->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);
}

/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
int USART3_IRQHandler(void)
{	
	static u8 count,last_data,last_last_data,Usart_ON_Count,restart=0;
	if(USART3->SR&(1<<5))//接收到数据
	{	      
		u8 temp=USART3->DR;//读数字
		USART_ClearITPendingBit(USART3,USART_IT_RXNE); 
		    if(Usart_ON_Flag==0)
				{	
					if(++Usart_ON_Count>10)Usart_ON_Flag=1;//  就是接受到10个字符以后Usart_ON_Flag就会永久变成1  这时就屏蔽了蓝牙控制
				}
				
				   if(Usart_Flag==0||restart==1)
						{	
							if(last_data==0xfe&&last_last_data==0xff) 
							{
								Usart_Flag=1,count=0;	
								restart=0;
							}
						}
						
					 if(Usart_Flag==1&&restart==0)
						{	
							Urxbuf[count]=temp;     
							count++;                
							if(count==8)
							{
								if(temp==0x65&&last_data==0x66)
								{
									all_receive_num++;
									Usart_Flag=0;
									watch=0;
								}
								else
								{
									restart=1;
								}
							}
						}
						last_last_data=last_data;
						last_data=temp;
						
						
   }
return 0;	
}








void uart_init(void)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  USART_DeInit(USART1); 
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = 115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模�
	USART_Init(USART1, &USART_InitStructure); 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);   
/*MY_NVIC_Init(0,1,USART2_IRQn,2);//组2，最低优先级 
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);  */
                  //使能串口1 
									/*
	//使能接收中断
	USART1->CR1|=1<<8;    //PE中断使能
	USART1->CR1|=1<<5;    //接收缓冲区非空中断使能	    	
	MY_NVIC_Init(0,1,USART1_IRQn,2);*/

}
int USART1_IRQHandler(void)
{	
	if(USART1->SR&(1<<5))//接收到数据
	{	      
		u8 temp=USART1->DR;//读数字
		static unsigned char ucRxBuffer[20];
		static unsigned char ucRxCnt = 0;	

		ucRxBuffer[ucRxCnt++]=temp;
		if (ucRxBuffer[0]!=0x55)
			ucRxCnt=0;
		
		if(ucRxCnt>11)
		{
			ucRxCnt=0;
		}
		else if(ucRxCnt==11 && ucRxBuffer[1]==0x53)
		{
			float emm=(float)((ucRxBuffer[7]<<8)|ucRxBuffer[6])/32768.0*180;
			ucRxCnt=0;
			if(emm>180)
				Yaw = emm-360;
			else
				Yaw=emm;
		}		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);				
   }
return 0;		
}




