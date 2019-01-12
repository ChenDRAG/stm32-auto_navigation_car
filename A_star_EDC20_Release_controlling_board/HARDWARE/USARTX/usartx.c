
#include "usartx.h"

/**************************ʵ�ֺ���**********************************************
*��    ��:		usart3����һ���ֽ�
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
		while((USART3->SR&0x40)==0);	
}
/**************************************************************************
�������ܣ�����3��ʼ��
��ڲ�����pclk2:PCLK2 ʱ��Ƶ��(Mhz)    bound:������
����  ֵ����
**************************************************************************/
void uart3_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
  mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->APB2ENR|=1<<0;    //��������ʱ��
	RCC->APB2ENR|=1<<4;   //ʹ��PORTC��ʱ��  
	RCC->APB1ENR|=1<<18;  //ʹ�ܴ���ʱ�� 
	GPIOC->CRH&=0XFFFF00FF; 
	GPIOC->CRH|=0X00008B00;//IO״̬���    � 8b--pc11 10   rx  tx
	GPIOC->ODR|=1<<10;	 
  AFIO->MAPR|=1<<4;      //������ӳ��

	RCC->APB1RSTR|=1<<18;   //��λ����1
	RCC->APB1RSTR&=~(1<<18);//ֹͣ��λ	   	   
	//����������
 	USART3->BRR=mantissa; // ����������	 
	USART3->CR1|=0X200C;  //1λֹͣ,��У��λ.
	//ʹ�ܽ����ж�
	USART3->CR1|=1<<8;    //PE�ж�ʹ��
	USART3->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART3_IRQn,2);
}

/**************************************************************************
�������ܣ�����3�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
int USART3_IRQHandler(void)
{	
	static u8 count,last_data,last_last_data,Usart_ON_Count,restart=0;
	if(USART3->SR&(1<<5))//���յ�����
	{	      
		u8 temp=USART3->DR;//������
		USART_ClearITPendingBit(USART3,USART_IT_RXNE); 
		    if(Usart_ON_Flag==0)
				{	
					if(++Usart_ON_Count>10)Usart_ON_Flag=1;//  ���ǽ��ܵ�10���ַ��Ժ�Usart_ON_Flag�ͻ����ñ��1  ��ʱ����������������
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
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  USART_DeInit(USART1); 
	
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģ�
	USART_Init(USART1, &USART_InitStructure); 
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);   
/*MY_NVIC_Init(0,1,USART2_IRQn,2);//��2��������ȼ� 
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);  */
                  //ʹ�ܴ���1 
									/*
	//ʹ�ܽ����ж�
	USART1->CR1|=1<<8;    //PE�ж�ʹ��
	USART1->CR1|=1<<5;    //���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(0,1,USART1_IRQn,2);*/

}
int USART1_IRQHandler(void)
{	
	if(USART1->SR&(1<<5))//���յ�����
	{	      
		u8 temp=USART1->DR;//������
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




