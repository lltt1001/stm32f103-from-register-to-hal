#ifndef __DRI_TIM3_H__
#define __DRI_TIM3_H__

#include <stm32f10x.h>

/**
 * @brief 初始化
 */
void dri_tim3_init(void);

/**
 * @brief 使能定时器
 */
void dri_tim3_enable(void);

/**
 * @brief 关闭定时器
 */
void dri_tim3_disable(void);

/**
 * @brief 设置cc2的比较寄存器的值(控制占空比)
 */
void dri_tim3_set_compare(uint16_t value);

#endif /* __DRI_TIM3_H__ */

