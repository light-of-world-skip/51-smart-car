// timer.c
#include <reg52.h>
#include "config.h"
#include "timer.h"
#include "motor.h"

static unsigned char pwm_div = 0;
static volatile unsigned int g_tick_10ms = 0;

void Timer_Init(void) {
    TMOD &= 0xF0;
    TMOD |= 0x01;       // Timer0 模式1（16位）
    TH0 = 0xFF;         // 11.0592MHz，100us
    TL0 = 0xA4;
    ET0 = 1;
    EA  = 1;
    TR0 = 1;
}

void Timer0_ISR(void) interrupt 1 {
    TH0 = 0xFF;
    TL0 = 0xA4;

    Motor_PWM_Tick();   // 每 100us 更新一次 PWM 电平

    if (++pwm_div >= PWM_PERIOD) {
        pwm_div = 0;
        g_tick_10ms++;  // 每 10ms 递增一次
    }
}

unsigned int Timer_GetTick(void) {
    unsigned int t;
    unsigned char ea_save = EA;
    EA = 0;
    t = g_tick_10ms;
    EA = ea_save;
    return t;
}