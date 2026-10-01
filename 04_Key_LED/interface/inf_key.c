#include "inf_key.h"
#include "dri_key.h"

/**
 * @brief 初始化key
 *
 */
void inf_key_init()
{
   dri_Key_Init();
}

/**
 * @brief 中断服务程序
 *
 */

void EXTI4_IRQHandler(void)
{
   // 清除中断挂起位
   EXTI->PR = EXTI_PR_PR4;
   inf_led_toggle(LED1);
}
