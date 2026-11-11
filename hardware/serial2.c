#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData;
uint8_t Serial_RXflag;


//串口发送
void Serial_Init(void){
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
void Serial_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);//把字节写入串口发送数据寄存器(TDR)
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
}
//发送一个数组
void Serial_SendArray(uint8_t *Array,uint16_t Length){
	uint16_t i;
	for(i=0;i<Length;i++){
		Serial_SendByte(Array[i]);//发送一个数组
	
	}
}
//发送字符串
void Serial_SendString(char* String){
	uint8_t i;
	for(i=0;String[i]!='\0';i++){
		Serial_SendByte(String[i]);
	}
	
}

uint32_t Serial_Pow(uint32_t X,uint32_t Y){//x的y次方
	uint32_t Result=1;
	while(Y--){
		
		Result*=X;
	}
	return Result;
	
}
//分解数字
void Serial_SendNumber(uint32_t Number,uint8_t Length){
	uint8_t i;
	for(i=0;i<Length;i++){
		Serial_SendByte(Number/Serial_Pow(10,Length-i-1)%10+'0');
		
	}
	
}
//c语言形式打印
int fputc(int ch,FILE *f){
	Serial_SendByte(ch);
	return ch;
}
//封装一个支持格式化输出串口的打印函数(类似C语言中printf)
void Serial_Printf(char* format,...){ //...可变参数
	char String[100];
	va_list arg;//参数游标(指针)(遍历传入可变参数)
	va_start(arg,format);//初始化游标(从format开始寻参)
	vsprintf(String,format,arg);//格式化字符串
	va_end(arg);//清理结束可变参数读取(防止内存泄露)
	Serial_SendString(String);//调用串口发送字符串函数
	
}
//中断写法
//复原函数
uint8_t Serial_GetRxFlag(void){
	if(Serial_RXflag==1){ //检查接收标志位
		Serial_RXflag=0;
		return 1;
	}
	return 0;
}
//返回数据
uint8_t Serial_GetRxData(void){
	return Serial_RxData;
}
//中断函数
void USART_IRQHandler(void){
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET){
		Serial_RxData=USART_ReceiveData(USART1);
		Serial_RXflag=1;
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

