#include "control.h"	
#include "filter.h"	
#include "PointToPoint.h"
u8 Flag_Target,Flag_Change;                             //相关标志位
u8 temp1;                                               //临时变量
float Gyro_K=0.004;     				  											//陀螺仪比例系数
int j;
#define a_PARAMETER          (0.0915f)               
#define b_PARAMETER          (0.0875f)        
/**************************************************************************
函数功能：小车运动数学模型
入口参数：X Y Z 三轴速度或者位置
返回  值：无
**************************************************************************/
void Kinematic_Analysis(float Vy,float Vz)
{
        Target_A   = Vy-Vz*(a_PARAMETER+b_PARAMETER);
        Target_B   = Vy-Vz*(a_PARAMETER+b_PARAMETER);
	      Target_C   = Vy+Vz*(a_PARAMETER+b_PARAMETER);
				Target_D   = Vy+Vz*(a_PARAMETER+b_PARAMETER);  
}
/**************************************************************************
函数功能：获取位置控制过程速度值
入口参数：X Y Z 三轴位置变化量
返回  值：无
**************************************************************************/
void Kinematic_Analysis2(float Vy,float Vz)
{
	      Rate_A   = Vy-Vz*(a_PARAMETER+b_PARAMETER);
        Rate_B   = Vy-Vz*(a_PARAMETER+b_PARAMETER);
	      Rate_C   = Vy+Vz*(a_PARAMETER+b_PARAMETER);
				Rate_D   = Vy+Vz*(a_PARAMETER+b_PARAMETER);
}

/**************************************************************************
函数功能：所有的控制代码都在这里面
         5ms定时中断由MPU6050的INT引脚触发
         严格保证采样和数据处理的时间同步				 
**************************************************************************/
int EXTI15_10_IRQHandler(void) 

