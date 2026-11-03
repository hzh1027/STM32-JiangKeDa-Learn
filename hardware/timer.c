#include "stm32f10x.h"                  // Device header


void Timer_Init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_InternalClockConfig(TIM2);//选择内部时钟(CK_INT=72M)
	
	//定时器时基单元配置
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructture;
	TIM_TimeBaseInitStructture.TIM_ClockDivision=TIM_CKD_DIV1;//时钟分频(1为不分频 还有2,4分频)
	TIM_TimeBaseInitStructture.TIM_CounterMode=TIM_CounterMode_Up;//计数模式(向上计数 还有向下,中心对齐)
	TIM_TimeBaseInitStructture.TIM_Period=10000-1;//ARR自动重装寄存器的值(CNT到这数触发更新事件,更新中断) -1是因为从0开始
	TIM_TimeBaseInitStructture.TIM_Prescaler=7200-1;//PSC预分频器
	//T=7200*10000/72000000=1s(1秒溢出一次)
	TIM_TimeBaseInitStructture.TIM_RepetitionCounter=0;//重复计数器(高级计数器才使用)
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructture);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//更新中断标志位清除,避免刚初始化完就进中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//TIM_IT_Update 为更新中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructture;
	NVIC_InitStructture.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructture.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructture.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructture.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructture);
	
	TIM_Cmd(TIM2,ENABLE);

}

//中断函数
/*void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)==SET){
	
	}

    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
} */

