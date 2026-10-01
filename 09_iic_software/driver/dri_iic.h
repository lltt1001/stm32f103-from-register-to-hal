#ifndef __DRI_IIC_H__
#define __DRI_IIC_H__
#include <stm32f10x.h>
#include "common_delay.h"

// 控制宏
#define SCL_HIGH (GPIOB->ODR |= GPIO_ODR_ODR6)
#define SCL_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR6)

#define SDA_HIGH (GPIOB->ODR |= GPIO_ODR_ODR7)
#define SDA_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR7)

#define READ_SDA (GPIOB->IDR & GPIO_IDR_IDR7)

#define IIC_DELAY (common_delay_ms(1))

#define ACK 0
#define NACK 1

/**
 * @brief 初始化
 */
void dri_iic_init(void);

/**
 * @brief 发送起始信号
 */
void dri_iic_start(void);

/**
 * @brief 发送停止信号
 */
void dri_iic_stop(void);

/**
 * @brief 发送应答信号
 */
void dri_iic_ack(void);

/**
 * @brief 发送非应答信号
 */
void dri_iic_nack(void);

/**
 * @brief 接收应答
 */
uint8_t dri_iic_receive_ack(void);

/**
 * @brief 发送数据
 */
void dri_iic_sendbyte(uint8_t byte);

/**
 * @brief 接收数据
 */
uint8_t dri_iic_receivebyte(void);

#endif /* __DRI_IIC_H__ */
