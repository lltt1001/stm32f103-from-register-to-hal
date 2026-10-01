#ifndef __INF_KEY_H__
#define __INF_KEY_H__


#include "dri_key.h"
#include "inf_led.h"
/**
 * @brief 初始化key
 * 
 */
void inf_key_init(void);


/**
 * @brief 中断服务程序
 * 
 */

 void EXTI4_IRQHandler(void);

#endif /* __INF_KEY_H__ */
