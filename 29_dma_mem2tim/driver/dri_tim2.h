#ifndef __DRI_TIM2_H__
#define __DRI_TIM2_H__

#include <stm32f10x.h>
#include <stdio.h>
/**
 * @brief 定时器的初始化
 */
void dri_tim2_init(void);

/**
 * @brief 启动定时器呼吸使能
 */
void dri_tim2_enable(void);

#endif /* __DRI_TIM2_H__ */
