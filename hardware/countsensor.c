#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

//中断信号从 GPIO->AFIO->EXTI->NVIC->CPU
void CountSensor_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //AFIO 功能 外部中断线路 复用功能 重映射
	// EXTI(外部中断) 和 NVIC(外部中断控制器) 的时钟是常开  
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	//检测电平边沿(上升沿/下降沿),产生中断请求信号
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);//AFLO外部中断引脚配置
	
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line=EXTI_Line14;//EXTI的第14个线路
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;//
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;//中断模式 还有事件模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发 还有上升沿触发,上升沿和下降沿触发
	EXTI_Init(&EXTI_InitStructure);
	
	//管理所有的中断优先级
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级分组 2:抢占2位,响应2位
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;//中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢断优先级 Group2(0-3) 数字小优先级高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;//子优先级 同上
	NVIC_Init(&NVIC_InitStructure);
	
}

uint16_t CountSensor_Get(void){

return CountSensor_Count;
}

void EXTI15_10_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line14)==SET){//判断EXTI14中断标志是不是为1
		
	   CountSensor_Count++;
	   EXTI_ClearITPendingBit(EXTI_Line14); 
	}



}





