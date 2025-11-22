#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	
	PWM_Init();
}

void Motor_LeftSpeed(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		PWM_SetCompare_Left(-Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		GPIO_SetBits(GPIOB, GPIO_Pin_9);
		PWM_SetCompare_Left(Speed);
	}
}

void Motor_RightSpeed(int16_t Speed)
{
	if(Speed<0)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
		PWM_SetCompare_Right(-Speed);
	}
	if(Speed>0)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
		PWM_SetCompare_Right(Speed);
	}
}

void Stop(uint8_t t)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_6);
	GPIO_SetBits(GPIOB, GPIO_Pin_7);
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
	Delay_s(t);
}
