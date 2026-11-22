#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

//停止模式+对射式红外传感器
int main()
{

	OLED_Init();
	CountSensor_Init();
	
	OLED_ShowString(1,1,"Count:");
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	
    while(1){
		OLED_ShowNum(1,7,CountSensor_Get(),5);
		
		OLED_ShowString(2,1,"Running");
		Delay_ms(500);
		OLED_ShowString(2,1,"       ");
		Delay_ms(500);
		
		PWR_EnterSTOPMode(PWR_Regulator_ON,PWR_STOPEntry_WFI);
		
    }
	
} 
 