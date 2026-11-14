#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//控制I2C时钟线(SCL/SDA)电平
 void MyI2C_W_SCL(uint8_t BitValue){
	 GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)BitValue);
	 Delay_us(10);
}
 
 void MyI2C_W_SDA(uint8_t BitValue){                     
	 GPIO_WriteBit(GPIOB,GPIO_Pin_11,(BitAction)BitValue);
	 Delay_us(10);
}
    
uint8_t MyI2C_R_SDA(void){
	 uint8_t BitValue;
	 BitValue=GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);//读11控制SDA(SCL由主机控制)
	 Delay_us(10);
	 return BitValue;
}

void MyI2C_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_OD;//配合上拉电阻使用(向下拉才导通)
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Initstructure);
	
}

//起始条件
void MyI2C_Start(void){
	MyI2C_W_SCL(1);//释放总线(保证高阻态)
	MyI2C_W_SDA(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);

}

//终止条件
void MyI2C_Stop(void){
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

//发送一个字节
void MyI2C_SendByte(uint8_t Byte){
	uint8_t i;
	for(i=0;i<8;i++){
		//禁止的情况是SCL为1时改动SDA(所有改SDA在SCL前面)
		MyI2C_W_SDA(Byte & (0x80 >> i));//得最高位(依次向下一位)
	    MyI2C_W_SCL(1);
	    MyI2C_W_SCL(0);
	}
}

//接收一个字节
uint8_t MyI2C_ReceiveByte(void){
	uint8_t i;
	uint8_t Byte=0x00;
	MyI2C_W_SDA(1);//释放SDA,把主线让给从机
	for(i=0;i<8;i++){
	    MyI2C_W_SCL(1);
	if(MyI2C_R_SDA()==1){
		Byte |= ( 0x80 >> i );
	}
	    MyI2C_W_SCL(0);
	}
	return Byte;

}

//应答
void MyI2C_SendAck(uint8_t AckBit){
	
		MyI2C_W_SDA(AckBit);//传0(ACK)应答(收到) 1(NACK)(没收到)
	    MyI2C_W_SCL(1);
	    MyI2C_W_SCL(0);
	
}

//接收
uint8_t MyI2C_ReceiveAck(void){
	uint8_t AckBit;
	uint8_t Byte=0x00;
	MyI2C_W_SDA(1);//释放SDA
	MyI2C_W_SCL(1);
	AckBit=MyI2C_R_SDA();//读取应答位
	MyI2C_W_SCL(0);
	
	return AckBit;
}

