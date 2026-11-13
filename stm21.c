#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "SERIAL3.h"
#include "Key.h"

uint8_t KeyNum;
uint8_t RxData;

//串口收发HEX数据包
int main(){
	OLED_Init();
	Key_Init();
	Serial3_Init();
	
	OLED_ShowString(1,1,"TxPacket");
	OLED_ShowString(3,1,"RxPacket");
	
	Serial3_TxPacket[0]=0x01;
	Serial3_TxPacket[1]=0x02;
	Serial3_TxPacket[2]=0x03;
	Serial3_TxPacket[3]=0x04;
	

	

	while(1){
		KeyNum=Key_GetNum();
		if(KeyNum==1){
			Serial3_TxPacket[0]++;
	        Serial3_TxPacket[1]++;
	        Serial3_TxPacket[2]++;
	        Serial3_TxPacket[3]++;
			
			Serial3_SendPacket();
			
			OLED_ShowHexNum(2,1,Serial3_TxPacket[0],2);
			OLED_ShowHexNum(2,4,Serial3_TxPacket[1],2);
			OLED_ShowHexNum(2,7,Serial3_TxPacket[2],2);
			OLED_ShowHexNum(2,10,Serial3_TxPacket[3],2);
			
	
		}
		if(Serial3_GetRxFlag()==1){
			OLED_ShowHexNum(4,1,Serial3_RxPacket[0],2);
			OLED_ShowHexNum(4,4,Serial3_RxPacket[1],2);
			OLED_ShowHexNum(4,7,Serial3_RxPacket[2],2);
			OLED_ShowHexNum(4,10,Serial3_RxPacket[3],2);
		}
		
	
	
    }
}

