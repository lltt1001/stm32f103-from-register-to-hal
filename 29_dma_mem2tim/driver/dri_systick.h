#ifndef __DRI_SYSTICK_H__
#define __DRI_SYSTICK_H__

#include "stm32f10x.h"
#include "inf_led.h"

/**
 * @brief systick初始化
 */
void dri_systick_init(void);

/**
 * @brief 封装一个函数,专门用来外部获取当前的心跳值
 */
uint32_t dri_systick_getTick(void);

/**
 * @brief 根据当前systick封装一个ms延迟函数
 */
void my_delay(uint32_t ms);

#endif /* __DRI_SYSTICK_H__ */