{    
	if(INT==0)		
	{   
		  EXTI->PR=1<<15;                                                      //清除LINE5上的中断标志位  		
		   Flag_Target=!Flag_Target;
		watch++;
		  if(delay_flag==1)   //delay_flag  被设成0 主循环就执行一次
			 {
				 if(++delay_100==20)	 delay_100=0,delay_flag=0;                     //给主函数提供100ms的精准延时
			 }
			 
		  if(Flag_Target==1)                                                  //10ms读取一次陀螺仪和加速度计的值
			{
					if(Usart_Flag==0&&Usart_ON_Flag==1)  memcpy(rxbuf,Urxbuf,8*sizeof(u8));	//如果解锁了串口控制标志位，进入串口控制模式
					//Read_DMP();                                                           //===更新姿态		
			return 0;	                                               
			} 
					
		//===10ms控制一次，为了保证M法测速的时间基准，首先读取编码器数据
			//一下内容每10ms来一次
			Encoder_A=Read_Encoder(2);                                          		//===读取编码器的值
			Position_A+=Encoder_A;                                                	 //===积分得到速度   
			Encoder_B=Read_Encoder(3);                                          		//===读取编码器的值
			Position_B+=Encoder_B;                                                 	//===积分得到速度   
			Encoder_C=-Read_Encoder(4);                                         		//===读取编码器的值
			Position_C+=Encoder_C;                                                 	//===积分得到速度   
			Encoder_D=-Read_Encoder(5);                                         		//===读取编码器的值
			Position_D+=Encoder_D;                                                 	//===积分得到速度 
			
	  	//Read_DMP();                                                            	//===更新姿态	
			if(speed_mode==1)
				Led_Flash(50);  
			else
				Led_Flash(100);  
				//===LED闪烁;常规模式 1s改变一次指示灯的状态	
			

			      
		  if(Usart_ON_Flag==1) CAN_N_Usart_Control();          //接到串口或者CAN遥控解锁指令之后，使能CAN和串口控制输入
			if(RC_Velocity>0&&RC_Velocity<15)  RC_Velocity=15;        

		 { 			 
		  if(Run_Flag==0||1)//速度模式
			{		
				if(Usart_ON_Flag==0)  Get_RC(Run_Flag);                //===串口和CAN控制都未使能，则接收蓝牙遥控指
				Motor_A=Incremental_PI_A(Encoder_A,Target_A);                         //===速度闭环控制计算电机A最终PWM
				Motor_B=Incremental_PI_B(Encoder_B,Target_B);                         //===速度闭环控制计算电机B最终PWM
				Motor_C=Incremental_PI_C(Encoder_C,Target_C);                         //===速度闭环控制计算电机C最终PWM
				Motor_D=Incremental_PI_D(Encoder_D,Target_D);                         //===速度闭环控制计算电机C最终PWM
			}
			 else//位置模式
			{
				if(Usart_ON_Flag==0) //===串口和CAN控制都未使能，则接收蓝牙遥控指令
				 {	
					if(Turn_Flag==0) 	
						Flag_Direction=click_RC();     
					Get_RC(Run_Flag);
				 }
					Motor_A=Position_PID_A(Position_A,Target_A)>>8;//位置闭环控制，计算电机A速度内环的输入量
					Motor_B=Position_PID_B(Position_B,Target_B)>>8;//位置闭环控制，计算电机B速度内环的输入量
					Motor_C=Position_PID_C(Position_C,Target_C)>>8;//位置闭环控制，计算电机C速度内环的输入量
				 	Motor_D=Position_PID_D(Position_D,Target_D)>>8;//位置闭环控制，计算电机C速度内环的输入量
					//此时pwm完全可以无限大或者无限小
			   if(rxbuf[0]!=2)  Count_Velocity();   //这是调节位置控制过程的速度大小
					else 	
					Xianfu_Velocity(RC_Velocity,RC_Velocity,RC_Velocity,RC_Velocity); 
					Motor_A=Incremental_PI_A(Encoder_A,-Motor_A);         //===速度闭环控制计算电机A最终PWM
					Motor_B=Incremental_PI_B(Encoder_B,-Motor_B);         //===速度闭环控制计算电机B最终PWM
					Motor_C=Incremental_PI_C(Encoder_C,-Motor_C);         //===速度闭环控制计算电机C最终PWM
					Motor_D=Incremental_PI_D(Encoder_D,-Motor_D);         //===速度闭环控制计算电机C最终PWM
				
				
				
			}	 
			//Motor_A=7199;
			//Motor_B=7199;
			//Motor_C=7199;
			//Motor_D=7199;
      Xianfu_Pwm(6900);                     //===PWM限幅

			
			Set_Pwm(Motor_A,Motor_B,Motor_C,Motor_D);     //===赋值给PWM寄存器  
		 }
		 

 }
	
 
	 return 0;	 
} 


/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int motor_a,int motor_b,int motor_c,int motor_d)//a 左前  //b  左后 //c   右前  //d  右后
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
函数功能：限制PWM赋值 
入口参数：幅值
返回  值：无
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
函数功能：位置PID控制过程中速度的设置
入口参数：无、幅值
返回  值：无
**************************************************************************/
void Xianfu_Velocity(int amplitude_A,int amplitude_B,int amplitude_C,int amplitude_D)
{	/*
    if(Motor_A<-amplitude_A) Motor_A=-amplitude_A;	//位置控制模式中，A电机的运行速度
		if(Motor_A>amplitude_A)  Motor_A=amplitude_A;	  //位置控制模式中，A电机的运行速度
	  if(Motor_B<-amplitude_B) Motor_B=-amplitude_B;	//位置控制模式中，B电机的运行速度
		if(Motor_B>amplitude_B)  Motor_B=amplitude_B;		//位置控制模式中，B电机的运行速度
	  if(Motor_C<-amplitude_C) Motor_C=-amplitude_C;	//位置控制模式中，C电机的运行速度
		if(Motor_C>amplitude_C)  Motor_C=amplitude_C;		//位置控制模式中，C电机的运行速度
	  if(Motor_D<-amplitude_D) Motor_D=-amplitude_D;	//位置控制模式中，D电机的运行速度
		if(Motor_D>amplitude_D)  Motor_D=amplitude_D;		//位置控制模式中，D电机的运行速度
	*/
}

