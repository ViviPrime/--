#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);
void PWM_SetCompare_Left(uint16_t Compare);//1号电机pwm设置函数
void PWM_SetCompare_Right(uint16_t Compare);//2号电机pwm设置函数

#endif
