#include "show.h"

unsigned char i;          //��������
unsigned char Send_Count; //������Ҫ���͵����ݸ���
float Vol;
/*
void APP_Show(void)
{    
	  static u8 flag;
	//  int app_2,app_3,app_4;
	//app_4=100;//ԭ��Ϊ��ѹֵ������ǿ��д��100
	 // app_2=Move_X*0.7;  if(app_2<0)app_2=-app_2;			                   //�Ա��������ݾ������ݴ������ͼ�λ�
		//app_3=Move_Y*0.7;  if(app_3<0)app_3=-app_3;	
	 //if(Run_Flag==1)//app_2=0,app_3=0;
	  flag=!flag;
	 if(PID_Send==1)//����PID����
	{
		//printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",(int)RC_Velocity,(int)RC_Position,(int)Z_Position,(int)Position_KP,(int)Position_KI,(int)Position_KD,(int)Velocity_KP,(int)Velocity_KI,0);//��ӡ��APP����	
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$",6666,(int)RC_Position,(int)Z_Position,(int)Position_KP,(int)Position_KI,(int)Position_KD,(int)Velocity_KP,(int)Velocity_KI,0);//��ӡ��APP����	
		PID_Send=0;	
		
	}	
   else	if(flag==0)// 
			printf("{A%d:%d:%d:%d}$",(int)((Target_A+Target_C)/2.0),(int)(Target_A-Target_C),delta_angle,transformYaw()); //��ӡ��APP����
	 else
	 //printf("{B%d:%d:%d:%d}$",(int)Pitch,(int)Roll,(int)Yaw,(int)Encoder_D);//��ӡ��APP���� ��ʾ����
		printf("{B%d:%d:%d:%d:%d}$",(int)now_x,(int)now_y,(int)go_x,(int)go_y,(int)all_receive_num);//��ӡ��APP���� ��ʾ����	 
}
*/

/*�к�����
void APP_Show(void)
{    
	  static u8 flag;
	  flag=!flag;
	if(flag==1)
		//printf("{%d:%d:%d:%d:%d:%d:%d:%d}$",rxbuf[0],rxbuf[1],rxbuf[2],rxbuf[3],rxbuf[4],rxbuf[5],rxbuf[6],rxbuf[7]);//��ӡ��APP���� ��ʾ����	 
//	else
		printf("{A%d:%d:%d:%d
		:%d:%d}$\n",(int)now_x,(int)now_y,(int)go_x,(int)go_y,(int)transformYaw(),(int)all_receive_num);//��ӡ��APP���� ��ʾ���
	else
		printf("{B%d:%d:%d:%d}$\n",(int)l1,(int)l2,(int)r1,(int)r2);
	//
		//printf("{B%d:%d:%d:%d}$\n",(int)LEFT1,(int)LEFT2,(int)RIGHT1,(int)RIGHT2);
		//else
			//printf("{B%d:%d:%d:%d}$\n",(int)Encoder_A,(int)Encoder_B,(int)Target_A,(int)Motor_A);//��ӡ��APP���� ��ʾ����
	
}
*/
void APP_Show(void)
{    
	  static u8 flag;
	  flag=!flag;
	if(flag==1)
		//printf("{%d:%d:%d:%d:%d:%d:%d:%d}$",rxbuf[0],rxbuf[1],rxbuf[2],rxbuf[3],rxbuf[4],rxbuf[5],rxbuf[6],rxbuf[7]);//��ӡ��APP���� ��ʾ����	 
//	else
		printf("{A%d:%d:%d:%d:%d:%d}$\n",(int)now_x,(int)now_y,(int)go_x,(int)go_y,(int)transformYaw(),(int)all_receive_num);//��ӡ��APP���� ��ʾ���
	//else
		//printf("{B%d:%d:%d:%d}$\n",(int)l1,(int)l2,(int)r1,(int)r2);
	//
		//printf("{B%d:%d:%d:%d}$\n",(int)LEFT1,(int)LEFT2,(int)RIGHT1,(int)RIGHT2);
		//else
			//printf("{B%d:%d:%d:%d}$\n",(int)Encoder_A,(int)Encoder_B,(int)Target_A,(int)Motor_A);//��ӡ��APP���� ��ʾ����
	
}
