#include "stm32f10x.h"                  // Device header

//PWM驱动LED呼吸灯
void PWM_Init(void){
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//重映射(更改复用的引脚)
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	//GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);将 A0 改到 B15
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;PWM驱动舵机
	//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;PWM驱动直流电机
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructture;
	TIM_TimeBaseInitStructture.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructture.TIM_Period=100-1; //ARR 分辨率=1/ARR+1
	TIM_TimeBaseInitStructture.TIM_Prescaler=720-1; //PSC
	//T=720*100/72000000=1ms(1毫秒溢出一次)
	TIM_TimeBaseInitStructture.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructture);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//向上计数,CNT<CCR 有效电平(反则无效)
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//高电平有效
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//CCR 占空比=CRR/ARR+1
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	//TIM_OC2Init(TIM2,&TIM_OCInitStructure);PWM驱动舵机
    //舵机参数 ARR=20000-1  PSC=72-1 
	//TIM_OC3Init(TIM2,&TIM_OCInitStructure);PWM驱动直流电机
	//直流电机参数 ARR= 100-1  PSC=36-1(消音)
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);

}

void PWM_SetCompare1(uint16_t Compare){
	TIM_SetCompare1(TIM2,Compare);

}
//PWM驱动舵机
void PWM_SetCompare2(uint16_t Compare){
	TIM_SetCompare2(TIM2,Compare);

}

//PWM驱动直流电机
void PWM_SetCompare3(uint16_t Compare){
	TIM_SetCompare3(TIM2,Compare);

}
	


