#include "stm32f10x.h"                  // Device header

//串口发送
void Serial_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;//波特率(通信速率,双方都得相同)
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//硬件流控制(防止数据丢失)关
	USART_InitStructure.USART_Mode=USART_Mode_Tx;//发送模式
	USART_InitStructure.USART_Parity=USART_Parity_No;//奇偶效验(无)
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位(设置为1位停止)
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//数据字长(没有开启奇偶一般为8位)
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	
	
}

void Serial_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);//把字节写入串口发送数据寄存器(TDR)
	
	
}
