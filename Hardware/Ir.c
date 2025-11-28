#include "stm32f10x.h"                  // Device header

void Ir_init(void)
{
	// 使能GPIOA和GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    // 初始化GPIOA的引脚
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化GPIOB的B1和B3引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

// 读取五路红外传感器状态
void IR_GetAllValues(unsigned char* sensor_values)
{
    sensor_values[0] = (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0) ? 0 : 1;  // 最左侧传感器
    sensor_values[1] = (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0) ? 0 : 1;  // 左侧传感器
    sensor_values[2] = (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0) ? 0 : 1;  // 中间传感器
    sensor_values[3] = (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) ? 0 : 1;  // 右侧传感器
    sensor_values[4] = (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0) ? 0 : 1;  // 最右侧传感器
}

// 获取单个传感器状态（可选，保持兼容性）
unsigned char IR_GetSensor(unsigned char sensor_num)
{
    switch(sensor_num)
    {
        case 0: return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0) ? 0 : 1;  // 最左
        case 1: return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0) ? 0 : 1;  // 左
        case 2: return (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0) ? 0 : 1;  // 中
        case 3: return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0) ? 0 : 1;  // 右
        case 4: return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0) ? 0 : 1;  // 最右
        default: return 1;  // 默认返回1（白线）
    }
}
