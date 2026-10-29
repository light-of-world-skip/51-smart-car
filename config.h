#ifndef __CONFIG_H__
#define __CONFIG_H__

// ============ 循迹参数 ============
#define BASE_DUTY       45      // 循迹基础占空比（0~100）
#define KP              6       // 比例系数
#define KD              15      // 微分系数
#define OUTPUT_MAX      80      // PD输出限幅

// ============ 遥控参数 ============
#define REMOTE_SPEED    60      // 遥控模式电机速度
#define REMOTE_TIMEOUT  30      // 失联超时（单位：10ms tick，30 = 300ms）

// ============ PWM ============
#define PWM_PERIOD      100     // 100us * 100 = 10ms，100Hz

// ============ 串口波特率 ============
// 11.0592MHz + Timer1模式2 → 9600bps，误差0%
// TH1 = 0xFD

#endif