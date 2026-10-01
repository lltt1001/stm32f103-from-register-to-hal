#include "dri_tim3.h"

/**
 * @brief 初始化
 */
void dri_tim3_init(void)
{
    // 1.开启TIM3和GPIOB，AFIO的时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 配置PB5工作模式 MODE:11 CNF:10
    GPIOB->CRL |= GPIO_CRL_MODE5;
    GPIOB->CRL |= GPIO_CRL_CNF5_1;
    GPIOB->CRL &= ~GPIO_CRL_CNF5_0;

    /*TIM3 部分重映射：将TIM3_CH2从默认引脚PA7重映射到PB5
    AFIO->MAPR bit[11:10] = TIM3_REMAP[1:0]
    00 = 不重映射(默认) -> TIM3_CH2在PA7
    10 = 部分重映射       -> TIM3_CH2在PB5  ← 我们选这个
    11 = 完全重映射       -> TIM3_CH2在PC7
    */

    //AFIO->MAPR 寄存器中只有 TIM1、TIM2、TIM3、TIM4 的重映射位
    //TIM5 的四个通道永久固定在 PA0~PA3，无法搬走
    //AFIO->MAPR |= (0b10 << 10);
    AFIO->MAPR |= AFIO_MAPR_TIM3_REMAP_PARTIALREMAP;


    // 3. 定时器配置
    // 3.1 CMS[1:0]：选择中央对齐模式->000 边沿对齐模式
    TIM3->CR1 &= ~TIM_CR1_CMS;
    // 3.2 DIR：方向 (Direction)   0->向上计数
    TIM3->CR1 &= ~TIM_CR1_DIR;
    // 3.3 ARPE：自动重装载预装载允许位 0->没有缓冲
    TIM3->CR1 &= ~TIM_CR1_ARPE;

    // 3.4 捕获比较寄存器配置为输出模式
    TIM3->CCMR1 &= ~TIM_CCMR1_CC2S;

    // 3.5 OC2PE：输出比较1预装载使能 0-->禁止
    TIM3->CCMR1 &= ~TIM_CCMR1_OC2PE;

    // 3.6 OC2M[2:0]：输出比较1模式  110->PWM1模式
    TIM3->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
    TIM3->CCMR1 &= ~TIM_CCMR1_OC2M_0;

    // 3.7 CC2E：输入/捕获2输出使能
    TIM3->CCER |= TIM_CCER_CC2E;

    // 3.8 CC2P：输入/捕获2输出极性 配置为低电平有效(LED低电平点亮)
    TIM3->CCER |= TIM_CCER_CC2P;

    // 3.9 预分频器配置
    TIM3->PSC = 72 - 1;

    // 3.10 自动重装载寄存器
    TIM3->ARR = 1000 - 1;
}

/**
 * @brief 使能定时器
 */
void dri_tim3_enable(void)
{

    TIM3->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief 关闭定时器
 */
void dri_tim3_disable(void)
{

    TIM3->CR1 &= ~TIM_CR1_CEN;
}

/**
 * @brief 设置cc2的比较寄存器的值(控制占空比)
 */
void dri_tim3_set_compare(uint16_t value)
{

    TIM3->CCR2 = value;
}
