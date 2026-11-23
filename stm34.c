#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "KEY.h"

//窗口看门狗
int main()
{

	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1,1,"WWDG TEST");
	
	
	if(RCC_GetFlagStatus(RCC_FLAG_WWDGRST)==SET){ //判断复位是否由独立看门狗触发
		OLED_ShowString(2,1,"WWDGRST");
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
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	WWDG_SetPrescaler(WWDG_Prescaler_8);// T=0.91ms
	//计数器频率=PCLK1/4096/预分频系数   36MHz/4096/8=1.098Hz 
	WWDG_SetWindowValue(0x40 | 21);//窗口值 118-85=33 33*0.9=30ms(最早)
	WWDG_Enable(0x40 | 54);//计数器118 118-64(0x40)=54 33*54=50ms(最晚)
	
	
	
    while(1){
		Key_GetNum();//控制时间
		
		
		OLED_ShowString(4,1,"FEED");
		Delay_ms(20);
		OLED_ShowString(4,1,"    ");
		Delay_ms(20);
	    
		WWDG_SetCounter(0x40 | 54);//放后面与前面隔开
    }
	
} 
 