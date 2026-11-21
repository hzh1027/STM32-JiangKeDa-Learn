#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "KEY.h"
#include "OLED.h"

uint16_t ArrayWrite[]={0x1234,0x5678};
uint16_t ArrayRead[2];

//读写备份寄存器
int main()
{

	OLED_Init();
	Key_Init();
	
	OLED_ShowString(1,1,"W:");
	OLED_ShowString(2,1,"R:");
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);//使能电源(PWR)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP,ENABLE);//备份寄存器(BKP)
	
	PWR_BackupAccessCmd(ENABLE);
	
	//BKP_WriteBackupRegister(BKP_DR1,0x1234);//向备份寄存器写入 0x1234
	//OLED_ShowHexNum(1,1,BKP_ReadBackupRegister(BKP_DR1),4);//读出 
	
	
    while(1){
		uint8_t KeyNum=Key_GetNum();
		if(KeyNum==1){
			
			BKP_WriteBackupRegister(BKP_DR1,ArrayWrite[0]);
			BKP_WriteBackupRegister(BKP_DR2,ArrayWrite[1]);
			
			ArrayWrite[0]++;
			ArrayWrite[1]++;
			
			OLED_ShowHexNum(1,3,ArrayWrite[0],4);
			OLED_ShowHexNum(1,8,ArrayWrite[1],4);
		}
		
		ArrayRead[0]=BKP_ReadBackupRegister(BKP_DR1);
		ArrayRead[1]=BKP_ReadBackupRegister(BKP_DR2);
		
		OLED_ShowHexNum(2,3,ArrayRead[0],4);
		OLED_ShowHexNum(2,8,ArrayRead[1],4);
    }
	
} 
 