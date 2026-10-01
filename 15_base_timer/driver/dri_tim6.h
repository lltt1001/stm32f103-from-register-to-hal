#ifndef __DRI_TIM6_H__
#define __DRI_TIM6_H__

#include <stm32f10x.h>
#include "inf_led.h"
#include "stdio.h"

/**
 * @brief 初始化定时器6
 */
void dri_tim6_init(void);

/**
 * @brief 使能定时器6
 */
void dri_tim6_enable(void);

/**
 * @brief 关闭定时器6
 */
void dri_tim6_disable(void);

#endif /* __DRI_TIM6_H__ */
