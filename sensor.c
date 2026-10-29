// sensor.c
#include <reg52.h>
#include "sensor.h"

sbit S_L2 = P1^0;
sbit S_L1 = P1^1;
// sbit S_M  = P1^2; // 已去掉
sbit S_R1 = P1^3;
sbit S_R2 = P1^4;

void Sensor_Init(void) {
    S_L2 = 1; S_L1 = 1; S_R1 = 1; S_R2 = 1;
}

unsigned char Sensor_ReadMask(void) {
    unsigned char mask = 0;
    if (S_L2 == SENSOR_ACTIVE) mask |= SENSOR_BIT_L2;
    if (S_L1 == SENSOR_ACTIVE) mask |= SENSOR_BIT_L1;
    // 中间 M 的掩码不再读取，永远为0
    if (S_R1 == SENSOR_ACTIVE) mask |= SENSOR_BIT_R1;
    if (S_R2 == SENSOR_ACTIVE) mask |= SENSOR_BIT_R2;
    return mask;
}

int Sensor_CalcError(unsigned char mask) {
    int sum = 0;
    unsigned char count = 0;

    // 外围传感器离得远，权重给大一点（比如 -3 和 +3）
    // 内侧传感器紧贴中心，权重给小一点（-1 和 +1）
    if (mask & SENSOR_BIT_L2) { sum -= 3; count++; }
    if (mask & SENSOR_BIT_L1) { sum -= 1; count++; }
    if (mask & SENSOR_BIT_R1) { sum += 1; count++; }
    if (mask & SENSOR_BIT_R2) { sum += 3; count++; }

    if (count == 0) return 0;
    
    // 乘以 10 放大误差，方便 PD 控制器计算
    return (sum * 10) / count;
}