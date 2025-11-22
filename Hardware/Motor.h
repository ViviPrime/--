#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_LeftSpeed(int16_t Speed);
void Motor_RightSpeed(int16_t Speed);
void Stop(uint8_t t);

#endif
