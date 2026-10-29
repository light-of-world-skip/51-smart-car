// remote.h
#ifndef __REMOTE_H__
#define __REMOTE_H__

void Remote_Init(void);
void Remote_Poll(void);   // 解析串口命令（含模式切换）
void Remote_Run(void);    // 遥控模式下执行当前动作

#endif