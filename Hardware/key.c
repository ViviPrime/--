#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void key_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  // 只保留一个按键
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

// 带消抖的按键检测（阻塞式）
unsigned char key_getnum(void)
{
	unsigned char keynum = 0;
	
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		Delay_ms(20);  // 消抖
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			keynum = 1;
		}
		// 等待按键释放
		while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0);
		Delay_ms(20);  // 释放消抖
	}
	
	return keynum;
}

// 非阻塞式按键检测（立即返回当前状态）
unsigned char key_getstate(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
	{
		Delay_ms(10);  // 简单消抖
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
		{
			return 1;  // 按键按下
		}
	}
	return 0;  // 按键未按下
}
