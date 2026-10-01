#include "inf_led.h"
static uint16_t leds[3] = {LED1, LED2, LED3};

/**
 * @brief 初始化
 */
void inf_led_init(void)
{
    // 1. 初始化驱动
    dri_led_init();

    // 2. 关闭所有灯
    inf_led_close_all();
}

/**
 * @brief 开某个灯
 */
void inf_led_open(uint16_t led)
{
    GPIOA->ODR &= ~led;
}

/**
 * @brief 关某个灯
 */
void inf_led_close(uint16_t led)
{
    GPIOA->ODR |= led;
}

/**
 * @brief 关闭所有灯
 */
void inf_led_close_all(void)
{
    // 把所有的LED放入数组
    uint16_t leds[3] = {LED1, LED2, LED3};
    // 遍历
    for (uint16_t i = 0; i < 3; i++)
    {
        GPIOA->ODR |= leds[i];
    }
}

/**
 * @brief 开启所有灯
 */
void inf_led_open_all(void)
{
    // 把所有的LED放入数组
    uint16_t leds[3] = {LED1, LED2, LED3};
    // 遍历
    for (uint16_t i = 0; i < 3; i++)
    {
        GPIOA->ODR &= ~leds[i];
    }
}

/**
 * @brief 切换某个灯的状态
 */
void inf_led_toggle(uint16_t led)
{
    // 获取当前led的状态
    if ((GPIOA->IDR & led) == 0)
    {
        GPIOA->ODR |= led;
    }
    else
    {
        GPIOA->ODR &= ~led;
    }
}

/**
 * @brief 流水灯
 */
void inf_led_flow(void)
{
    for (uint16_t i = 0; i < 3; i++)
    {
        inf_led_open(leds[i]);
        // common_delay_ms(500);
        uint16_t count = 500;
        while (count > 0)
        {
            count--;
            common_delay_ms(1);
            // 只要发现按键按下 就立马退出流水灯,执行后续判断代码
            if (key1_pressed == 1)
            {
                // 关闭所有灯
                inf_led_close_all();
                return;
            }
        }

        inf_led_close(leds[i]);
    }
}

/**
 * @brief 每1s闪烁一次led1
 */
void inf_led1_focus(void)
{
    static uint32_t lastTick = 0;
    uint32_t current_time = dri_systick_getTick();
    if (current_time - lastTick >= 1000)
    {
        inf_led_toggle(LED1);
        lastTick = current_time;
    }
}
