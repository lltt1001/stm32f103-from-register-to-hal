#include "dri_tim4.h"

/**
 * @brief TIM4初始化
 * * PB6->TIM4-CH1
 */
void dri_tim4_init(void)
{
    // 1. 开启tim4和gpioa的时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. 配置GPIO的工作模式 MODE:00 CNF:01
    GPIOB->CRL &= ~GPIO_CRL_MODE6;
    GPIOB->CRL |= GPIO_CRL_CNF6_0;
    GPIOB->CRL &= ~GPIO_CRL_CNF6_1;

    // 3.定时器配置
    // 3.1 UDIS：禁止更新 - 不禁止(默认)
    // 3.2 URS：更新请求源 - 默认(默认)
    // 3.3 CMS[1:0]：选择中央对齐模式 - 边沿对齐(默认)
    // 3.4 DIR：方向 - 向上计数(默认)
    // 3.5 ARPE：自动重装载预装载允许位 -- 不允许(默认)
    // 3.6 配置预分频
    TIM4->PSC = 72 - 1;
    // 3.7 自动重装载寄存器
    TIM4->ARR = 65535;

    // 4. 输入捕获通道配置
    // 4.1 TI1S：TI1选择 -> TI1_CH1(默认)
    // 4.2 CC1S[1:0]：捕获/比较1选择 - 选择捕获 并且IC1和TI1直连
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    // 4.3 IC1PSC[1:0]：输入/捕获1预分频器 - 不分频(默认)
    // 4.4 IC1F[3:0]：输入捕获1滤波器-不滤波(默认)
    // 4.5 CC1E：输入/捕获1输出使能-使能
    TIM4->CCER |= TIM_CCER_CC1E;
    // 4.6 CC1P：输入/捕获1输出极性-配置为上升沿(默认)

    // 5. 开启中断
    // 5.1 开启通道1的输入捕获中断(方便我们检测到中断之后,把计数器置0)
    TIM4->DIER |= TIM_DIER_CC1IE;
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM4_IRQn, 7);
    NVIC_EnableIRQ(TIM4_IRQn);
}

/**
 * @brief 启动定时器4
 */
void dri_tim4_enable(void)
{
    TIM4->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 关闭定时器4
 */
void dri_tim4_disable(void)
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 测量pwm周期
 */
uint16_t dri_tim4_get_pwmcycle(void)
{
    // 捕获寄存器的值就是 两个上升沿之间的间隔(数量要+1 因为是从0开始数的)
    return TIM4->CCR1 + 1;
}

/**
 * @brief 测量频率
 */
float dri_tim4_get_pwmfreq(void)
{
    return 1000000.0 / (TIM4->CCR1 + 1);
}

/**
 * @brief 中断服务程序
 */
void TIM4_IRQHandler(void)
{
    // 判断是哪一个类型的中断产生
    if (TIM4->SR & TIM_SR_CC1IF)
    {
        // 说明此时已经捕获完成
        // 清标志位
        TIM4->SR &= ~TIM_SR_CC1IF;

        // 把计数器清0 重新计数
        TIM4->EGR |= TIM_EGR_UG;
    }
}
