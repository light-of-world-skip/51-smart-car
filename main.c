#include <reg52.h>
#include "config.h"
#include "system.h"
#include "timer.h"
#include "motor.h"
#include "sensor.h"
#include "control.h"
#include "track.h"
#include "uart.h"
#include "remote.h"

sbit LED_MODE = P3^7;   // 模式指示：循迹亮，遥控灭

// 系统当前模式变量（在 system.h 中声明，这里定义）
volatile unsigned char g_mode = MODE_TRACK;

// 粗略延时，用于上电后等待 HC-05 稳定
void BT_Delay_ms(unsigned int ms) {
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 110; j++);
}

void main(void) {
    // 初始化顺序：先电机，防止上电乱转
    Motor_Init();
    Sensor_Init();
    Control_Reset();
    UART_Init();

    // HC-05 上电后稍等稳定即可，不再需要 ESP8266 那一套 AT 指令
    BT_Delay_ms(300);
    UART_ClearBuffer();

    Remote_Init();
    Timer_Init();
    Track_Init();

    // 默认进入循迹模式
    g_mode = MODE_TRACK;
    LED_MODE = 0;   // 亮灯表示循迹

    while (1) {
        // 随时处理蓝牙串口命令
        Remote_Poll();

        if (g_mode == MODE_TRACK) {
            LED_MODE = 0;
            Track_Run();
        } else {
            LED_MODE = 1;       // 灭灯表示遥控
            Remote_Run();
        }
    }
}