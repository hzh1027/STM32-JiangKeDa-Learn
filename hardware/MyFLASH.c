#include "stm32f10x.h"                  // Device header


//读写内部FLASH
//读字
uint32_t MyFLASH_ReadWord(uint32_t Address){
	return *((__IO uint32_t *)(0x08000000));//__IO(告诉编译器这个地址的数据可被改变,禁止编译器对读取操作进行优化)
}

//读半字
uint32_t MyFLASH_ReadHalfWord(uint32_t Address){ 
	return *((__IO uint16_t *)(0x08000000));
}

//读字节
uint32_t MyFLASH_ReadByte(uint32_t Address){ //读字节
	return *((__IO uint8_t *)(0x08000000));
}

//全擦除
void MyFLASH_EraseALLPages(void){
	FLASH_Unlock();//解锁FLASH
	FLASH_EraseAllPages();//执行全擦除
	FLASH_Lock();//上锁FLASH
}
//页擦除
void MyFLASH_ErasePage(uint32_t PageAddress){
	FLASH_Unlock();
	FLASH_ErasePage(PageAddress);
	FLASH_Lock();
}
//字节写入
void MyFLASH_ProgramWord(uint32_t Address,uint32_t Data){
	FLASH_Unlock();
	FLASH_ProgramWord(Address,Data);
	FLASH_Lock();
}
//半字写入
void MyFLASH_ProgramHalfWord(uint32_t Address,uint16_t Data){
	FLASH_Unlock();
	FLASH_ProgramHalfWord(Address,Data);
	FLASH_Lock();
}











