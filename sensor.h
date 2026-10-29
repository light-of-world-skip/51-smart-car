// sensor.h
#ifndef __SENSOR_H__
#define __SENSOR_H__

// 传感器位掩码
#define SENSOR_BIT_L2  0x01   // 最左
#define SENSOR_BIT_L1  0x02
#define SENSOR_BIT_M   0x04
#define SENSOR_BIT_R1  0x08
#define SENSOR_BIT_R2  0x10   // 最右

// 黑线对应的电平（TCRT5000 通常黑线 = 低电平）
#define SENSOR_ACTIVE  0

void Sensor_Init(void);
unsigned char Sensor_ReadMask(void);          // 返回5位掩码
int  Sensor_CalcError(unsigned char mask);    // 加权平均偏差

#endif