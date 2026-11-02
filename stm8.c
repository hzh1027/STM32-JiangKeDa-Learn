#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"

int main(){
	OLED_Init();
	CountSensor_Init();
	OLED_ShowString(1,3,"Count");
	while(1){
	OLED_ShowNum(2,1,CountSensor_Get(),5);
	}
	
}
