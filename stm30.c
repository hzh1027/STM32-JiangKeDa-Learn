#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "SERIAL2.h"

//睡眠模式
int main()
{

	OLED_Init();
	OLED_ShowString(1,1,"RxData:");
	
	Serial_Init();
	
    while(1){
		if(Serial_GetRxFlag()==1){
			uint8_t RxData=Serial_GetRxData();
			Serial_SendByte(RxData);
			OLED_ShowHexNum(1,8,RxData,2);
		}
		OLED_ShowString(2,1,"Running");
		Delay_ms(100);
		OLED_ShowString(2,1,"       ");
		Delay_ms(100);
		
		__WFI();//(睡眠模式)  只有串口发送数据Running才会闪
    }
	
} 
 