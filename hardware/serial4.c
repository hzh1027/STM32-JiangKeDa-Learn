#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

char Serial4_RxData[100];
char Serial4_RxPacket[100];
uint8_t Serial4_TxPacket[4];
uint8_t Serial4_RxFlag;

//串口收发(文本)
void Serial4_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//串口接收
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;//波特率(通信速率,双方都得相同)
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//硬件流控制(防止数据丢失)关
	USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;//发送模式 | 接收模式
	USART_InitStructure.USART_Parity=USART_Parity_No;//奇偶效验(无)
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位(设置为1位停止)
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//数据字长(没有开启奇偶一般为8位)
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启RXNE标志位到NVIC的输出
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	
	
}

//发送一个字符
void Serial4_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);//把字节写入串口发送数据寄存器(TDR)
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
}
//发送一个数组
void Serial4_SendArray(uint8_t *Array,uint16_t Length){
	uint16_t i;
	for(i=0;i<Length;i++){
		Serial4_SendByte(Array[i]);//发送一个数组
	
	}
}
//发送字符串
void Serial4_SendString(char* String){
	uint8_t i;
	for(i=0;String[i]!='\0';i++){
		Serial4_SendByte(String[i]);
	}
	
}

//发送数据包
void Serial4_SendPacket(void){
	Serial4_SendByte(0xFF);//包头
	Serial4_SendArray(Serial4_TxPacket,4);//有效数据
	Serial4_SendByte(0xFE);//包尾
}

//复原函数
uint8_t Serial4_GetRxFlag(void){
	if(Serial4_RxFlag==1){ //检查接收标志位
		Serial4_RxFlag=0;
		return 1;
	}
	return 0;
}

//中断函数
void USART1_IRQHandler(void)
{
    static uint8_t RxState = 0;
    static uint8_t pRxPacket = 0;
    
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
        uint8_t RxData = USART_ReceiveData(USART1);
        
        if (RxState == 0)
        {
            // 同时满足才执行接收，否则就是发送过快，可以有效避免数据包错位
            if (RxData == '@' && Serial4_RxFlag == 0)
            {
                RxState = 1;
                pRxPacket = 0;
            }
        }
        else if (RxState == 1)
        {
            // 判断包尾
            if (RxData == '\r')
            {
                RxState = 2;
            }
            else
            {
                Serial4_RxPacket[pRxPacket] = RxData;
                pRxPacket ++;
            }
        }
        else if (RxState == 2)
        {
            if (RxData == '\n')
            {
                RxState = 0;
                // 给字符数组最后加一个字符串结束标志位0
                Serial4_RxPacket[pRxPacket] = '\0';
                Serial4_RxFlag = 1;
            }
        }
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
