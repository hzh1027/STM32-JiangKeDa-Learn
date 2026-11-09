#include "stm32f10x.h"                  // Device header

uint16_t AD2_Value[4];

//AD单通道
void AD2_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//分频 (72MHz/6=12MHz)(不能超12MHz)
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入 防止GPIO口输入输出对模拟电压产生影响
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	//配置ADC1的通道0,把他放在转换序列的第一位,采样时间设为55.5个ADC时钟周期 (菜单)
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//多次转换模式
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//软件触发
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//独立模式(ADC1,ADC2,互不干扰)
	ADC_InitStructure.ADC_NbrOfChannel=4;//四个通道
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;//扫描模式(自动采集多个通道)
	ADC_Init(ADC1,&ADC_InitStructure);
	
	
   
    // DMA 使用前先关闭通道，防止配置出错
	DMA_InitTypeDef DMA_InitStructure;// 开启 DMA1 的 AHB 外设时钟
    DMA_Cmd(DMA1_Channel1, DISABLE);// 设置源地址（AddrA）
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;// ADC1的DR寄存器地址
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 内存地址不自增 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;// 内存数据宽度：半字
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;// 外设数据宽度：半字（uint16_t）
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;// 传输模式：正常模式，传输一遍就停止，不是循环传输
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD2_Value;// 传输方向：外设作为源（M2M 内存到内存）
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;// 外设地址自增：每传完一个数据，地址自动 + 对应步长
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// DMA 通道优先级：中等优先级
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;// 开启内存到内存模式，M2M 只能软件触发
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//不使用软件触发
	DMA_InitStructure.DMA_BufferSize=4;
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);// 设置本次要传输的数据个数
	
    DMA_Cmd(DMA1_Channel1, ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);//复位校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//判断校准完成
	ADC_StartCalibration(ADC1);//开启ADC自校准
	while((ADC_GetCalibrationStatus(ADC1))==SET);
	
}

void AD2_GetValue(void){
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,4);
	DMA_Cmd(DMA1_Channel1, DISABLE);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//ADC1开始一次转换(开始测电压)
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

