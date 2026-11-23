#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "KEY.h"

//独立看门狗
int main()
{

	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1,1,"IWDG TEST");
	
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST)==SET){ //判断复位是否由独立看门狗触发
		OLED_ShowString(2,1,"IWDGRST");
		Delay_ms(500);
		OLED_ShowString(2,1,"       ");
		Delay_ms(100);
		RCC_ClearFlag();
	}else{
		OLED_ShowString(2,1,"RST");
		Delay_ms(500);
		OLED_ShowString(2,1,"       ");
		Delay_ms(100);
	}
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//解除写保护(向寄存器写入0x5555)
	IWDG_SetPrescaler(IWDG_Prescaler_16);//预分频
	//超时时间=(预分频系数*(重装载值+1))/时钟频率(40KHz)  16*(2499+1)/40000=1s
	IWDG_SetReload(2499);//重装值
	IWDG_ReloadCounter();//喂狗
	IWDG_Enable();
	
    while(1){
		Key_GetNum();//控制时间
		
		IWDG_ReloadCounter();
		
		OLED_ShowString(4,1,"FEED");
		Delay_ms(200);
		OLED_ShowString(4,1,"    ");
		Delay_ms(600);
    }
	
} 
 