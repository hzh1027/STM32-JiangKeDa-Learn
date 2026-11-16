#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"


uint8_t ID;
int16_t AX,AY,AZ,GX,GY,GZ;

//软件I2C读写MPU6050
int main()
{
	OLED_Init();
	MPU6050_Init();
	/*
	MPU6050_WriteReg(0x6B,0x00);//写之前要解除睡眠模式
	
	MPU6050_WriteReg(0x19,0xAA);//写入
	
	uint8_t ID=MPU6050_ReadReg(0x19);//读出
	
	OLED_ShowHexNum(1,1,ID,2);
	*/
	
	OLED_ShowString(1,1,"ID:");
	ID=MPU6050_GetID();
	OLED_ShowHexNum(1,4,ID,2);
	
    while(1){
		MPU6050_GetData(&AX,&AY,&AZ,&GX,&GY,&GZ);
		
		OLED_ShowSignedNum(2,1,AX,5);
		OLED_ShowSignedNum(3,1,AY,5);
		OLED_ShowSignedNum(4,1,AZ,5);
		OLED_ShowSignedNum(2,8,GX,5);
		OLED_ShowSignedNum(3,8,GY,5);
		OLED_ShowSignedNum(4,8,GZ,5);
		
		
		
    }
}

 