#include "dri_tim6.h"

/**
 * @brief 初始化定时器6
 */
void dri_tim6_init(void)
{
    // 1. 开启tim6时钟使能
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    // 2. 定时器的配置
    // 2.1 URS：更新请求源  0->所有请求都可以  1->只有溢出
    TIM6->CR1 &= ~TIM_CR1_URS;
    // 2.2 UDIS：禁止更新 0->请求源可以产生更新时间  1->不会产生更新时间
    TIM6->CR1 &= ~TIM_CR1_UDIS;
    // 2.3 ARPE：自动重装载预装载使能 0->更新预装载值的时候直接交给影子当次生效
    TIM6->CR1 |= TIM_CR1_ARPE;
    // 2.4 开启更新中断
    TIM6->DIER |= TIM_DIER_UIE;
    // 2.5 预分频
    TIM6->PSC = (72 - 1);
    // 2.6 自动重装载值
    TIM6->ARR = (1000 - 1);

    // 3. 中断的配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM6_IRQn, 7);
    NVIC_EnableIRQ(TIM6_IRQn);
}

/**
 * @brief 使能定时器6
 */
void dri_tim6_enable(void)
{
    // 清掉计数器的值,重新开始
    TIM6->CNT = 0;
    // 使能
    TIM6->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 关闭定时器6
 */
void dri_tim6_disable(void)
{
    TIM6->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 中断服务程序
 */
void TIM6_IRQHandler(void)
{
    // 基本定时器只有一种中断情况,所以不需要判断
    static uint16_t count = 0;

    // 清除标志位
    TIM6->SR &= ~TIM_SR_UIF;

    count++;
    if (count == 1000)
    {
        printf("test1....\r\n");
        inf_led_toggle(LED1);
        count = 0;
    }
}