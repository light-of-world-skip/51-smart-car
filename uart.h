// uart.h
#ifndef __UART_H__
#define __UART_H__

void UART_Init(void);
void UART_SendByte(unsigned char b);
void UART_SendString(const char *s);
unsigned char UART_ReadByte(unsigned char *b);   // 读到返回1，空返回0
void UART_ClearBuffer(void);
#endif