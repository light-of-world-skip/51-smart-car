// motor.c
#include <reg52.h>
#include "config.h"
#include "motor.h"

sbit IN1 = P2^1;   // 左电机方向
sbit IN2 = P2^2;
sbit IN3 = P2^3;   // 右电机方向
sbit IN4 = P2^4;
sbit ENA = P2^0;   // 左电机 PWM
sbit ENB = P2^5;   // 右电机 PWM

static volatile unsigned char left_duty  = 0;
static volatile unsigned char right_duty = 0;
static unsigned char pwm_cnt = 0;

void Motor_Init(void) {
    IN1 = 0; IN2 = 0; IN3 = 0; IN4 = 0;
    ENA = 0; ENB = 0;
    left_duty = 0; right_duty = 0;
}

void Motor_SetSpeed(int left, int right) {
    // 左轮方向
    if (left >= 0) {
        IN1 = 1; IN2 = 0;
        left_duty = (unsigned char)left;
    } else {
        IN1 = 0; IN2 = 1;
        left_duty = (unsigned char)(-left);
    }

    // 右轮方向
    if (right >= 0) {
        IN3 = 1; IN4 = 0;
        right_duty = (unsigned char)right;
    } else {
        IN3 = 0; IN4 = 1;
        right_duty = (unsigned char)(-right);
    }

    if (left_duty  > 100) left_duty  = 100;
    if (right_duty > 100) right_duty = 100;
}

void Motor_Stop(void) {
    IN1 = 0; IN2 = 0; IN3 = 0; IN4 = 0;
    left_duty = 0; right_duty = 0;
}

void Motor_PWM_Tick(void) {
    if (++pwm_cnt >= PWM_PERIOD) pwm_cnt = 0;
    if (pwm_cnt < left_duty)  ENA = 1; else ENA = 0;
    if (pwm_cnt < right_duty) ENB = 1; else ENB = 0;
}