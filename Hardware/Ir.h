#ifndef __IR_H__
#define __IR_H__

void Ir_init(void);
void IR_GetAllValues(unsigned char* sensor_values);
unsigned char IR_GetSensor(unsigned char sensor_num);

// 传感器编号定义
#define IR_LEFTMOST    0   // 最左侧传感器
#define IR_LEFT        1   // 左侧传感器  
#define IR_MIDDLE      2   // 中间传感器
#define IR_RIGHT       3   // 右侧传感器
#define IR_RIGHTMOST   4   // 最右侧传感器

#endif
