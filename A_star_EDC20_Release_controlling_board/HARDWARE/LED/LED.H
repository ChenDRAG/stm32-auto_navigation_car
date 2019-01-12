#ifndef __LED_H
#define __LED_H
#include "sys.h"
//LED 端口定义
#define LED PAout(4) 
#define LED_wrong PBout(2) 

void LED_Init(void);  //初始化
void Led_Flash(u16 time);
void Indicate(u16 on);

#endif

