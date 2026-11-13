#include "stm32f10x.h"
// Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial4.h"
#include "LED.h"
#include "string.h"

//串口发送文本数据包
int main(void)
{
    OLED_Init();
    LED_Init();
    Serial4_Init();

    OLED_ShowString(1, 1, "TxPacket");
    OLED_ShowString(3, 1, "RxPacket");

    while (1){
        if (Serial4_RxFlag == 1) // 代表接收到数据包
        {
            OLED_ShowString(4, 1, "                ");
            OLED_ShowString(4, 1, Serial4_RxPacket);

            if (strcmp(Serial4_RxPacket, "LED_ON") == 0)
            {
                LED1_ON();
                Serial4_SendString("LED_ON_OK\r\n");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(2, 1, "LED_ON_OK");
            }
            else if (strcmp(Serial4_RxPacket, "LED_OFF") == 0)
            {
                LED1_OFF();
                Serial4_SendString("LED_OFF_OK\r\n");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(2, 1, "LED_OFF_OK");
            }
            else
            {
                Serial4_SendString("ERROR_COMMAND\r\n");
                OLED_ShowString(2, 1, "                ");
                OLED_ShowString(2, 1, "ERROR_COMMAND");
            }

            Serial4_RxFlag = 0;
        }
    }
}