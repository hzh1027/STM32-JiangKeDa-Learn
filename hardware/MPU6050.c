#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050_REG.h"

#define MPU6050_ADDERESS      0xD0


//软件I2C读写MPU6050
//向MPU6050的特定寄存器写数据的函数
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data){
	MyI2C_Start();//发起I2C起始信号
	MyI2C_SendByte(MPU6050_ADDERESS);//发送设备地址
	MyI2C_ReceiveAck();//应答
	MyI2C_SendByte(RegAddress);//要写入的地址
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);//发送数据到指定寄存器
	MyI2C_ReceiveAck();
	MyI2C_Stop();
	
}

//读取MPU6050中一个字节数据
uint8_t MPU6050_ReadReg(uint8_t RegAddress){
	uint8_t Data;
	MyI2C_Start();
	MyI2C_SendByte(MPU6050_ADDERESS);//发送设备写地址(0xD0)
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);//发送想读取目的寄存器地址
	MyI2C_ReceiveAck();
	
	MyI2C_Start();//重复起始信号(从写模式切换到读模式)
	MyI2C_SendByte(MPU6050_ADDERESS | 0x01);//再次发送设备地址(0x01 读)
	MyI2C_ReceiveAck();
	Data=MyI2C_ReceiveByte();
	MyI2C_SendAck(1);//停止接收
	MyI2C_Stop();
	
	return Data;
}

//获取芯片ID号
uint8_t MPU6050_GetID(void){
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}


//初始化MPU6050六轴传感器
void MPU6050_Init(void){
	MyI2C_Init();
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1,0x01);
	//     0        0        0       0         0                 0 0 1        
 	//  设备不复位  解除睡眠   不用循环   无关   温度传感器不失能    内部时钟(x轴陀螺仪时钟) 
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2,0x00);//全失能
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV,0x09);//采样率分频(采样频率=陀螺仪输出频率/1+9 =100Hz)
	MPU6050_WriteReg(MPU6050_CONFIG,0x06);//配置寄存器(低频通带,5Hz,滤噪)
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG,0x18);//陀螺仪配置(满量程,+-2000)
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG,0x18);//加速度计配置(+-16)
}

//获取数据寄存器
void MPU6050_GetData(int16_t *AccX,int16_t *AccY,int16_t *AccZ,
	                 int16_t *GyroX,int16_t *GyroY,int16_t *GyroZ){
	
    uint16_t DataH, DataL;
    // 加速度X轴16位数据，用指针返回数据
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (DataH << 8) | DataL;

    // 加速度Y轴
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (DataH << 8) | DataL;

    // 加速度Z轴
    DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (DataH << 8) | DataL;

    // 陀螺仪X轴
    DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (DataH << 8) | DataL;

    // 陀螺仪Y轴
    DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (DataH << 8) | DataL;

    // 陀螺仪Z轴
    DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (DataH << 8) | DataL;
		 	 
}
