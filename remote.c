// remote.c
#include <reg52.h>
#include "config.h"
#include "system.h"
#include "motor.h"
#include "control.h"
#include "uart.h"
#include "timer.h"
#include "remote.h"

static unsigned char remote_cmd = 0;
static unsigned int  remote_last_tick = 0;

void Remote_Init(void) {
    remote_cmd = 0;
    remote_last_tick = 0;
}

void Remote_Poll(void) {
    unsigned char b;
    while (UART_ReadByte(&b)) {
        switch (b) {
            case 'T':
                Motor_Stop();
                Control_Reset();
                g_mode = MODE_TRACK;
                remote_cmd = 0;
                break;

            case 'F': case 'B': case 'L': case 'R':
            case 'G': case 'I': case 'H': case 'J':
            case 'S':
                g_mode = MODE_REMOTE;
                remote_cmd = b;
                remote_last_tick = Timer_GetTick(); // 每次收到指令都刷新时间
                break;

            default:
                break;
        }
    }
}

void Remote_Run(void) {
    unsigned int now = Timer_GetTick();

    // 【核心】失联保护：如果 300ms 内没收到新指令，自动停车
    // 这就是为什么你松手后车会停下来的原因
    if ((unsigned int)(now - remote_last_tick) > REMOTE_TIMEOUT) {
        remote_cmd = 0; 
        Motor_Stop();
        return;
    }

    switch (remote_cmd) {
        // --- 直行 ---
        case 'F': Motor_SetSpeed(REMOTE_SPEED, REMOTE_SPEED); break;
        case 'B': Motor_SetSpeed(-REMOTE_SPEED, -REMOTE_SPEED); break;
        
        // --- 单按左/右：偏转力度调大 ---
        // 原来是 0 和 1/2，现在改成：内侧轮稍微反转，外侧轮全速
        // 这样转弯更利索，但因为靠超时停车，不会直接甩出 90 度
        case 'L': Motor_SetSpeed(-REMOTE_SPEED / 3, REMOTE_SPEED); break; 
        case 'R': Motor_SetSpeed(REMOTE_SPEED, -REMOTE_SPEED / 3); break;
        
        // --- 组合键（边走边转）：内侧轮减速至 1/4 ---
        case 'G': Motor_SetSpeed(REMOTE_SPEED / 4, REMOTE_SPEED); break;     // 前左
        case 'I': Motor_SetSpeed(REMOTE_SPEED, REMOTE_SPEED / 4); break;     // 前右
        case 'H': Motor_SetSpeed(-REMOTE_SPEED / 4, -REMOTE_SPEED); break;   // 后左
        case 'J': Motor_SetSpeed(-REMOTE_SPEED, -REMOTE_SPEED / 4); break;   // 后右

        // --- 停止 ---
        case 'S':
        default:  Motor_Stop(); break;
    }
}