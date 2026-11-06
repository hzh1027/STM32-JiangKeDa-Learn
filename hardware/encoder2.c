#include "stm32f10x.h"                  // Device header

//编码器接口测数
void Encoder2_Init(void){
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;//TIM3 对应 PA6|PA7
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	//TIM_InternalClockConfig(TIM3);编码器接口托管时钟(带方向控制的外部时钟)
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructture;
	TIM_TimeBaseInitStructture.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructture.TIM_CounterMode=TIM_CounterMode_Up;//计数方向也被托管
	TIM_TimeBaseInitStructture.TIM_Period=65536-1; //ARR 分辨率=1/ARR+1
	TIM_TimeBaseInitStructture.TIM_Prescaler=1-1; //PSC 
	TIM_TimeBaseInitStructture.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructture);
	
	//输入捕获模式占空比
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	TIM_ICStructInit(&TIM_ICInitStructure);//给默认值的
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//选择TIM3 通道1 (PA6)作为捕获引脚
	TIM_ICInitStructure.TIM_ICFilter=0xF;//滤波器(最大强度)
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter=0xF;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_Cmd(TIM3,ENABLE);
	
}

int16_t Encoder_Get2(void){
	int16_t Temp;
	Temp=TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3,0);
	return Temp;
	

}
