#include "dri_tim2.h"
#define ARRAY_LEN 256
uint16_t pwm_ccr_arr[ARRAY_LEN] = {0};

/**
 * @brief 静态 生成数组的值 0-999
 */
static void dri_tim2_genrate_arr(void)
{
    for (uint16_t i = 0; i < ARRAY_LEN; i++)
    {
        if (i < ARRAY_LEN / 2)
        {
            pwm_ccr_arr[i] = i * 2000 / ARRAY_LEN;
        }
        else
        {
            pwm_ccr_arr[i] = (ARRAY_LEN - i) * 2000 / ARRAY_LEN;
        }
    }
}

/**
 * @brief 定时器的初始化
 */
void dri_tim2_init(void)
{
    // 1. 生成PWM所需要的比较寄存器的值,然后放在数组中
    dri_tim2_genrate_arr();
    /* for (uint16_t i = 0; i < ARRAY_LEN; i++)
    {
        printf("%d\r\n", pwm_ccr_arr[i]);
    } */

    // 1. 时钟 DMA GPIO TIM2
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 1. GPIO MODE:11 CNF:10
    GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_CNF0_1);
    GPIOA->CRL &= ~GPIO_CRL_CNF0_0;

    // 2. 配置定时器的时基单元 100hz
    TIM2->PSC = 720 - 1;
    TIM2->ARR = 1000 - 1;

    // 3. 配置cc通道
    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M;
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    TIM2->CCER |= TIM_CCER_CC1E;
    TIM2->CCER |= TIM_CCER_CC1P;
    TIM2->CCR1 = 0;

    // 4. 配置DMA
    DMA1_Channel2->CCR |= (DMA_CCR2_MSIZE_0 | DMA_CCR2_PSIZE_0 | DMA_CCR2_MINC | DMA_CCR2_CIRC | DMA_CCR2_DIR);
    DMA1_Channel2->CNDTR = ARRAY_LEN;
    DMA1_Channel2->CPAR = (uint32_t)&TIM2->CCR1;
    DMA1_Channel2->CMAR = (uint32_t)pwm_ccr_arr;
}

/**
 * @brief 启动定时器呼吸使能
 */
void dri_tim2_enable(void)
{
    TIM2->DIER |= TIM_DIER_UDE;
    DMA1_Channel2->CCR |= DMA_CCR2_EN;
    TIM2->CR1 |= TIM_CR1_CEN;
}
