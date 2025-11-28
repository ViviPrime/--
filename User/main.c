#include "stm32f10x.h"  
#include "Delay.h"
#include "Motor.H"
#include "Ir.H"
#include "OLED.h"
#include "Key.h"

// 状态枚举
typedef enum {
    STATE_MENU = 0,     // 菜单状态
    STATE_RUNNING = 1,  // 运行状态
    STATE_STOP = 2      // 停止状态
} SystemState;

// 全局变量
SystemState g_SystemState = STATE_MENU;
uint8_t g_FirstRun = 1;

// 函数声明
void DisplayMenu(void);
void KeyInput(void);
void CarControl(void);

// 显示菜单
void DisplayMenu(void)
{
    OLED_Printf(0, 0, OLED_8X16, "5-Way Line Tracking");
    
    if(g_SystemState == STATE_MENU)
    {
        OLED_Printf(0, 16, OLED_8X16, "Press KEY to");
        OLED_Printf(0, 32, OLED_8X16, "Start Car");
    }
    else if(g_SystemState == STATE_RUNNING)
    {
        OLED_Printf(0, 16, OLED_8X16, "Running....");
        OLED_Printf(0, 32, OLED_8X16, "Press KEY to stop");
        
        // 显示五路传感器状态
        unsigned char sensor_values[5];
        IR_GetAllValues(sensor_values);
        OLED_Printf(0, 48, OLED_8X16, "%d%d%d%d%d", 
                   sensor_values[0], sensor_values[1], sensor_values[2], 
                   sensor_values[3], sensor_values[4]);
    }
    else if(g_SystemState == STATE_STOP)
    {
        OLED_Printf(0, 16, OLED_8X16, "Stopped");
        OLED_Printf(0, 32, OLED_8X16, "Press KEY to menu");
    }
}

int main(void)
{   
    // 初始化模块
    Motor_Init();
    Ir_init();
    key_init();
    OLED_Init();
    
    // 显示初始菜单
    OLED_Clear();
    DisplayMenu();
    
    while(1)
    {
        // 按键输入处理
        KeyInput();
        
        // 根据状态执行相应逻辑
        switch(g_SystemState)
        {
            case STATE_MENU:
                // 菜单状态下小车停止
                Motor_LeftSpeed(0);
                Motor_RightSpeed(0);
                break;
                
            case STATE_RUNNING:
                // 运行状态下执行循迹逻辑
                CarControl();
                break;
                
            case STATE_STOP:
                // 停止状态下小车停止
                Motor_LeftSpeed(0);
                Motor_RightSpeed(0);
                break;
        }
        
        Delay_ms(10); // 延时
    }
}

// 按键输入处理
void KeyInput(void)
{
    unsigned char keyValue = key_getnum();
    
    if(keyValue == 1&& !g_FirstRun) // KEY1按下
    {
        Delay_ms(20); // 消抖
        
        switch(g_SystemState)
        {
            case STATE_MENU:
                // 从菜单状态切换到运行状态
                g_SystemState = STATE_RUNNING;
                OLED_Clear();
                DisplayMenu();
                break;
                
            case STATE_RUNNING:
                // 从运行状态切换到停止状态
                g_SystemState = STATE_STOP;
                OLED_Clear();
                DisplayMenu();
                break;
                
            case STATE_STOP:
                // 从停止状态切换回菜单状态
                g_SystemState = STATE_MENU;
                OLED_Clear();
                DisplayMenu();
                break;
        }
        
        // 等待按键释放
        while(key_getnum() == 1);
        Delay_ms(20);
    }
	else if(keyValue == 0)
    {
        g_FirstRun = 0; // 第一次检测到按键释放后，启用按键功能
    }
}

// 五路循迹控制算法
void CarControl(void)
{
    unsigned char sensor_values[5];
    IR_GetAllValues(sensor_values);
    
    // 五路循迹逻辑
    // 传感器值：0=检测到黑线，1=检测到白线
    // 情况1：中间传感器检测到黑线 - 直行
    if(sensor_values[IR_MIDDLE] == 0 && 
       sensor_values[IR_LEFT] == 1 && 
       sensor_values[IR_RIGHT] == 1)
    {
        Motor_LeftSpeed(50);
        Motor_RightSpeed(50);
    }
    // 情况2：左传感器检测到黑线 - 轻微左转
    else if(sensor_values[IR_LEFT] == 0 && 
            sensor_values[IR_MIDDLE] == 1 && 
            sensor_values[IR_RIGHT] == 1)
    {
        Motor_LeftSpeed(30);
        Motor_RightSpeed(60);
    }
    // 情况3：右传感器检测到黑线 - 轻微右转
    else if(sensor_values[IR_RIGHT] == 0 && 
            sensor_values[IR_MIDDLE] == 1 && 
            sensor_values[IR_LEFT] == 1)
    {
        Motor_LeftSpeed(60);
        Motor_RightSpeed(30);
    }
    // 情况4：最左传感器检测到黑线 - 大左转
    else if(sensor_values[IR_LEFTMOST] == 0)
    {
        Motor_LeftSpeed(-40);
        Motor_RightSpeed(70);
    }
    // 情况5：最右传感器检测到黑线 - 大右转
    else if(sensor_values[IR_RIGHTMOST] == 0)
    {
        Motor_LeftSpeed(70);
        Motor_RightSpeed(-40);
    }
    // 情况6：中间和左传感器检测到黑线 - 小左转
    else if(sensor_values[IR_MIDDLE] == 0 && 
            sensor_values[IR_LEFT] == 0)
    {
        Motor_LeftSpeed(40);
        Motor_RightSpeed(55);
    }
    // 情况7：中间和右传感器检测到黑线 - 小右转
    else if(sensor_values[IR_MIDDLE] == 0 && 
            sensor_values[IR_RIGHT] == 0)
    {
        Motor_LeftSpeed(55);
        Motor_RightSpeed(40);
    }
    // 情况8：所有传感器都检测到白线 - 可能脱线，根据上次状态处理或停止
    else if(sensor_values[IR_LEFTMOST] == 1 && 
            sensor_values[IR_LEFT] == 1 && 
            sensor_values[IR_MIDDLE] == 1 && 
            sensor_values[IR_RIGHT] == 1 && 
            sensor_values[IR_RIGHTMOST] == 1)
    {
        // 这里可以添加脱线处理逻辑，比如保持上次转向或缓慢前进寻找黑线
        Motor_LeftSpeed(20);
        Motor_RightSpeed(20);
    }
    // 情况9：所有传感器都检测到黑线 - 十字路口或停车线
    else if(sensor_values[IR_LEFTMOST] == 0 && 
            sensor_values[IR_LEFT] == 0 && 
            sensor_values[IR_MIDDLE] == 0 && 
            sensor_values[IR_RIGHT] == 0 && 
            sensor_values[IR_RIGHTMOST] == 0)
    {
        // 遇到十字路口，可以直行或停止
        Motor_LeftSpeed(0);
        Motor_RightSpeed(0);
        Delay_s(2);
        g_SystemState = STATE_STOP; // 自动切换到停止状态
        OLED_Clear();
        DisplayMenu();
    }
    // 默认情况：直行
    else
    {
        Motor_LeftSpeed(50);
        Motor_RightSpeed(50);
    }
}
