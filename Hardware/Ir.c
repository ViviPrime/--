#include "stm32f10x.h"                  // Device header

void Ir_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init (GPIOA,&GPIO_InitStructure);
}

unsigned char Left_xunji(void)
{
	unsigned char Left_xunji_flag=0;
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==0)
	{
		Left_xunji_flag=0;
	}
	
	else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==1)
	{
		Left_xunji_flag=1;
	}
	
	return Left_xunji_flag;
}

unsigned char Right_xunji(void)
{
	unsigned char Right_xunji_flag=0;
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==0)
	{
		Right_xunji_flag=0;
	}
	
	else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==1)
	{
		Right_xunji_flag=1;
	}
	
	return Right_xunji_flag;
}
