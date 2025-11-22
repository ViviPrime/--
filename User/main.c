#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "key.h"
#include "Motor.h"
#include "Ir.h"

int main(void)
{   
	Motor_Init();
	Ir_init();
	
	unsigned char Left_xunji_flag=0;
	unsigned char Right_xunji_flag=0;
	while(1)
	{
		Left_xunji_flag=Left_xunji();
		Right_xunji_flag=Right_xunji();
		
		if(Left_xunji_flag==0&&Right_xunji_flag==0)
		{		
			Motor_LeftSpeed(50);             
			Motor_RightSpeed(50);          
		}

		else if(Left_xunji_flag==1&&Right_xunji_flag==0)
		{					
			Motor_LeftSpeed(-50);         
			Motor_RightSpeed(50);          
		}
		
		else if(Left_xunji_flag==0&&Right_xunji_flag==1)
		{	
			Motor_LeftSpeed(50);            
			Motor_RightSpeed(-50);        
		}
		
		else if(Left_xunji_flag==1&&Right_xunji_flag==1)
		{
			Stop(1);
	    }
	}
}
