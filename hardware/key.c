#include "stm32f10x.h"                  // Device header
#include"Delay.h"

void Key_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	//上拉输入(引脚内部接上拉电阻,按键为高电平 1,按下接地为低电平 0)
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint8_t Key_GetNum(void){
    uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0){
	    Delay_ms(20);//消抖
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
		Delay_ms(20);
		KeyNum=1;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0){
	    Delay_ms(20);
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);
		Delay_ms(20);
		KeyNum=2;
	}
    return KeyNum;

}
