#include "control.h"	
#include "filter.h"	
#include "PointToPoint.h"
u8 Flag_Target,Flag_Change;                             //��ر�־λ
u8 temp1;                                               //��ʱ����
float Gyro_K=0.004;     				  											//�����Ǳ���ϵ��
int j;
#define a_PARAMETER          (0.0915f)               
#define b_PARAMETER          (0.0875f)        
/**************************************************************************
�������ܣ�С���˶���ѧģ��
��ڲ�����X Y Z �����ٶȻ���λ��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis(float Vy,float Vz)
{
        Target_A   = Vy-Vz*(a_PARAMETER+b_PARAMETER);
        Target_B   = Vy-Vz*(a_PARAMETER+b_PARAMETER);
	      Target_C   = Vy+Vz*(a_PARAMETER+b_PARAMETER);
				Target_D   = Vy+Vz*(a_PARAMETER+b_PARAMETER);  
}
/**************************************************************************
�������ܣ���ȡλ�ÿ��ƹ����ٶ�ֵ
��ڲ�����X Y Z ����λ�ñ仯��
����  ֵ����
**************************************************************************/
void Kinematic_Analysis2(float Vy,float Vz)
{
	      Rate_A   = Vy-Vz*(a_PARAMETER+b_PARAMETER);
        Rate_B   = Vy-Vz*(a_PARAMETER+b_PARAMETER);
	      Rate_C   = Vy+Vz*(a_PARAMETER+b_PARAMETER);
				Rate_D   = Vy+Vz*(a_PARAMETER+b_PARAMETER);
}

/**************************************************************************
�������ܣ����еĿ��ƴ��붼��������
         5ms��ʱ�ж���MPU6050��INT���Ŵ���
         �ϸ�֤���������ݴ����ʱ��ͬ��				 
**************************************************************************/
int EXTI15_10_IRQHandler(void) 

