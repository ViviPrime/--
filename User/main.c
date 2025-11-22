#include "stm32f10x.h"  // Device header
#include "Delay.h"
#include "Motor.H"
#include "Ir.H"
#include "OLED.h"
#include "Key.h"

// 状态枚举
typedef enum {
    STATE_MENU = 0,     // 菜单状态
    STATE_RUNNING = 1,  // 运行中
    STATE_STOP = 2      // 停止
} SystemState;

// 全局变量
SystemState g_SystemState = STATE_MENU;

// 函数声明
void DisplayMenu(void);
void KeyInput(void);
void CarControl(void);

// 显示菜单界面
void DisplayMenu(void)
{
    OLED_Printf(0, 0, OLED_8X16, "Car Control Menu");
    
    if(g_SystemState == STATE_MENU)
    {
        OLED_Printf(0, 16, OLED_8X16, "Press KEY to");
        OLED_Printf(0, 32, OLED_8X16, "Start Car");
    }
    else if(g_SystemState == STATE_RUNNING)
    {
        OLED_Printf(0, 16, OLED_8X16, "Running....");
        OLED_Printf(0, 32, OLED_8X16, "Press KEY to stop");
    }
    else if(g_SystemState == STATE_STOP)
    {
        OLED_Printf(0, 16, OLED_8X16, "Stopped");
        OLED_Printf(0, 32, OLED_8X16, "Press KEY to menu");
    }
}

int main(void)
{   
    // 初始化各模块
    Motor_Init();
    Ir_init();
    key_init();
    OLED_Init();
    
    // 显示初始菜单
    OLED_Clear();
    DisplayMenu();
    
    while(1)
    {
        // 处理按键输入
        KeyInput();
        
        // 根据状态执行相应逻辑
        switch(g_SystemState)
        {
            case STATE_MENU:
                // 菜单状态，小车不动
                Motor_LeftSpeed(0);
                Motor_RightSpeed(0);
                break;
                
            case STATE_RUNNING:
                // 运行状态，执行循迹逻辑
                CarControl();
                break;
                
            case STATE_STOP:
                // 停止状态，小车停止
                Motor_LeftSpeed(0);
                Motor_RightSpeed(0);
                break;
        }
        
        Delay_ms(10); // 短暂延时
    }
}

// 处理按键输入
void KeyInput(void)
{
    unsigned char keyValue = key_getnum();
    
    if(keyValue == 1) // KEY1按下
    {
        Delay_ms(20); // 消抖
        
        switch(g_SystemState)
        {
            case STATE_MENU:
                // 从菜单进入运行状态
                g_SystemState = STATE_RUNNING;
                OLED_Clear();
                DisplayMenu();
                break;
                
            case STATE_RUNNING:
                // 运行中按按键停止
                g_SystemState = STATE_STOP;
                OLED_Clear();
                DisplayMenu();
                break;
                
            case STATE_STOP:
                // 停止状态按按键返回菜单
                g_SystemState = STATE_MENU;
                OLED_Clear();
                DisplayMenu();
                break;
        }
        
        // 等待按键释放
        while(key_getnum() == 1);
        Delay_ms(20);
    }
}

// 小车控制逻辑（循迹算法）
void CarControl(void)
{
    unsigned char Left_xunji_flag = Left_xunji();
    unsigned char Right_xunji_flag = Right_xunji();
    
    // 循迹控制逻辑
    if(Left_xunji_flag == 0 && Right_xunji_flag == 0)
    {        
        // 直行
        Motor_LeftSpeed(50);
        Motor_RightSpeed(50);
    }
    else if(Left_xunji_flag == 1 && Right_xunji_flag == 0)
    {                    
        // 左转
        Motor_LeftSpeed(-50);
        Motor_RightSpeed(50);
    }
    else if(Left_xunji_flag == 0 && Right_xunji_flag == 1)
    {    
        // 右转
        Motor_LeftSpeed(50);
        Motor_RightSpeed(-50);
    }
    else if(Left_xunji_flag == 1 && Right_xunji_flag == 1)
    {
        // 检测到十字路口或终点，停止
        Motor_LeftSpeed(0);
        Motor_RightSpeed(0);
        Delay_s(2);
        g_SystemState = STATE_STOP; // 自动进入停止状态
        OLED_Clear();
        DisplayMenu();
    }
}
