#ifndef __DRI_IIC_H__
#define __DRI_IIC_H__
#include <stm32f10x.h>
#include "common_delay.h"

#define ACK 0
#define NACK 1

/**
 * @brief 初始化
 */
void dri_iic_init(void);

/**
 * @brief 发送起始信号
 */
uint8_t dri_iic_start(void);

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
 * @brief 接收应答(不需要这个函数了,我们在状态寄存器判断的时候,人家已经判断好是否有ack应答了)
 */
// uint8_t dri_iic_receive_ack(void);

/**
 * @brief 发送地址
 */
uint8_t dri_iic_sendaddr(uint8_t addr);

/**
 *  @brief 发送数据
 */
uint8_t dri_iic_sendbyte(uint8_t byte);

/**
 * @brief 接收数据
 */
uint8_t dri_iic_receivebyte(uint8_t *byte);

#endif /* __DRI_IIC_H__ */
