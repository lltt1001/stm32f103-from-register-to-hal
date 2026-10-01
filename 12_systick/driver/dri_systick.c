#include "dri_systick.h"

/**
 * @brief systick初始化
 */
void dri_systick_init(void)
{
    // 1. 设置systick重装载值(72MHZ->1ms  72000分频)
    SysTick->LOAD = (72000 - 1);

    // 2. 配置时钟源(AHB)
    // SysTick_CTRL_CLKSOURCE_Pos -> 2
    //(1ul << SysTick_CTRL_CLKSOURCE_Pos)  1<<2  => 0000100
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    // 3. 开启中断
    SysTick->CTRL |= SysTick_CTRL_TICKINT;

    // 4. 开启计数器使能
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
}

// 中断服务程序
void SysTick_Handler(void)
{
    static uint16_t count = 0;
    // 每1ms执行一次中断
    count++;
    if (count == 1000)
    {
        inf_led_toggle(LED1);
        count = 0;
    }
}