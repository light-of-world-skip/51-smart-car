// uart.c
#include <reg52.h>
#include "uart.h"

#define RX_BUF_SIZE  16

static volatile unsigned char rx_buf[RX_BUF_SIZE];
static volatile unsigned char rx_head = 0;
static volatile unsigned char rx_tail = 0;

void UART_Init(void) {
    SCON = 0x50;        // 模式1，8位UART，REN=1
    TMOD &= 0x0F;
    TMOD |= 0x20;       // Timer1 模式2（8位自动重载）
    TH1 = 0xFD;         // 9600 @ 11.0592MHz
    TL1 = 0xFD;
    TR1 = 1;
    ES  = 1;
    EA  = 1;
}

void UART_ISR(void) interrupt 4 {
    unsigned char next;
    if (RI) {
        RI = 0;
        // 计算下一个写入位置
        next = (rx_head + 1) & (RX_BUF_SIZE - 1);
        // 如果缓冲区没满，才存入数据；满了就丢弃（防止指针错乱卡死）
        if (next != rx_tail) {
            rx_buf[rx_head] = SBUF;
            rx_head = next;
        }
    }
    if (TI) {
        TI = 0;
    }
}
void UART_SendByte(unsigned char b) {
    unsigned char es_save = ES;
    ES = 0;
    TI = 0;
    SBUF = b;
    while (!TI);
    TI = 0;
    ES = es_save;
}
void UART_SendString(const char *s) {
    while (*s) {
        UART_SendByte(*s++);
    }
}

unsigned char UART_ReadByte(unsigned char *b) {
    if (rx_head == rx_tail) return 0;
    *b = rx_buf[rx_tail];
    rx_tail = (rx_tail + 1) & (RX_BUF_SIZE - 1);
    return 1;
}
void UART_ClearBuffer(void) {
    ES = 0;         // 关中断
    rx_head = 0;    // 头指针清零
    rx_tail = 0;    // 尾指针清零
    ES = 1;         // 开中断
}