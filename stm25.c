#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"

uint8_t MID;
uint16_t DID;

uint8_t ArrayWrite[]={0x01,0x02,0x03,0x04};
uint8_t ArrayRead[4];


//软件SPI读写W25Q64
int main()
{

	OLED_Init();
	W25Q64_Init();
	W25Q64_ReadID(&MID,&DID);
	
	OLED_ShowString(1,1,"MID:   DID:");
	OLED_ShowString(2,1,"W");
	OLED_ShowString(3,1,"R");
	
	OLED_ShowHexNum(1,5,MID,2);
	OLED_ShowHexNum(1,12,DID,4);
	
	W25Q64_SectorErase(0x000000);//扇区擦除 ()
	W25Q64_PageProgram(0x000000,ArrayWrite,4);
	W25Q64_ReadData(0x000000,ArrayWrite,4);
	OLED_ShowHexNum(2,3,ArrayWrite[0],2);
	OLED_ShowHexNum(2,6,ArrayWrite[1],2);
	OLED_ShowHexNum(2,9,ArrayWrite[2],2);
	OLED_ShowHexNum(2,12,ArrayWrite[3],2);
	
	OLED_ShowHexNum(3,3,ArrayWrite[0],2);
	OLED_ShowHexNum(3,6,ArrayWrite[1],2);
	OLED_ShowHexNum(3,9,ArrayWrite[2],2);
	OLED_ShowHexNum(3,12,ArrayWrite[3],2);
	
	
	
    while(1){
		
    }
	
} 
 