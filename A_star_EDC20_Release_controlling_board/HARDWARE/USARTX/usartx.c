
#include "usartx.h"

/**************************ÊµÏÖº¯Êý**********************************************
*¹¦    ÄÜ:		usart3·¢ËÍÒ»¸ö×Ö½Ú
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
		while((USART3->SR&0x40)==0);	
}
/**************************************************************************
º¯Êý¹¦ÄÜ£º´®¿Ú3³õÊ¼»¯
Èë¿Ú²ÎÊý£ºpclk2:PCLK2 Ê±ÖÓÆµÂÊ(Mhz)    bound:²¨ÌØÂÊ
·µ»Ø  Öµ£ºÎÞ
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//µÃµ½USARTDIV
	mantissa=temp;				 //µÃµ½ÕûÊý²¿·Ö
	fraction=(temp-mantissa)*16; //µÃµ½Ð¡Êý²¿·Ö	 
  mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->APB2ENR|=1<<0;    //¿ªÆô¸¨ÖúÊ±ÖÓ
	RCC->APB2ENR|=1<<4;   //Ê¹ÄÜPORTC¿ÚÊ±ÖÓ  
	RCC->APB1ENR|=1<<18;  //Ê¹ÄÜ´®¿ÚÊ±ÖÓ 
	GPIOC->CRH&=0XFFFF00FF; 
	GPIOC->CRH|=0X00008B00;//IO×´Ì¬ÉèÖ    Ã 8b--pc11 10   rx  tx
	GPIOC->ODR|=1<<10;	 
  AFIO->MAPR|=1<<4;      //²¿·ÖÖØÓ³Éä

	RCC->APB1RSTR|=1<<18;   //¸´Î»´®¿Ú1
	RCC->APB1RSTR&=~(1<<18);//Í£Ö¹¸´Î»	   	   
	//²¨ÌØÂÊÉèÖÃ
 	USART3->BRR=mantissa; // ²¨ÌØÂÊÉèÖÃ	 
	USART3->CR1|=0X200C;  //1Î»Í£Ö¹,ÎÞÐ£ÑéÎ».
	//Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
	USART3->CR1|=1<<8;    //PEÖÐ¶ÏÊ¹ÄÜ
	USART3->CR1|=1<<5;    //½ÓÊÕ»º³åÇø·Ç¿ÕÖÐ¶ÏÊ¹ÄÜ	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);
}

/**************************************************************************
º¯Êý¹¦ÄÜ£º´®¿Ú3½ÓÊÕÖÐ¶Ï
Èë¿Ú²ÎÊý£ºÎÞ
·µ»Ø  Öµ£ºÎÞ
**************************************************************************/
int USART3_IRQHandler(void)
{	
	static u8 count,last_data,last_last_data,Usart_ON_Count,restart=0;
	if(USART3->SR&(1<<5))//½ÓÊÕµ½Êý¾Ý
	{	      
		u8 temp=USART3->DR;//¶ÁÊý×Ö
		USART_ClearITPendingBit(USART3,USART_IT_RXNE); 
		    if(Usart_ON_Flag==0)
				{	
					if(++Usart_ON_Count>10)Usart_ON_Flag=1;//  ¾ÍÊÇ½ÓÊÜµ½10¸ö×Ö·ûÒÔºóUsart_ON_Flag¾Í»áÓÀ¾Ã±ä³É1  ÕâÊ±¾ÍÆÁ±ÎÁËÀ¶ÑÀ¿ØÖÆ
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
  //GPIO¶Ë¿ÚÉèÖÃ
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//Ê¹ÄÜUSART1£¬GPIOAÊ±ÖÓ
  USART_DeInit(USART1); 
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
  GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯GPIOA.9
   
  //USART1_RX	  GPIOA.10³õÊ¼»¯
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
  GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯GPIOA.10  
   //USART ³õÊ¼»¯ÉèÖÃ

	USART_InitStructure.USART_BaudRate = 115200;//´®¿Ú²¨ÌØÂÊ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê
	USART_Init(USART1, &USART_InitStructure); 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);   
/*MY_NVIC_Init(0,1,USART2_IRQn,2);//×é2£¬×îµÍÓÅÏÈ¼¶ 
  USART_Init(USART1, &USART_InitStructure); //³õÊ¼»¯´®¿Ú1
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//¿ªÆô´®¿Ú½ÓÊÜÖÐ¶Ï
  USART_Cmd(USART1, ENABLE);  */
                  //Ê¹ÄÜ´®¿Ú1 
									/*
	//Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
	USART1->CR1|=1<<8;    //PEÖÐ¶ÏÊ¹ÄÜ
	USART1->CR1|=1<<5;    //½ÓÊÕ»º³åÇø·Ç¿ÕÖÐ¶ÏÊ¹ÄÜ	    	
	MY_NVIC_Init(0,1,USART1_IRQn,2);*/

}
int USART1_IRQHandler(void)
{	
	if(USART1->SR&(1<<5))//½ÓÊÕµ½Êý¾Ý
	{	      
		u8 temp=USART1->DR;//¶ÁÊý×Ö
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




