#ifndef __DRI_TIM4_H__
#define __DRI_TIM4_H__

#include "stm32f10x.h"
#include "stdio.h"

/**
 * @brief TIM4初始化
 * PB6->TIM4-CH1
 */
void dri_tim4_init(void);

/**
 * @brief 启动定时器4
 */
void dri_tim4_enable(void);

/**
 * @brief 关闭定时器4
 */
void dri_tim4_disable(void);

/**
 * @brief 测量pwm周期
 */
uint16_t dri_tim4_get_pwmcycle(void);

/**
 * @brief 测量频率
 */
float dri_tim4_get_pwmfreq(void);

/**
 * @brief 测量占空比
 */
float dri_tim4_get_dutycycle(void);
#endif /* __DRI_TIM4_H__ */
