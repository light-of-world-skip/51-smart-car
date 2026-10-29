// track.c
#include <reg52.h>
#include "config.h"
#include "motor.h"
#include "sensor.h"
#include "control.h"
#include "track.h"
#include "timer.h"
#include "uart.h"

// 打开此宏会在串口输出传感器掩码，便于调试；
// 正式跑车请注释掉，否则 9600bps 发送约 10 字节需 10ms，会拖慢控制周期。
// #define TRACK_DEBUG

// 控制周期，单位：10ms tick（Timer0 每 10ms 让 g_tick_10ms 加 1）
// 1 = 10ms 一次控制；2 = 20ms。原 Delay_ms(5) 不精确，这里改用定时器节拍。
#define TRACK_PERIOD_TICK   1

static unsigned int last_ctrl_tick = 0;

void Track_Init(void) {
    Control_Reset();
    last_ctrl_tick = Timer_GetTick();
}

void Track_Run(void) {
    unsigned int now;
    unsigned char mask;
    int error;
    int output;
    int speedL;
    int speedR;

    // ---- 非阻塞节拍：没到控制周期就直接返回 ----
    // 主循环可以继续跑 Remote_Poll()，串口响应不会被 Delay 卡住
    now = Timer_GetTick();
    if ((unsigned int)(now - last_ctrl_tick) < TRACK_PERIOD_TICK) {
        return;
    }
    last_ctrl_tick = now;

    mask = Sensor_ReadMask();

#ifdef TRACK_DEBUG
    {
        const char hex[] = "0123456789ABCDEF";
        UART_SendString("Mask: ");
        UART_SendByte(hex[(mask >> 4) & 0x0F]);
        UART_SendByte(hex[mask & 0x0F]);
        UART_SendString("\r\n");
    }
#endif

    // ---- 全丢线：停车 + 复位 PD，避免恢复时微分项基于旧误差产生冲击 ----
    if (mask == 0) {
        Motor_Stop();
        Control_Reset();
        return;
    }

        // ---- 全压线（十字/横线）：保持直行通过，不停车 ----
    if (mask == 0x1B) {  // <--- 改为 0x1B（L2+L1+R1+R2）
        Motor_SetSpeed(BASE_DUTY, BASE_DUTY);
        Control_Reset();
        return;
    }

    error  = Sensor_CalcError(mask);
    output = Control_Update(error);

    speedL = BASE_DUTY - output;
    speedR = BASE_DUTY + output;

    Motor_SetSpeed(speedL, speedR);
}