{    
	if(INT==0)		
	{   
		  EXTI->PR=1<<15;                                                      //���LINE5�ϵ��жϱ�־λ  		
		   Flag_Target=!Flag_Target;
		watch++;
		  if(delay_flag==1)   //delay_flag  �����0 ��ѭ����ִ��һ��
			 {
				 if(++delay_100==20)	 delay_100=0,delay_flag=0;                     //���������ṩ100ms�ľ�׼��ʱ
			 }
			 
		  if(Flag_Target==1)                                                  //10ms��ȡһ�������Ǻͼ��ٶȼƵ�ֵ
			{
					if(Usart_Flag==0&&Usart_ON_Flag==1)  memcpy(rxbuf,Urxbuf,8*sizeof(u8));	//��������˴��ڿ��Ʊ�־λ�����봮�ڿ���ģʽ
					//Read_DMP();                                                           //===������̬		
			return 0;	                                               
			} 
					
		//===10ms����һ�Σ�Ϊ�˱�֤M�����ٵ�ʱ���׼�����ȶ�ȡ����������
			//һ������ÿ10ms��һ��
			Encoder_A=Read_Encoder(2);                                          		//===��ȡ��������ֵ
			Position_A+=Encoder_A;                                                	 //===���ֵõ��ٶ�   
			Encoder_B=Read_Encoder(3);                                          		//===��ȡ��������ֵ
			Position_B+=Encoder_B;                                                 	//===���ֵõ��ٶ�   
			Encoder_C=-Read_Encoder(4);                                         		//===��ȡ��������ֵ
			Position_C+=Encoder_C;                                                 	//===���ֵõ��ٶ�   
			Encoder_D=-Read_Encoder(5);                                         		//===��ȡ��������ֵ
			Position_D+=Encoder_D;                                                 	//===���ֵõ��ٶ� 
			
	  	//Read_DMP();                                                            	//===������̬	
			if(speed_mode==1)
				Led_Flash(50);  
			else
				Led_Flash(100);  
				//===LED��˸;����ģʽ 1s�ı�һ��ָʾ�Ƶ�״̬	
			

			      
		  if(Usart_ON_Flag==1) CAN_N_Usart_Control();          //�ӵ����ڻ���CANң�ؽ���ָ��֮��ʹ��CAN�ʹ��ڿ�������
			if(RC_Velocity>0&&RC_Velocity<15)  RC_Velocity=15;        

		 { 			 
		  if(Run_Flag==0||1)//�ٶ�ģʽ
			{		
				if(Usart_ON_Flag==0)  Get_RC(Run_Flag);                //===���ں�CAN���ƶ�δʹ�ܣ����������ң��ָ
				Motor_A=Incremental_PI_A(Encoder_A,Target_A);                         //===�ٶȱջ����Ƽ�����A����PWM
				Motor_B=Incremental_PI_B(Encoder_B,Target_B);                         //===�ٶȱջ����Ƽ�����B����PWM
				Motor_C=Incremental_PI_C(Encoder_C,Target_C);                         //===�ٶȱջ����Ƽ�����C����PWM
				Motor_D=Incremental_PI_D(Encoder_D,Target_D);                         //===�ٶȱջ����Ƽ�����C����PWM
			}
			 else//λ��ģʽ
			{
				if(Usart_ON_Flag==0) //===���ں�CAN���ƶ�δʹ�ܣ����������ң��ָ��
				 {	
					if(Turn_Flag==0) 	
						Flag_Direction=click_RC();     
					Get_RC(Run_Flag);
				 }
					Motor_A=Position_PID_A(Position_A,Target_A)>>8;//λ�ñջ����ƣ�������A�ٶ��ڻ���������
					Motor_B=Position_PID_B(Position_B,Target_B)>>8;//λ�ñջ����ƣ�������B�ٶ��ڻ���������
					Motor_C=Position_PID_C(Position_C,Target_C)>>8;//λ�ñջ����ƣ�������C�ٶ��ڻ���������
				 	Motor_D=Position_PID_D(Position_D,Target_D)>>8;//λ�ñջ����ƣ�������C�ٶ��ڻ���������
					//��ʱpwm��ȫ�������޴��������С
			   if(rxbuf[0]!=2)  Count_Velocity();   //���ǵ���λ�ÿ��ƹ��̵��ٶȴ�С
					else 	
					Xianfu_Velocity(RC_Velocity,RC_Velocity,RC_Velocity,RC_Velocity); 
					Motor_A=Incremental_PI_A(Encoder_A,-Motor_A);         //===�ٶȱջ����Ƽ�����A����PWM
					Motor_B=Incremental_PI_B(Encoder_B,-Motor_B);         //===�ٶȱջ����Ƽ�����B����PWM
					Motor_C=Incremental_PI_C(Encoder_C,-Motor_C);         //===�ٶȱջ����Ƽ�����C����PWM
					Motor_D=Incremental_PI_D(Encoder_D,-Motor_D);         //===�ٶȱջ����Ƽ�����C����PWM
				
				
				
			}	 
			//Motor_A=7199;
			//Motor_B=7199;
			//Motor_C=7199;
			//Motor_D=7199;
      Xianfu_Pwm(6900);                     //===PWM�޷�

			
			Set_Pwm(Motor_A,Motor_B,Motor_C,Motor_D);     //===��ֵ��PWM�Ĵ���  
		 }
		 

 }
	
 
	 return 0;	 
} 


