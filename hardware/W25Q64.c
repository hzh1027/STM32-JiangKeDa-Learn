#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_LNS.h"

//软件SPI读写W25Q64
void W25Q64_Init(void){
	MySPI_Init();
	
}

void W25Q64_ReadID(uint8_t *MID,uint16_t *DID){
	MySPI_Start();
	MySPI_SwapByte(0x9F);//发送读取指令
	*MID=MySPI_SwapByte(0xFF);//制造商AD
	*DID=MySPI_SwapByte(0xFF);//设备AD
	*DID<<=8;//左移8位
	*DID |=MySPI_SwapByte(0xFF);//结合
	MySPI_Stop();
}

void W25Q64_WriteEnable(void){
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);//发送写使能指令
	MySPI_Stop();
}
//等待芯片
void W25Q64_WaitBusy(void){
	uint32_t Timeout=10000;
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);//发送"读取状态寄存器1"的指令
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01)==0x01){ //
		Timeout--;
		if(Timeout==0){
			break;
		}
	}
	MySPI_Stop();
}
//写入
void W25Q64_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t Count){
	uint16_t i;
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);//指令(执行写入)
	MySPI_SwapByte(Address >> 16);//三次发送24位目标内存地址
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i=0;i<Count;i++){
		MySPI_SwapByte(DataArray[i]);//发送数据
	}
	MySPI_Stop();
	W25Q64_WaitBusy();//等待从机忙完
}
//扇区擦除
void W25Q64_SectorErase(uint32_t Address){
    
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	MySPI_Stop();
	
	W25Q64_WaitBusy();//等待
}
//读取
void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint16_t Count){
	uint32_t i;
	W25Q64_WriteEnable();
	
	MySPI_Start();
	MySPI_SwapByte(W25Q64_READ_DATA);
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i=0;i<Count;i++){
		DataArray[i]=MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
	
}


	