#ifndef __SERIAL4_H
#define __SERIAL4_H
#include <stdio.h>

// 外部声明：必须与 serial4.c 中的定义完全一致
extern uint8_t Serial4_TxPacket[];
extern char Serial4_RxPacket[]; 
extern uint8_t Serial4_RxFlag;

void Serial4_Init(void);
void Serial4_SendByte(uint8_t Byte);
void Serial4_SendArray(uint8_t *Array, uint16_t Length);
void Serial4_SendString(char* String);
void Serial4_SendNumber(uint32_t Number, uint8_t Length);
void Serial4_Printf(char* format, ...);

void Serial4_SendPacket(void);

// 中断写法
uint8_t Serial4_GetRxFlag(void);

#endif