/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d)//a ��ǰ  //b  ��� //c   ��ǰ  //d  �Һ�
{
			if(motor_a<100&&motor_a>-100)
															PWMA=0, INA1=0,INA2=0;
    	else if(motor_a>0)			PWMA=motor_a/7200.0*(7200.0-siqu)+siqu, INA1=1,INA2=0;
			else										PWMA=myabs(motor_a)/7200.0*(7200.0-siqu)+siqu,INA1=0,INA2=1;

			if(motor_b<100&&motor_b>-100)
															PWMB=0, INB1=0,INB2=0;
    	else if(motor_b>0)			PWMB=motor_b/7200.0*(7200.0-siqu)+siqu, INB1=1,INB2=0;
			else										PWMB=myabs(motor_b)/7200.0*(7200.0-siqu)+siqu,INB1=0,INB2=1;
	
			if(motor_c<100&&motor_c>-100)
															PWMC=0, INC1=0,INC2=0;
    	else if(motor_c>0)			PWMC=motor_c/7200.0*(7200.0-siqu)+siqu, INC1=1,INC2=0;
			else										PWMC=myabs(motor_c)/7200.0*(7200.0-siqu)+siqu,INC1=0,INC2=1;
	
			if(motor_d<100&&motor_d>-100)
															PWMD=0, IND1=0,IND2=0;
    	else if(motor_d>0)			PWMD=motor_d/7200.0*(7200.0-siqu)+siqu, IND1=1,IND2=0;
			else										PWMD=myabs(motor_d)/7200.0*(7200.0-siqu)+siqu,IND1=0,IND2=1;
}

/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(int amplitude)
{	
    if(Motor_A<-amplitude) Motor_A=-amplitude;	
		if(Motor_A>amplitude)  Motor_A=amplitude;	
	  if(Motor_B<-amplitude) Motor_B=-amplitude;	
		if(Motor_B>amplitude)  Motor_B=amplitude;		
	  if(Motor_C<-amplitude) Motor_C=-amplitude;	
		if(Motor_C>amplitude)  Motor_C=amplitude;		
	  if(Motor_D<-amplitude) Motor_D=-amplitude;	
	  if(Motor_D>amplitude)  Motor_D=amplitude;		
}
/**************************************************************************
�������ܣ�λ��PID���ƹ������ٶȵ�����
��ڲ������ޡ���ֵ
����  ֵ����
**************************************************************************/
void Xianfu_Velocity(int amplitude_A,int amplitude_B,int amplitude_C,int amplitude_D)
{	/*
    if(Motor_A<-amplitude_A) Motor_A=-amplitude_A;	//λ�ÿ���ģʽ�У�A����������ٶ�
		if(Motor_A>amplitude_A)  Motor_A=amplitude_A;	  //λ�ÿ���ģʽ�У�A����������ٶ�
	  if(Motor_B<-amplitude_B) Motor_B=-amplitude_B;	//λ�ÿ���ģʽ�У�B����������ٶ�
		if(Motor_B>amplitude_B)  Motor_B=amplitude_B;		//λ�ÿ���ģʽ�У�B����������ٶ�
	  if(Motor_C<-amplitude_C) Motor_C=-amplitude_C;	//λ�ÿ���ģʽ�У�C����������ٶ�
		if(Motor_C>amplitude_C)  Motor_C=amplitude_C;		//λ�ÿ���ģʽ�У�C����������ٶ�
	  if(Motor_D<-amplitude_D) Motor_D=-amplitude_D;	//λ�ÿ���ģʽ�У�D����������ٶ�
		if(Motor_D>amplitude_D)  Motor_D=amplitude_D;		//λ�ÿ���ģʽ�У�D����������ٶ�
	*/
}

