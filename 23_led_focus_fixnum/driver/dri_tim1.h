#ifndef __DRI_TIM1_H__
#define __DRI_TIM1_H__

#include <stm32f10x.h>
#include <stdio.h>

/**
 * @brief 初始化
 */
void dri_tim1_init(void);

/**
 * @brief tim1使能
 */
void dri_tim1_enable(void);

/**
 * @brief tim1关闭
 */
void dri_tim1_disable(void);

#endif /* __DRI_TIM1_H__ */
