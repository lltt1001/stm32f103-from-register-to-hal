#include "inf_key.h"
uint8_t key1_pressed = 0;
Led_Flow_Type flow_led_status = LED_FLOW_OFF;

/**
 * @brief 初始化key
 */
void inf_key_init(void)
{
    // 初始化key的驱动
    dri_key_init();
}

/**
 * @brief key1按下的处理程序
 */
void inf_key_key1pressd_handler(void)
{
    // 错误优先处理,排除不用处理按下的情况
    if (key1_pressed == 0)
    {
        return;
    }

    // 只要开始处理按下逻辑,则标志位就要恢复
    key1_pressed = 0;

    // 处理按下逻辑
    common_delay_ms(10);

    if ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
    {
        // 流水灯
        flow_led_status = flow_led_status == LED_FLOW_ON ? LED_FLOW_OFF : LED_FLOW_ON;
    }
}

/**
 * @brief 中断服务程序
 */
void EXTI9_5_IRQHandler(void)
{
    // 获取PF8引脚的值
    if ((GPIOF->IDR & GPIO_IDR_IDR8) == 0)
    {
        // 清除中断标志位
        EXTI->PR |= EXTI_PR_PR8;

        // 把标志位置1
        key1_pressed = 1;
    }
}
