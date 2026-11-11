#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "SERIAL2.h"

uint8_t RxData;

//串口的接受
int main(){
	OLED_Init();
	Serial_Init();
	/*
	Serial_SendByte(0x41);
	
	uint8_t MyArray[]={0x42,0x43,0x44,0x45};
	Serial_SendArray(MyArray,4);
	
	Serial_SendString("Hello stm32\r\n");//显示要开文本模式
	
	Serial_SendNumber(12345,5);
	
	printf("NUM=%d\r\n",666);
	
	char String[100];
	sprintf(String,"Num%d\r\n",666);
	Serial_SendString(String);
	
	Serial_Printf("Num%d\r\n",666); */

	while(1){
		//查询
		if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET){ //检查串口1的接受标志位
			//中断写法 if(Serial_GetRxFlag()==1);
			RxData=USART_ReceiveData(USART1);//读取收到的数据
			//中断写法 RxData=Serial_GetRxData();
			OLED_ShowHexNum(1,1,RxData,2);
		}
	
	
    }
}

