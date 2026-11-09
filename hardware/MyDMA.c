#include "stm32f10x.h"

uint16_t MyDMA_Size;

//DMA数据转运
void MyDMA_Init(uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size=Size;
    DMA_InitTypeDef DMA_InitStructure;// 开启 DMA1 的 AHB 外设时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // DMA 使用前先关闭通道，防止配置出错
	
    DMA_Cmd(DMA1_Channel1, DISABLE);// 设置源地址（AddrA）
    DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;// 设置目标地址（AddrB）
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;// 内存地址自增 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// 内存数据宽度：半字
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// 外设数据宽度：半字（uint16_t）
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// 传输模式：正常模式，传输一遍就停止，不是循环传输
    DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;// 传输方向：外设作为源（M2M 内存到内存）
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;// 外设地址自增：每传完一个数据，地址自动 + 对应步长
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// DMA 通道优先级：中等优先级
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;// 开启内存到内存模式，M2M 只能软件触发
    DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;// 将配置写入 DMA1 通道 1
	DMA_InitStructure.DMA_BufferSize=Size;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);// 设置本次要传输的数据个数
	
    DMA_Cmd(DMA1_Channel1, DISABLE);
}

// DMA 传输函数，等待传输完成
void MyDMA_Transfer(void)
{
	DMA_Cmd(DMA1_Channel1, ENABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,MyDMA_Size);
	DMA_Cmd(DMA1_Channel1, DISABLE);
    // 等待传输完成标志 TC1，没完成就一直等
    while (DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);

    // 清除传输完成标志，避免下次误判
    DMA_ClearFlag(DMA1_FLAG_TC1);

}

