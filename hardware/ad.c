#include "stm32f10x.h"                  // Device header

//AD单通道
void AD_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//分频 (72MHz/6=12MHz)(不能超12MHz)
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入 防止GPIO口输入输出对模拟电压产生影响
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
	//配置ADC1的通道0,把他放在转换序列的第一位,采样时间设为55.5个ADC时钟周期 (菜单)
	
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode=DISABLE;//单次转换模式(触发一次,采一次电压就停)
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//软件触发
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//独立模式(ADC1,ADC2,互不干扰)
	ADC_InitStructure.ADC_NbrOfChannel=1;//一个通道
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//扫描模式(自动采集多个通道)
	ADC_Init(ADC1,&ADC_InitStructure);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);//复位校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);//判断校准完成
	ADC_StartCalibration(ADC1);//开启ADC自校准
	while((ADC_GetCalibrationStatus(ADC1))==SET);
	
}

uint16_t AD_GetValue(uint8_t ADC_Channel){
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_55Cycles5);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//ADC1开始一次转换(开始测电压)
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET); 
	return ADC_GetConversionValue(ADC1);
	
}

