#include "dri_systick.h"
// 定义一个全局的心跳值
static uint32_t systick_tick_ms = 0;

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
    systick_tick_ms++;
}

/**
 * @brief 封装一个函数,专门用来外部获取当前的心跳值
 */
uint32_t dri_systick_getTick(void)
{
    return systick_tick_ms;
}

/**
 * @brief 根据当前systick封装一个ms延迟函数
 */
void my_delay(uint32_t ms)
{
    // 1. 获取当前时间
    uint32_t current_time = dri_systick_getTick();

    // 2. 循环卡死等待
    while (dri_systick_getTick() - current_time < ms)
        ;
}
