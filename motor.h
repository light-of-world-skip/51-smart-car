// motor.h
#ifndef __MOTOR_H__
#define __MOTOR_H__

void Motor_Init(void);
void Motor_SetSpeed(int left, int right);   // -100~100，负值反转
void Motor_Stop(void);
void Motor_PWM_Tick(void);                  // Timer0 ISR 调用

#endif