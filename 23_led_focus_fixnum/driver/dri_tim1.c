#include "dri_tim1.h"

/**
 * @brief 初始化
 */
void dri_tim1_init(void)
{
    // 1. 开启时钟 GPIOA TIM1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. GPIO工作模式 MODE:11 CNF:10
    GPIOA->CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_CNF8_1);
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;

    // 3. 配置定时器的时基
    TIM1->PSC = 7199;
    TIM1->ARR = 4999;
    TIM1->RCR = 4;

    // 4. cc通道配置
    TIM1->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M;
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    TIM1->CCR1 = 2500;
    TIM1->CCER |= TIM_CCER_CC1P;
    TIM1->CCER |= TIM_CCER_CC1E;
    TIM1->BDTR |= TIM_BDTR_OSSI;

    // 补充: 高级定时器中,初始化完成后,需要手动的ug更新 才能让配置生效
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->SR &= ~TIM_SR_UIF;

    // 5. 中断
    TIM1->DIER |= TIM_DIER_UIE;
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(TIM1_UP_IRQn, 7);
    NVIC_EnableIRQ(TIM1_UP_IRQn);
}

/**
 * @brief tim1使能
 */
void dri_tim1_enable(void)
{
    TIM1->BDTR |= TIM_BDTR_MOE;
    TIM1->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief tim1关闭
 */
void dri_tim1_disable(void)
{
    TIM1->BDTR &= ~TIM_BDTR_MOE;
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 中断服务程序
 */
void TIM1_UP_IRQHandler(void)
{
    // printf("hello....\r\n");
    // 1. 清除标志位
    TIM1->SR &= ~TIM_SR_UIF;

    // 2. 关闭tim
    dri_tim1_disable();

    // 3. 设置通道1的空闲电平
    TIM1->CR2 &= ~TIM_CR2_OIS1;
}