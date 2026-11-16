#include "stm32f10x.h"                  // Device header
#include "MPU6050_REG.h"

#define MPU6050_ADDRESS      0xD0

//解决死循环
void MPU6050_WaitEvent(I2C_TypeDef* I2Cx,uint32_t I2C_EVENT){
	uint32_t Timeout;
	Timeout=10000;

	while(I2C_CheckEvent(I2Cx,I2C_EVENT)!=SUCCESS){
		Timeout --;
		if(Timeout==0){
			break;
		}
	}
}

//硬件I2C读写MPU6050
//向MPU6050的特定寄存器写数据的函数
void MPU6050_WriteReg(uint8_t RegAddress,uint8_t Data){
	
	I2C_GenerateSTART(I2C2,ENABLE);//开始通信
    MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT);
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);//发送MPU6050地址
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	
	I2C_SendData(I2C2,RegAddress);//发送寄存器地址
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_SendData(I2C2,Data);//发送数据
	MPU6050_WaitEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	
	I2C_GenerateSTOP(I2C2,ENABLE);//停止
}

//读取MPU6050中一个字节数据
uint8_t MPU6050_ReadReg(uint8_t RegAddress){
	uint8_t Data;
//	MyI2C_Start();
//	MyI2C_SendByte(MPU6050_ADDERESS);//发送设备写地址(0xD0)
//	MyI2C_ReceiveAck();
//	MyI2C_SendByte(RegAddress);//发送想读取目的寄存器地址
//	MyI2C_ReceiveAck();
//	
//	MyI2C_Start();//重复起始信号(从写模式切换到读模式)
//	MyI2C_SendByte(MPU6050_ADDERESS | 0x01);//再次发送设备地址(0x01 读)
//	MyI2C_ReceiveAck();
//	Data=MyI2C_ReceiveByte();
//	MyI2C_SendAck(1);//停止接收
//	MyI2C_Stop();
	
    
	I2C_GenerateSTART(I2C2,ENABLE);//开始通信
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);//阻塞等待
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Transmitter);//发送MPU6050地址
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
	
	I2C_SendData(I2C2,RegAddress);//发送寄存器地址
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);
	
	I2C_GenerateSTART(I2C2,ENABLE);//重复起始信号
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
	
	I2C_Send7bitAddress(I2C2,MPU6050_ADDRESS,I2C_Direction_Receiver);//发送设备地址
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)!=SUCCESS);//RECEIVER
	
	I2C_AcknowledgeConfig(I2C2,DISABLE);//响应提前至1(它会等数据读完至)
	I2C_GenerateSTOP(I2C2,ENABLE);//准备停止信号
	
	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED)!=SUCCESS);//等待数据接收完成
	Data=I2C_ReceiveData(I2C2);
	
	I2C_AcknowledgeConfig(I2C2,ENABLE);
	
	
	return Data;
}

//获取芯片ID号
uint8_t MPU6050_GetID(void){
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}


//初始化MPU6050六轴传感器
void MPU6050_Init(void){

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_AF_OD;//复用开漏
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed=50000;//50KHz(0-100-400)
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;//只有 >100KHz 有效 低电平时间是高电平两倍
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;//7位地址格式
	I2C_InitStructure.I2C_OwnAddress1=0x00;//主机地址,基本没用
	
	I2C_Init(I2C2,&I2C_InitStructure);
	I2C_Cmd(I2C2,ENABLE);
	
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
