#include "common_delay.h"

/**
 * @brief 延迟函数
 * @param 时间长度,单位毫秒
 */
void common_delay_us(uint16_t us)
{
    /* 定时器重装值 */
    SysTick->LOAD = 72 * us;
    /* 清除当前计数值 */
    SysTick->VAL = 0;
    /*设置内部时钟源为系统时钟（2位->1）,不需要中断（1位->0），并启动定时器(0位->1)*/
    SysTick->CTRL = 0x5;
    /*等待计数到0， 如果计数到0则16位会置为1*/
    while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0)
    {
    };
    /* 关闭定时器 */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
}

/**
 * @brief 延迟函数
 * @param (uint16_t)时间长度,单位毫秒
 */
void common_delay_ms(uint16_t ms)
{
    while (ms--)
    {
        common_delay_us(1000);
    }
}

/**
 * @brief 延迟函数
 * @param (uint16_t)时间长度,单位秒
 */
void common_delay_s(uint16_t s)
{
    while (s--)
    {
        common_delay_ms(1000);
    }
}
