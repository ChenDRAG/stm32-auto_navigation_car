#ifndef __LED_H
#define __LED_H
#include "sys.h"
//LED �˿ڶ���
#define LED PAout(4) 
#define LED_wrong PBout(2) 

void LED_Init(void);  //��ʼ��
void Led_Flash(u16 time);
void Indicate(u16 on);

#endif