/**************************************************************************
�������ܣ�����ֵ����
��ڲ�����long int
����  ֵ��unsigned int
**************************************************************************/
u32 myabs(long int a)
{ 		   
	  u32 temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
int transformYaw()
{
	int Yaw_reg=Yaw-yaw_offset;
	if(Yaw_reg>180)
		Yaw_reg-=360;
	else if(Yaw_reg<-180)
		Yaw_reg+=360;
	return Yaw_reg;
}
/**************************************************************************
�������ܣ�����PI������
��ڲ���������������ֵ��Ŀ���ٶ�
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
�����ǵ��ٶȿ��Ʊջ�ϵͳ���棬ֻʹ��PI����
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm-=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //����ƫ��
	 Pwm-=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_C (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Pwm-=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
int Incremental_PI_D (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Pwm-=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //������һ��ƫ�� 
	 return Pwm;                         //�������
}
/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
**************************************************************************/
int Position_PID_A (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_B (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_C (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
int Position_PID_D (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //����ƫ��
	 Integral_bias+=Bias;	                                 //���ƫ��Ļ���
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //λ��ʽPID������
	 Last_Bias=Bias;                                       //������һ��ƫ�� 
	 return Pwm;                                           //�������
}
/**************************************************************************
�������ܣ�ͨ������ָ���С������ң��
��ڲ���������ָ��
����  ֵ����
**************************************************************************/
void Get_RC(u8 mode)
{

	  if(mode==0)//�ٶ�
		{	
				 switch(Flag_Direction)   //�������
				 {
				 case 0:      Move_Y=0; 			 			 	 Move_Z=0;        break;	 
				 case 1:      Move_Y=RC_Velocity;  	 	 Move_Z=0;       	break;
				 case 2:      Move_Y=RC_Velocity;  	 	 Move_Z=-RC_Velocity;   	break;
				 case 3:      Move_Y=0;      				 	 Move_Z=-RC_Velocity;   	break;
				 case 4:      Move_Y=-RC_Velocity;  	 Move_Z=RC_Velocity;  	break;
				 case 5:      Move_Y=-RC_Velocity;  	 Move_Z=0;        break;
				 case 6:      Move_Y=-RC_Velocity;  	 Move_Z=-RC_Velocity;    break;
				 case 7:      Move_Y=0;     	 			 	 Move_Z=RC_Velocity;    break;
				 case 8:      Move_Y=RC_Velocity; 	 	 Move_Z=RC_Velocity;    break; 
				 default:     break;	 
			 }

			 
	 }
		 else if(mode==1)//λ��ģʽ
		{	
				 switch(Flag_Direction)   //�������
				 {
				 case 1:     	Move_Y+=RC_Position;  	      															 break;
				 case 2:      Move_Y+=RC_Position;           Move_Z-=Z_Position;       		 break;
				 case 3:      															 Move_Z-=Z_Position;           break;
				 case 4:      Move_Y-=RC_Position; 					 Move_Z+=Z_Position;       		 break;
				 case 5:      Move_Y-=RC_Position;  	 			 															 break;
				 case 6:      Move_Y-=RC_Position; 					 Move_Z-=Z_Position;       		 break;
				 case 7:      															 Move_Z+=Z_Position;     		   break;
				 case 8:      Move_Y+=RC_Position;           Move_Z+=Z_Position;           break; 	 
				 default: break;	 
			 }
	 }
		 Kinematic_Analysis(Move_Y*0.7882,Move_Z*1.3757);//�õ�����Ŀ��ֵ�������˶�ѧ����
}
/**************************************************************************
�������ܣ�ÿ�����λ�ÿ��ƹ����ٶȼ��㣬������Ϊλ�ÿ���ר��
��ڲ�������
����  ֵ����
**************************************************************************/
void Count_Velocity(void)
{
	static double Last_Target_Y,Last_Target_Z,Divider;
	double Bias_Y,Bias_Z;
	Bias_Y=(Move_Y-Last_Target_Y);	//��Y��λ����
	Bias_Z=(Move_Z-Last_Target_Z);	//��Z��λ����
	if(Bias_Y!=0||Bias_Z!=0)Divider=sqrt(Bias_Y*Bias_Y+Bias_Z*Bias_Z);
	if(Bias_Y!=0||Bias_Z!=0) Kinematic_Analysis2(Bias_Y,Bias_Z);

	Xianfu_Velocity(RC_Velocity*myabs(Rate_A)/Divider,RC_Velocity*myabs(Rate_B)/Divider,RC_Velocity*myabs(Rate_C)/Divider,RC_Velocity*myabs(Rate_D)/Divider); 
	Last_Target_Y=Move_Y;   //����Y����һ�ε�λ����Ϣ�����ڵ���
	Last_Target_Z=Move_Z;   //����Z����һ�ε�λ����Ϣ�����ڵ���
}
/**************************************************************************
�������ܣ�����CAN���ߴ��ڿ���ָ����д���
��ڲ�������
����  ֵ����
**************************************************************************/
//alpha Խ��  ��仯Խ��
int compute_ray(float aver)
{/*
	static float x=0;
	static int ll1,ll2,lr1,lr2;
	float delta_x1=aver*(1-alpha1)/alpha1*0.25;
	float delta_x2=aver*(1-alpha2)/alpha2*0.4;//���±�̫��  ��һ�θ��� 0.6 0.8 �ĳ��� 0.4 0.6��Ϊ���۱�̫��
  int8_t left1=LEFT1,left2=LEFT2,right1=RIGHT1,right2=RIGHT2;		
	if(left2==1&&left1==1)
		left1=0;
	if(right2==1&&right1==1)
		right1=0;	

	if((left1==1||left2==1)&&(right1==1||right2==1))
	{
		
		x=0;
		return 0;
	}
	else
	{	
		if(left2==1)
		{
			if(x<0)	
				x=1.0;
			x=(x+delta_x2)*alpha2;
		}
		else if(left1==1)
		{
			if(x<0)
				x=1.0;
			x=(x+delta_x1)*alpha1;
		}
		else if(right2==1)
		{
			if(x>0)
				x=-1.0;
			x=(x-delta_x2)*alpha2;
		}
		else if(right1==1)
		{
			if(x>0)
				x=-1.0;
			x=(x-delta_x1)*alpha1;
		}
		else
			x=x*0.96;
  }
	if(ll1==1&&left1==0)
		l1=x;
	if(ll2==1&&left2==0)
		l2=x;
	if(lr1==1&&right1==0)
		r1=x;
	if(lr2==1&&right2==0)
		r2=x;	
	ll1=left1;
	ll2=left2;
	lr1=right1;
	lr2=right2;
	*/
	//return((int)x);
	return 0;
}
void CAN_N_Usart_Control(void)
{
	float aver;
	int distance=0;
	const float angle_constrain=15;
	int temp;
	//static int i=0;
	//��Ϣ�������Ӧ
	//0---��ǰ����x
	//1---��ǰ����y
	//2---Ŀ������x
	//3---Ŀ������y
	//4---00000000�����λ��Ӧ1234����λ�������λ��  ����λ��ʾĿ��ֵ�Ƿ���� 1��0��  ����λ��ʾ�Ƿ�ͣ��---1��   0  ��   ����Ϊ0
	//7---У��  �ݶ�0x66           ����λҲ�ݶ�Ϊ0x66
	if(rxbuf[4]&0x10)
	{
		Target_A=0;
		Target_B=0;
		Target_C=0;
		Target_D=0;
		now_x=rxbuf[0]+(((unsigned int)(rxbuf[4]&0x08))<<5);
		now_y=rxbuf[1]+(((unsigned int)(rxbuf[4]&0x04))<<6);
		go_x=rxbuf[2]+(((unsigned int)(rxbuf[4]&0x02))<<7); //?00110010?   
		go_y=rxbuf[3]+(((unsigned int)(rxbuf[4]&0x01))<<8);	//?00111100?   00		
		Led_Flash(0);
		Indicate(0);
	}
	else if((rxbuf[4]&0x20)!=0)
	{
		now_x=rxbuf[0]+(((unsigned int)(rxbuf[4]&0x08))<<5);
		now_y=rxbuf[1]+(((unsigned int)(rxbuf[4]&0x04))<<6);
		go_x=rxbuf[2]+(((unsigned int)(rxbuf[4]&0x02))<<7); //?00110010?   
		go_y=rxbuf[3]+(((unsigned int)(rxbuf[4]&0x01))<<8);	//?00111100?   00
		if(watch>70)
		{
			watch=88;
			go_x=now_x;
			go_y=now_y;
			Indicate(0);
		}
		else
		{
			Indicate(60);
		}
		if(((rxbuf[4]&0x80) >> 7 )== 0x01)
			yaw_offset=90;
		else
			yaw_offset=-180;
		delta_angle=getAngleToAim(now_x,now_y,go_x,go_y);
		distance=myabs(sqrt((now_x-go_x)*(now_x-go_x)+(now_y-go_y)*(now_y-go_y)));

		if(distance<8)
		{
			Target_A=0;
			Target_B=0;
			Target_C=0;
			Target_D=0;
		}
		else if(delta_angle>angle_constrain)//    10-20   180---50
		{		
			Target_A=-(10+(delta_angle-angle_constrain)*8.2/17.0);
			if(speed_mode==1)
				Target_A=-(10+(delta_angle-angle_constrain)*8.6/17.0);
			if(Target_A<-50)
				Target_A=-50;
			else if(Target_A>50)
				Target_A=50;
				
			Target_B=Target_A;
			Target_C=-Target_A;
			Target_D=Target_C;//cdΪ��
		}
		else if(delta_angle<-angle_constrain)//    10-20   180---50
		{
			Target_A=(10+(-delta_angle-angle_constrain)*8.2/17.0);
			if(speed_mode==1)
				Target_A=(10+(-delta_angle-angle_constrain)*8.6/17.0);
			if(Target_A<-50)
				Target_A=-50;
			else if(Target_A>50)
				Target_A=50;			
			Target_B=Target_A;
			Target_C=-Target_A;
			Target_D=Target_C;//cdΪ��
		}			 
		else
		{
			if(distance>40)
			{
				if(speed_mode==1)
					aver=NORMAL_SPEED*(2.5-0.78*(myabs(delta_angle))/angle_constrain);
				else
					aver=NORMAL_SPEED*(2.2-0.6*(myabs(delta_angle))/angle_constrain); 
			}
			else if(distance>25)//20 to 25   1.3 to 1.2
			{
				if(speed_mode==1)
				{
					aver=NORMAL_SPEED*((distance/40.0*2.45));	
					aver=aver*(1-0.38*(myabs(delta_angle)/angle_constrain));				
				}
				else
				{
					aver=NORMAL_SPEED*((distance/40.0*2.3));	
					aver=aver*(1-0.38*(myabs(delta_angle)/angle_constrain));					
				}
			}				
			else if(distance>8)
				if(speed_mode==1)
					aver=(((angle_constrain-myabs(delta_angle))/angle_constrain)*0.77+0.6*(distance-5)/20.0)*NORMAL_SPEED;
				else
					aver=(((angle_constrain-myabs(delta_angle))/angle_constrain)*0.77+0.55*(distance-5)/20.0)*NORMAL_SPEED;
			else
				aver=0;
			
			showuse=compute_ray((float)(aver));
			showuse=0;
			if(speed_mode==0)
				temp=(-(float)(delta_angle)/angle_constrain/2.0*(aver>NORMAL_SPEED?aver:NORMAL_SPEED))*1.1;
			else if(speed_mode==1)
				temp=(-(float)(delta_angle)/angle_constrain/2.0*(aver>NORMAL_SPEED?aver:NORMAL_SPEED))*1.14;
				
				
			Target_A=(temp>0?0:temp);                                        // abΪ��   //#1 Ϊ60.0+  compute_ray((float)(aver))
			Target_B=(temp
			>0?0:temp);
			Target_C=((-temp)>0?0:(-temp));
			Target_D=((-temp)>0?0:(-temp));//cdΪ��	
				
			if(showuse>0)
			{
				Target_C-=showuse;
				Target_D-=showuse;
			}
			else
			{
				Target_A+=showuse;
				Target_B+=showuse;
			}
			
			Target_A+=aver;
			Target_B+=aver;
			Target_C+=aver;
			Target_D+=aver;
		}

	}
	
}
