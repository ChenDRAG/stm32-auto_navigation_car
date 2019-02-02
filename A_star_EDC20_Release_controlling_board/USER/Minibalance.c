#include "sys.h"
#define RUN_FLAG 0   //0Ϊ�ٶ�ģʽ  1Ϊλ��ģʽ
#define KEY 1   //0Ϊ����debugģʽ  1Ϊ�̶�����ģʽ
//����� v8 �ٶ�����   �Ƕȱ���15  ת����΢��ǿ  ����LED��ʾ�Ƿ���ղ�����Ϣ

//Ƶ���Ѿ����һs5��
u8 Flag_Left,Flag_Right,Flag_Direction=0;   //����ң����صı���
u8 Flag_Stop=1;                 //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder_A,Encoder_B,Encoder_C,Encoder_D;          //���������������
long int Position_A,Position_B,Position_C,Position_D,Rate_A,Rate_B,Rate_C,Rate_D; //PID������ر���
int Encoder_A_EXTI;                       //ͨ���ⲿ�ж϶�ȡ�ı���������                       
long int Motor_A,Motor_B,Motor_C,Motor_D;        //���PWM����
long int Target_A,Target_B,Target_C,Target_D;     //���Ŀ��ֵ
u8 delay_100,delay_flag;                          //��ʱ��ر���
u8 Run_Flag=0;  												//����ң����ر���������״̬��־λ
u8 rxbuf[8],Urxbuf[8],Usart_ON_Flag=0,Usart_Flag,PID_Send,Flash_Send;  //CAN�ʹ��ڿ�����ر���
u8 txbuf[8],txbuf2[8],Turn_Flag;             //CAN������ر���
float Pitch,Roll,Yaw,Move_Y,Move_Z;   //����ǶȺ�XYZ��Ŀ���ٶ�
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash�������
int siqu=3800;
float alpha1=0.5,alpha2=0.3;
int l1,l2,r1,r2;

float	Position_KP=40,Position_KI=0,Position_KD=40;  //λ�ÿ���PID����
float Velocity_KP=10,Velocity_KI=10;	          //�ٶȿ���PID���� 

int RC_Velocity=45,RC_Position=10000,Z_Position=36000;         //����ң�ص��ٶȺ�λ��ֵ
volatile long all_receive_num=0;
int yaw_offset;//�˹�����С����ͷ����x�᷽��ʱ��yawֵ
int delta_angle;
int now_x,now_y,go_x,go_y;
int showuse;
int watch=0;
char speed_mode=1;

//ע��֮��Ҫ����dmp��ʼ��ʵʱ��

int main(void)
{ 
	Stm32_Clock_Init(9);            //=====ϵͳʱ������
	delay_init(72);                 //=====��ʱ��ʼ��
	JTAG_Set(JTAG_SWD_DISABLE);     //=====�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);           //=====��SWD�ӿ� �������������SWD�ӿڵ���
	LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	
	Run_Flag=RUN_FLAG;          //=====�����Ĺ����У�����ģʽѡ�񿪹�ȷ������λ��ģʽ�����ٶ�ģʽ
	uart_init();	
	uart2_init(36,9600);            //=====����2��ʼ��
  uart3_init(36,115200);          //=====����3��ʼ�� 
	Encoder_Init_TIM2();            //=====�������ӿ�
	Encoder_Init_TIM3();            //=====�������ӿ�
	Encoder_Init_TIM4();            //=====��ʼ��������C
	Encoder_Init_TIM5();            //=====��ʼ��������D
	KEY_Init(); 
	if(MODE_KEY==1)
		speed_mode=1;
	else
		speed_mode=0;		
	IIC_Init();                     //=====IIC��ʼ��
  MPU6050_initialize();           //=====MPU6050��ʼ��	
  DMP_Init();                     //=====��ʼ��DMP     
	MiniBalance_PWM_Init(7199,0);   //=====��ʼ��PWM 10KHZ�������������
	
	if(KEY==0) Flash_Read();        //=====��ȡFlash����Ĳ���
	
  MiniBalance_EXTI_Init();                    //=====MPU6050 5ms��ʱ�жϳ�ʼ��
	
	Velocity_KP=Velocity_KP*7200.0/(7200.0-siqu);
	Velocity_KI=Velocity_KI*7200.0/(7200.0-siqu);   //siqu is 4000  the  this is 22.5
	Led_Flash(0);
	Indicate(1);
	while(1)//100ms һ��
		{	
	  	if(Flash_Send==1)          //д��PID������Flash,��app���Ƹ�ָ��
			{
				Flash_Write();	
				Flash_Send=0;	
			}	
			APP_Show();	    
			
			delay_flag=1;	
			delay_100=0;
			while(delay_flag);	       //ͨ��MPU6050��INT�ж�ʵ�ֵ�50ms��׼��ʱ				
		} 
}
