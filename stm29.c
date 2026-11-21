#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

//修改主频
//文件右键属性,关闭只可读
int main()
{

	OLED_Init();
	
	OLED_ShowString(1,1,"SYSCLK:");
	OLED_ShowNum(1,8,SystemCoreClock,8);
	
	
    while(1){
		OLED_ShowString(2,1,"Running");
		Delay_ms(1000);
		OLED_ShowString(2,1,"       ");
		Delay_ms(1000);
    }
	
} 
 