#ifndef __DRI_UART_H__
#define __DRI_UART_H__

#include "stm32f10x.h"

/**
 * @brief uart1的初始化
 * PA9 : 复用推挽输出 MODE:11 CNF:10
 * PA10: 浮空输入 MODE:00 CNF:01
 */
void dri_uart_init(void);

/**
 * @brief uart1发送一个字节的数据
 */
void dri_uart_sendchar(uint8_t ch);

/**
 * @brief uart1接收一个字节的数据
 */
uint8_t dri_uart_receivechar(void);

/**
 * @brief uart1发送一个字符串的数据
 */
void dri_uart_sendstring(uint8_t data[], uint16_t size);

/**
 * @brief uart1接收一个字符串数据
 */
void dri_uart_receivestring(uint8_t data[], uint16_t *size);


#endif /* __DRI_UART_H__ */
