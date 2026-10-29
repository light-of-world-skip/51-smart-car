// control.h
#ifndef __CONTROL_H__
#define __CONTROL_H__

void Control_Reset(void);
int  Control_Update(int error);   // 输入偏差，返回 PD 输出

#endif