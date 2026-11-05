#include "stm32f10x.h"                  // Device header

void IC_Init(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//TIM3 对应 PA6|PA7
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructture;
	TIM_TimeBaseInitStructture.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructture.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructture.TIM_Period=65536-1; //ARR 分辨率=1/ARR+1
	TIM_TimeBaseInitStructture.TIM_Prescaler=72-1; //PSC 频率72M/72 1MHz
	TIM_TimeBaseInitStructture.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructture);
	//输入捕获模式占空比
	TIM_ICInitTypeDef TIM_ICInitStructure; 
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//选择TIM3 通道1 (PA6)作为捕获引脚
	TIM_ICInitStructure.TIM_ICFilter=0xF;//滤波器(最大强度)
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//上升沿触发() 低->高 CCR1=CNT
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//触发一个边沿捕获一次(还有DIV2|DICV4)
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//直连模式
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	TIM_PWMIConfig(TIM3,&TIM_ICInitStructure);//只支持通道1和2
	//等同下面
	
	//PWMI模式测频率占空比
	/*TIM_ICInitTypeDef TIM_ICInitStructure; 
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_2;//选择TIM3 通道2 (PA6)作为捕获引脚
	TIM_ICInitStructure.TIM_ICFilter=0xF;//滤波器(最大强度)
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;//下降沿触发() 
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//触发一个边沿捕获一次(还有DIV2|DICV4)
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_IndirectTI;//交叉模式
	TIM_ICInit(TIM3,&TIM_ICInitStructure); */
	
	
	TIM_SelectInputTrigger(TIM3,TIM_TS_TI1FP1);//选择TI1FP1作为定时器的触发源 TI1FP1:PA6的信号
	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);//接触到触发信号,CNT就立刻清零复位
	
	TIM_Cmd(TIM3,ENABLE);

}

//获取频率
uint32_t IC_GetFreq(void){
	return 1000000/(TIM_GetCapture1(TIM3)+1); //一个周期的频率值 +1消除误差
	//     1MHz
}

//获取占空比
uint32_t IC_GetDuty(void){
	return (TIM_GetCapture2(TIM3)+1)*100/(TIM_GetCapture1(TIM3)+1);
	
}

