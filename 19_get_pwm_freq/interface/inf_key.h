#ifndef __INF_KEY_H__
#define __INF_KEY_H__

#include "dri_key.h"
#include "inf_led.h"
#include "common_delay.h"

typedef enum
{
    LED_FLOW_OFF = 0,
    LED_FLOW_ON
} Led_Flow_Type;

extern Led_Flow_Type flow_led_status;
extern uint8_t key1_pressed;

/**
 * @brief 初始化key
 */
void inf_key_init(void);

/**
 * @brief key1按下的处理程序
 */
void inf_key_key1pressd_handler(void);

#endif /* __INF_KEY_H__ */
