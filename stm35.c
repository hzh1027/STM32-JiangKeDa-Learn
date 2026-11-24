#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyFLASH.h"
#include "key.h"

uint8_t KeyNum;

//读取内部FLASH
int main()
{

	OLED_Init();
	Key_Init();
	OLED_ShowHexNum(1,1,MyFLASH_ReadWord(0x08000000),8);
	OLED_ShowHexNum(2,1,MyFLASH_ReadWord(0x08000000),4);
	OLED_ShowHexNum(3,1,MyFLASH_ReadWord(0x08000000),2);
	
	
    while(1){
		
		
    }
	
} 
 