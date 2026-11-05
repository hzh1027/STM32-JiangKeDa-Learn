#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

uint8_t KeyNum;
int8_t Speed;


int main(){
	OLED_Init();
	PWM_Init();
	IC_Init();
	PWM_SetPrescaler(720-1);  //频率    Freq=72M/(PSC+1)/(ARR+1)
	//                                 1KHz              99+1
	PWM_SetCompare1(50);      //占空比   Duty=CRR/ARR+1
	//                                  50%   
	OLED_ShowString(1,1,"Freq:00000Hz");
	OLED_ShowString(2,1,"Duty:00%");

	while(1){
	   OLED_ShowNum(1,6,IC_GetFreq(),5);
	   OLED_ShowNum(2,6,IC_GetDuty(),2);
	}
	
	
}
