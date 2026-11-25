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
	
	MyFLASH_ErasePage(0x0800FC00);
	MyFLASH_ProgramWord(0x0800FC00,0x12345678);//写入字
	MyFLASH_ProgramHalfWord(0x0800FC10,0xABCD);//写入半字
	
	
    while(1){
		KeyNum=Key_GetNum();
		if(KeyNum==1){
			MyFLASH_EraseALLPages();//全擦除
		}
		if(KeyNum==2){
			MyFLASH_ErasePage(0x08000000);//页擦除
		}
		
    }
	
} 
 