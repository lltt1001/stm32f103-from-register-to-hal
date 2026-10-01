#ifndef __INF_LED_H__
#define __INF_LED_H__

#include "dri_led.h"
#include "inf_key.h"

/**
 * @brief 初始化
 */
void inf_led_init(void);

/**
 * @brief 开某个灯
 */
void inf_led_open(uint16_t led);

/**
 * @brief 关某个灯
 */
void inf_led_close(uint16_t led);

/**
 * @brief 关闭所有灯
 */
void inf_led_close_all(void);

/**
 * @brief 开启所有灯
 */
void inf_led_open_all(void);

/**
 * @brief 切换某个灯的状态
 */
void inf_led_toggle(uint16_t led);

/**
 * @brief 流水灯
 */
void inf_led_flow(void);
#endif /* __INF_LED_H__ */
