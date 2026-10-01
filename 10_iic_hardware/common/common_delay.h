#ifndef __COMMON_DELAY_H__
#define __COMMON_DELAY_H__

#include "stm32f10x.h"

/**
 * @brief 延迟函数
 * @param (uint16_t)时间长度,单位微秒
 */
void common_delay_us(uint16_t us);

/**
 * @brief 延迟函数
 * @param (uint16_t)时间长度,单位毫秒
 */
void common_delay_ms(uint16_t ms);

/**
 * @brief 延迟函数
 * @param (uint16_t)时间长度,单位秒
 */
void common_delay_s(uint16_t s);

#endif /* __COMMON_DELAY_H__ */
