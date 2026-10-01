#include "dri_tim5.h"

/**
 * @brief 初始化
 */
void dri_tim5_init(void)
{
    // 1. 开启tim5和gpioa的时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. 配置GPIO的工作模式 MODE:11 CNF:10
    GPIOA->CRL |= GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_CNF1_1;
    GPIOA->CRL &= ~GPIO_CRL_CNF1_0;

    // 3. 定时器配置
    // 3.1 CMS[1:0]：选择中央对齐模式->000 边沿对齐模式
    TIM5->CR1 &= ~TIM_CR1_CMS;
    // 3.2 DIR：方向 (Direction)   0->向上计数
    TIM5->CR1 &= ~TIM_CR1_DIR;
    // 3.3 ARPE：自动重装载预装载允许位 0->没有缓冲
    TIM5->CR1 &= ~TIM_CR1_ARPE;

    // 3.4 捕获比较寄存器配置为输出模式
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;

    // 3.5 OC2PE：输出比较1预装载使能 0-->禁止
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2PE;

    // 3.6 OC2M[2:0]：输出比较1模式  110->PWM1模式
    TIM5->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0;

    // 3.7 CC2E：输入/捕获2输出使能
    TIM5->CCER |= TIM_CCER_CC2E;

    // 3.8 CC2P：输入/捕获2输出极性 配置为低电平有效(LED低电平点亮)
    TIM5->CCER |= TIM_CCER_CC2P;

    // 3.9 预分频器配置
    TIM5->PSC = 72 - 1;

    // 3.10 自动重装载寄存器
    TIM5->ARR = 100 - 1;
}

/**
 * @brief 使能定时器
 */
void dri_tim5_enable(void)
{
    TIM5->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 关闭定时器
 */
void dri_tim5_disable(void)
{
    TIM5->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 设置cc2的比较寄存器的值(控制占空比)
 */
void dri_tim5_set_compare(uint16_t value)
{
    TIM5->CCR2 = value;
}
