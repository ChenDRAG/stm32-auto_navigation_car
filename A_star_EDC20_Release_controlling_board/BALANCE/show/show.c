#include "show.h"

unsigned char i;          //¼ÆÊı±äÁ¿
unsigned char Send_Count; //´®¿ÚĞèÒª·¢ËÍµÄÊı¾İ¸öÊı
float Vol;
/*
void APP_Show(void)
{    
	  static u8 flag;
	//  int app_2,app_3,app_4;
	//app_4=100;//Ô­±¾ÎªµçÑ¹Öµ£¬ÏÖÔÚÇ¿ĞĞĞ´³É100
	 // app_2=Move_X*0.7;  if(app_2<0)app_2=-app_2;			                   //¶Ô±àÂëÆ÷Êı¾İ¾ÍĞĞÊı¾İ´¦Àí±ãÓÚÍ¼ĞÎ»¯
		//app_3=Move_Y*0.7;  if(app_3<0)app_3=-app_3;	
	 //if(Run_Flag==1)//app_2=0,app_3=0;
	  flag=!flag;
	 if(PID_Send==1)//·¢ËÍPID²ÎÊı
	{
		//printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)RC_Velocity,(int)RC_Position,(int)Z_Position,(int)Position_KP,(int)Position_KI,(int)Position_KD,(int)Velocity_KP,(int)Velocity_KI,0);//´òÓ¡µ½APPÉÏÃæ	
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",6666,(int)RC_Position,(int)Z_Position,(int)Position_KP,(int)Position_KI,(int)Position_KD,(int)Velocity_KP,(int)Velocity_KI,0);//´òÓ¡µ½APPÉÏÃæ	
		PID_Send=0;	
		
	}	
   else	if(flag==0)// 
			printf("{A%d:%d:%d:%d}$",(int)((Target_A+Target_C)/2.0),(int)(Target_A-Target_C),delta_angle,transformYaw()); //´òÓ¡µ½APPÉÏÃæ
	 else
	 //printf("{B%d:%d:%d:%d}$",(int)Pitch,(int)Roll,(int)Yaw,(int)Encoder_D);//´òÓ¡µ½APPÉÏÃæ ÏÔÊ¾²¨ĞÎ
		printf("{B%d:%d:%d:%d:%d}$",(int)now_x,(int)now_y,(int)go_x,(int)go_y,(int)all_receive_num);//´òÓ¡µ½APPÉÏÃæ ÏÔÊ¾²¨ĞÎ	 
}
*/

/*ÓĞºìÍâÓÃ
void APP_Show(void)
{    
	  static u8 flag;
	  flag=!flag;
	if(flag==1)
		//printf("{%d:%d:%d:%d:%d:%d:%d:%d}$",rxbuf[0],rxbuf[1],rxbuf[2],rxbuf[3],rxbuf[4],rxbuf[5],rxbuf[6],rxbuf[7]);//´òÓ¡µ½APPÉÏÃæ ÏÔÊ¾²¨ĞÎ	 
//	else
		printf("{A%d:%d:%d:%d
		:%d:%d}$\n",(int)now_x,(int)now_y,(int)go_x,(int)go_y,(int)transformYaw(),(int)all_receive_num);//´òÓ¡µ½APPÉÏÃæ ÏÔÊ¾²¨Ğ
	else
		printf("{B%d:%d:%d:%d}$\n",(int)l1,(int)l2,(int)r1,(int)r2);
	//
		//printf("{B%d:%d:%d:%d}$\n",(int)LEFT1,(int)LEFT2,(int)RIGHT1,(int)RIGHT2);
		//else
			//printf("{B%d:%d:%d:%d}$\n",(int)Encoder_A,(int)Encoder_B,(int)Target_A,(int)Motor_A);//´òÓ¡µ½APPÉÏÃæ ÏÔÊ¾²¨ĞÎ
	
}
*/
void APP_Show(void)
{    
	  static u8 flag;
	  flag=!flag;
	if(flag==1)
		//printf("{%d:%d:%d:%d:%d:%d:%d:%d}$",rxbuf[0],rxbuf[1],rxbuf[2],rxbuf[3],rxbuf[4],rxbuf[5],rxbuf[6],rxbuf[7]);//´òÓ¡µ½APPÉÏÃæ ÏÔÊ¾²¨ĞÎ	 
//	else
		printf("{A%d:%d:%d:%d:%d:%d}$\n",(int)now_x,(int)now_y,(int)go_x,(int)go_y,(int)transformYaw(),(int)all_receive_num);//´òÓ¡µ½APPÉÏÃæ ÏÔÊ¾²¨Ğ
	//else
		//printf("{B%d:%d:%d:%d}$\n",(int)l1,(int)l2,(int)r1,(int)r2);
	//
		//printf("{B%d:%d:%d:%d}$\n",(int)LEFT1,(int)LEFT2,(int)RIGHT1,(int)RIGHT2);
		//else
			//printf("{B%d:%d:%d:%d}$\n",(int)Encoder_A,(int)Encoder_B,(int)Target_A,(int)Motor_A);//´òÓ¡µ½APPÉÏÃæ ÏÔÊ¾²¨ĞÎ
	
}
