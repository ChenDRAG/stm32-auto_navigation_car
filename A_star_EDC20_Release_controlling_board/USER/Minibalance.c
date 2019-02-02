#include "sys.h"
#define RUN_FLAG 0   //0为速度模式  1为位置模式
#define KEY 1   //0为参数debug模式  1为固定参数模式
//相比于 v8 速度提升   角度保持15  转向略微加强  增加LED显示是否接收不到信息

//频率已经变成一s5次
u8 Flag_Left,Flag_Right,Flag_Direction=0;   //蓝牙遥控相关的变量
u8 Flag_Stop=1;                 //停止标志位和 显示标志位 默认停止 显示打开
int Encoder_A,Encoder_B,Encoder_C,Encoder_D;          //编码器的脉冲计数
long int Position_A,Position_B,Position_C,Position_D,Rate_A,Rate_B,Rate_C,Rate_D; //PID控制相关变量
int Encoder_A_EXTI;                       //通过外部中断读取的编码器数据                       
long int Motor_A,Motor_B,Motor_C,Motor_D;        //电机PWM变量
long int Target_A,Target_B,Target_C,Target_D;     //电机目标值
u8 delay_100,delay_flag;                          //延时相关变量
u8 Run_Flag=0;  												//蓝牙遥控相关变量和运行状态标志位
u8 rxbuf[8],Urxbuf[8],Usart_ON_Flag=0,Usart_Flag,PID_Send,Flash_Send;  //CAN和串口控制相关变量
u8 txbuf[8],txbuf2[8],Turn_Flag;             //CAN发送相关变量
float Pitch,Roll,Yaw,Move_Y,Move_Z;   //三轴角度和XYZ轴目标速度
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash相关数组
int siqu=3800;
float alpha1=0.5,alpha2=0.3;
int l1,l2,r1,r2;

float	Position_KP=40,Position_KI=0,Position_KD=40;  //位置控制PID参数
float Velocity_KP=10,Velocity_KI=10;	          //速度控制PID参数 

int RC_Velocity=45,RC_Position=10000,Z_Position=36000;         //设置遥控的速度和位置值
volatile long all_receive_num=0;
int yaw_offset;//人工设置小车车头朝向x轴方向时的yaw值
int delta_angle;
int now_x,now_y,go_x,go_y;
int showuse;
int watch=0;
char speed_mode=1;

//注意之后要减少dmp初始化实时间

int main(void)
{ 
	Stm32_Clock_Init(9);            //=====系统时钟设置
	delay_init(72);                 //=====延时初始化
	JTAG_Set(JTAG_SWD_DISABLE);     //=====关闭JTAG接口
	JTAG_Set(SWD_ENABLE);           //=====打开SWD接口 可以利用主板的SWD接口调试
	LED_Init();                     //=====初始化与 LED 连接的硬件接口
	
	Run_Flag=RUN_FLAG;          //=====启动的过程中，根据模式选择开关确定进入位置模式还是速度模式
	uart_init();	
	uart2_init(36,9600);            //=====串口2初始化
  uart3_init(36,115200);          //=====串口3初始化 
	Encoder_Init_TIM2();            //=====编码器接口
	Encoder_Init_TIM3();            //=====编码器接口
	Encoder_Init_TIM4();            //=====初始化编码器C
	Encoder_Init_TIM5();            //=====初始化编码器D
	KEY_Init(); 
	if(MODE_KEY==1)
		speed_mode=1;
	else
		speed_mode=0;		
	IIC_Init();                     //=====IIC初始化
  MPU6050_initialize();           //=====MPU6050初始化	
  DMP_Init();                     //=====初始化DMP     
	MiniBalance_PWM_Init(7199,0);   //=====初始化PWM 10KHZ，用于驱动电机
	
	if(KEY==0) Flash_Read();        //=====读取Flash里面的参数
	
  MiniBalance_EXTI_Init();                    //=====MPU6050 5ms定时中断初始化
	
	Velocity_KP=Velocity_KP*7200.0/(7200.0-siqu);
	Velocity_KI=Velocity_KI*7200.0/(7200.0-siqu);   //siqu is 4000  the  this is 22.5
	Led_Flash(0);
	Indicate(1);
	while(1)//100ms 一次
		{	
	  	if(Flash_Send==1)          //写入PID参数到Flash,由app控制该指令
			{
				Flash_Write();	
				Flash_Send=0;	
			}	
			APP_Show();	    
			
			delay_flag=1;	
			delay_100=0;
			while(delay_flag);	       //通过MPU6050的INT中断实现的50ms精准延时				
		} 
}
