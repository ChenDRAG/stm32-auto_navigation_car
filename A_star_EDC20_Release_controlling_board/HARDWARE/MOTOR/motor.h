#ifndef __MOTOR_H
#define __MOTOR_H
#include <sys.h>	 

//1正0负前转 反之后转
//轮子定义  ：
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
//左后轮子  b14  b13


//右后  b12   b5
//a 左前  //b  左后 //c   右前  //d  右后
void MiniBalance_PWM_Init(u16 arr,u16 psc);
void MiniBalance_Motor_Init(void);
#endif
