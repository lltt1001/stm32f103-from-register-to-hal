#ifndef __DRI_LED_H__
#define __DRI_LED_H__
#include <stm32f10x.h>

// 定义所有LED的 宏
#define LED1 GPIO_ODR_ODR0
#define LED2 GPIO_ODR_ODR1
#define LED3 GPIO_ODR_ODR8

/**
 * @brief LED Driver 初始化
 */
void dri_led_init(void);

#endif /* __DRI_LED_H__ */
