#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyI2C.h"

//软件I2C与MPU6050(应答)
int main()
{
	OLED_Init();
	MyI2C_Init();
	
	MyI2C_Start();
	MyI2C_SendByte(0xD0);// 1101 000(MPU6050) 0(写)
	uint8_t Ack=MyI2C_ReceiveAck();//应答为 0 没有应答为 1
	MyI2C_Stop();
	
	OLED_ShowNum(1,1,Ack,3);

	
	
    while(1){
		
    }
}

 