/**************************************************************************
函数功能：绝对值函数
入口参数：long int
返回  值：unsigned int
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
函数功能：增量PI控制器
入口参数：编码器测量值，目标速度
返回  值：电机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
在我们的速度控制闭环系统里面，只使用PI控制
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Incremental_PI_A (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm-=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_B (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                //计算偏差
	 Pwm-=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_C (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Pwm-=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
int Incremental_PI_D (int Encoder,int Target)
{ 	
	 static int Bias,Pwm,Last_bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Pwm-=Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 if(Pwm>7200)Pwm=7200;
	 if(Pwm<-7200)Pwm=-7200;
	 Last_bias=Bias;	                   //保存上一次偏差 
	 return Pwm;                         //增量输出
}
/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
int Position_PID_A (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
int Position_PID_B (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
int Position_PID_C (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
int Position_PID_D (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 if(Integral_bias>100000)Integral_bias=100000;
	 if(Integral_bias<-100000)Integral_bias=-100000;
	 Pwm=Position_KP*Bias+Position_KI/100*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //增量输出
}
/**************************************************************************
函数功能：通过串口指令对小车进行遥控
入口参数：串口指令
返回  值：无
**************************************************************************/
void Get_RC(u8 mode)
{

	  if(mode==0)//速度
		{	
				 switch(Flag_Direction)   //方向控制
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
		 else if(mode==1)//位置模式
		{	
				 switch(Flag_Direction)   //方向控制
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
		 Kinematic_Analysis(Move_Y*0.7882,Move_Z*1.3757);//得到控制目标值，进行运动学分析
}
/**************************************************************************
函数功能：每个电机位置控制过程速度计算，本函数为位置控制专用
入口参数：无
返回  值：无
**************************************************************************/
void Count_Velocity(void)
{
	static double Last_Target_Y,Last_Target_Z,Divider;
	double Bias_Y,Bias_Z;
	Bias_Y=(Move_Y-Last_Target_Y);	//求Y轴位移量
	Bias_Z=(Move_Z-Last_Target_Z);	//求Z轴位移量
	if(Bias_Y!=0||Bias_Z!=0)Divider=sqrt(Bias_Y*Bias_Y+Bias_Z*Bias_Z);
	if(Bias_Y!=0||Bias_Z!=0) Kinematic_Analysis2(Bias_Y,Bias_Z);

	Xianfu_Velocity(RC_Velocity*myabs(Rate_A)/Divider,RC_Velocity*myabs(Rate_B)/Divider,RC_Velocity*myabs(Rate_C)/Divider,RC_Velocity*myabs(Rate_D)/Divider); 
	Last_Target_Y=Move_Y;   //保存Y轴上一次的位置信息，便于调用
	Last_Target_Z=Move_Z;   //保存Z轴上一次的位置信息，便于调用
}
/**************************************************************************
函数功能：接收CAN或者串口控制指令进行处理
入口参数：无
返回  值：无
**************************************************************************/
//alpha 越大  则变化越慢
int compute_ray(float aver)
{/*
	static float x=0;
	static int ll1,ll2,lr1,lr2;
	float delta_x1=aver*(1-alpha1)/alpha1*0.25;
	float delta_x2=aver*(1-alpha2)/alpha2*0.4;//害怕变太多  第一次更改 0.6 0.8 改成了 0.4 0.6因为肉眼变太多
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
	//信息解释与对应
	//0---当前坐标x
	//1---当前坐标y
	//2---目标坐标x
	//3---目标坐标y
	//4---00000000最低四位对应1234补充位（坐标高位）  第三位表示目标值是否更新 1是0否  第四位表示是否停机---1是   0  否   其余为0
	//7---校验  暂定0x66           其余位也暂定为0x66
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
			Target_D=Target_C;//cd为右
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
			Target_D=Target_C;//cd为右
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
				
				
			Target_A=(temp>0?0:temp);                                        // ab为左   //#1 为60.0+  compute_ray((float)(aver))
			Target_B=(temp
			>0?0:temp);
			Target_C=((-temp)>0?0:(-temp));
			Target_D=((-temp)>0?0:(-temp));//cd为右	
				
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
