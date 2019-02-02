#include "usart.h"	  
//¼ÓÈëÒÔÏÂ´úÂë,Ö§³Öprintfº¯Êý,¶ø²»ÐèÒªÑ¡Ôñuse MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//±ê×¼¿âÐèÒªµÄÖ§³Öº¯Êý                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef¡¯ d in stdio.h. */ 
FILE __stdout;       
//¶¨Òå_sys_exit()ÒÔ±ÜÃâÊ¹ÓÃ°ëÖ÷»úÄ£Ê½    
_sys_exit(int x) 
{ 
	x = x; 
} 
//ÖØ¶¨Òåfputcº¯Êý 
int fputc(int ch, FILE *f)
{      

	while((USART2->SR&0X40)==0);
	USART2->DR = (u8) ch;      
  return ch;
}
#endif 
int Usart_Receive;

//////////////////////////////////////////////////////////////////
/**************************ÊµÏÖº¯Êý**********************************************
*¹¦    ÄÜ:		usart1·¢ËÍÒ»¸ö×Ö½Ú
*********************************************************************************/
void usart2_send(u8 data)
{//À¶ÑÀ
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}
void uart2_init(u32 pclk2,u32 bound)//36 9600
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//µÃµ½USARTDIV
	mantissa=temp;				 //µÃµ½ÕûÊý²¿·Ö
	fraction=(temp-mantissa)*16; //µÃµ½Ð¡Êý²¿·Ö	 
  mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //Ê¹ÄÜPORTA¿ÚÊ±ÖÓ  
	RCC->APB1ENR|=1<<17;  //Ê¹ÄÜ´®¿ÚÊ±ÖÓ 
	GPIOA->CRL&=0XFFFF00FF; 
	GPIOA->CRL|=0X00008B00;//IO×´Ì¬ÉèÖÃ   8b--pa3 2   rx  tx
	GPIOA->ODR|=1<<10;	  
	RCC->APB1RSTR|=1<<18;   //¸´Î»´®¿Ú1
	RCC->APB1RSTR&=~(1<<18);//Í£Ö¹¸´Î»	   	   
	//²¨ÌØÂÊÉèÖÃ
 	USART2->BRR=mantissa; // ²¨ÌØÂÊÉèÖÃ	 
	USART2->CR1|=0X200C;  //1Î»Í£Ö¹,ÎÞÐ£ÑéÎ».
	//Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
	USART2->CR1|=1<<8;    //PEÖÐ¶ÏÊ¹ÄÜ
	USART2->CR1|=1<<5;    //½ÓÊÕ»º³åÇø·Ç¿ÕÖÐ¶ÏÊ¹ÄÜ	    	
	MY_NVIC_Init(0,1,USART2_IRQn,2);//×é2£¬×îµÍÓÅÏÈ¼¶ 
}
/**************************************************************************
º¯Êý¹¦ÄÜ£º´®¿Ú2½ÓÊÕÖÐ¶Ï
Èë¿Ú²ÎÊý£ºÎÞ
·µ»Ø  Öµ£ºÎÞ
**************************************************************************/
int USART2_IRQHandler(void)
{	
	if(USART2->SR&(1<<5))//½ÓÊÕµ½Êý¾Ý
	{	      
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
		Usart_Receive=USART2->DR;
	
	  if(Usart_Receive==0x4B) Turn_Flag=1;  //½øÈë×ªÏò¿ØÖÆ½çÃæ
	  else	if(Usart_Receive==0x49||Usart_Receive==0x4A) 	 Turn_Flag=0;	//·½Ïò¿ØÖÆ½çÃæ
		
		if(Run_Flag==0)//ËÙ¶È¿ØÖÆÄ£Ê½
		{			
				if(Turn_Flag==0)//ËÙ¶È¿ØÖÆÄ£Ê½
				{
						if(Usart_Receive>=0x41&&Usart_Receive<=0x48)  
						{	
							Flag_Direction=Usart_Receive-0x40;//1-8µÄÖµ  Ö¸¶¨ÁËÐ¡³µÇ°½øµÄ¼¸8¸ö·½Ïòò
						}
						else	if(Usart_Receive<=8)   
						{			
							Flag_Direction=Usart_Receive;
						}	
						else  Flag_Direction=0;
				}
				else	 if(Turn_Flag==1)//Èç¹û½øÈëÁË×ªÏò¿ØÖÆ½çÃæ
				 {
						if(Usart_Receive==0x43) Flag_Left=0,Flag_Right=1;       //flag left right Ä¿Ç°Ã»ÓÐÈÎºÎÓÃ´¦  ×¼±¸É¾³ý
						else if(Usart_Receive==0x47) Flag_Left=1,Flag_Right=0;
						else Flag_Left=0,Flag_Right=0;
						if(Usart_Receive==0x41||Usart_Receive==0x45)Flag_Direction=Usart_Receive-0x40;
						else  Flag_Direction=0;
				 }
	  }	
		
		//ÒÔÏÂÊÇÓëAPPµ÷ÊÔ½çÃæÍ¨Ñ¶
		if(Usart_Receive==0x7B) Flag_PID=1;   //APP²ÎÊýÖ¸ÁîÆðÊ¼Î»
		if(Usart_Receive==0x7D) Flag_PID=2;   //APP²ÎÊýÖ¸ÁîÍ£Ö¹Î»

		 if(Flag_PID==1)  //²É¼¯Êý¾Ý
		 {
			Receive[i]=Usart_Receive;
			i++;
		 }
		 if(Flag_PID==2)  //·ÖÎöÊý¾Ý
		 {
			     if(Receive[3]==0x50) 	 printf("##$"),PID_Send=1;
					 else  if(Receive[3]==0x57) 	 Flash_Send=1;
					 else  if(Receive[1]!=0x23) 
					 {								
						for(j=i;j>=4;j--)
						{
						  Data+=(Receive[j-1]-48)*pow(10,i-j);
						}
						switch(Receive[1])
						 {
							 case 0x30:  RC_Velocity=Data;break;
							 case 0x31:  RC_Position=Data;break;
							 case 0x32:  Z_Position=Data;break;
							 case 0x33:  Position_KP=Data;break;
							 case 0x34:  Position_KI=Data;break;
							 case 0x35:  Position_KD=Data;break;
							 case 0x36:  Velocity_KP=Data;break;
							 case 0x37:  Velocity_KI=Data;break;
							 case 0x38:  break; 
							 //Ô¤Áô
						 }
					 }				 
					 Flag_PID=0;//Ïà¹Ø±êÖ¾Î»ÇåÁã
					 i=0;
					 j=0;
					 Data=0;
					 memset(Receive, 0, sizeof(u8)*50);//Êý×éÇåÁã
		 } 	 
   }
return 0;	
}



/**************************************************************************
º¯Êý¹¦ÄÜ£º´®¿ÚÉ¨Ãè
**************************************************************************/
u8 click_RC (void)
{
			static u8 flag_key=1;//°´¼ü°´ËÉ¿ª±êÖ¾
	    u8 temp;
			if(flag_key&&Usart_Receive!=0x5A)
			{
				flag_key=0;
				if(Usart_Receive>=0x01&&Usart_Receive<=0x08)temp=Usart_Receive;
				else	if(Usart_Receive>=0x41&&Usart_Receive<=0x48)temp=Usart_Receive-0x40;	
				//else 	temp=0;
				return temp;	// °´¼ü°´ÏÂ
			}
			else if(Usart_Receive==0x5A)			flag_key=1;
			return 0;//ÎÞ°´¼ü°´ÏÂ
}
