#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 

//1��0��ǰת ��֮��ת
//���Ӷ���  ��
#define PWMD   TIM8->CCR4  
#define PWMC   TIM8->CCR3  
#define PWMB   TIM8->CCR2 
#define PWMA   TIM8->CCR1 

#define IND1   PBout(12)  
#define INC1   PCout(2)  
#define INB1   PBout(14)  
#define INA1   PCout(0) 

#define IND2   PBout(5)  
#define INC2   PCout(3)  
#define INB2   PBout(13)  
#define INA2   PCout(1)  
//�������  b14  b13


//�Һ�  b12   b5
//a ��ǰ  //b  ��� //c   ��ǰ  //d  �Һ�
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
