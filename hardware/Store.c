#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"

#define STORE_START_ADDRESS  0x0800FC00
#define STORE_COUNT          512

uint16_t Store_Data[512];

//用SRAM缓存数组管理FLASH
void Store_Init(void){
	//初始化
	if(MyFLASH_ReadHalfWord(STORE_START_ADDRESS)!=0xA5A5){ //0xA5A5 是标志位(判读是否有数据)
		MyFLASH_ErasePage(STORE_START_ADDRESS);//先擦除
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS,0xA5A5);//标记有数据
		for(uint16_t i=1;i<STORE_COUNT;i++){
			MyFLASH_ProgramHalfWord(STORE_START_ADDRESS+i*2,0x0000);
		}
	}
	for(uint16_t i=0;i<STORE_COUNT;i++){
		Store_Data[i]=MyFLASH_ReadHalfWord(STORE_START_ADDRESS+i*2);
	}
	Store_Data[0]=0xA5A5;
}
//保存数据
void Store_Save(void){
	MyFLASH_ErasePage(STORE_START_ADDRESS);//擦除
	for(uint16_t i=0;i<STORE_COUNT;i++){ //写入
		MyFLASH_ProgramHalfWord(STORE_START_ADDRESS+i*2,Store_Data[i]);
	}
}
//清除
void Store_Clear(void){
	Store_Data[0]=0xA5A5;
	for(uint16_t i=1;i<STORE_COUNT;i++){
		Store_Data[i]=0x0000;
	}
	Store_Save();
}



