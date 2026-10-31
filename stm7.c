#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main(){
	OLED_Init();
	OLED_ShowChar(1,1,'A'); //4行16列 
	OLED_ShowString(1,3,"HelloStm32");
	OLED_ShowNum(2,1,12345,5);// >5 补0(1前)   <5 切除高位(1) 
	OLED_ShowSignedNum(2,7,12345,5);//带正负
	OLED_ShowHexNum(3,1,0xAA55,4);//16进制
	OLED_ShowBinNum(4,1,0xAA55,16);//2进制用16表示
	
	OLED_Clear();//全清  清除指定的在输入中加空格就行

	while(1){
	
	
	
	
	}
	
	
	
}
