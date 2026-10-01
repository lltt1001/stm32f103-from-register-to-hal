#include "dri_key.h"

/**
 * @brief key的驱动初始化
 * key1(sw3) --> PF8
 */
/**
 * @brief 初始化DRI按键
 * @note  此函数用于配置按键相关的硬件，包括GPIO时钟、外部中断等
 */
void dri_key_init(void)
{
    // 1. 开启GPIOF的时钟(去stm32f10x.h 中 开启当前的芯片型号)
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;

    // 2. 因为用到了外部中断 所以要开启AFIO的时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    // 3. 配置PF8为上拉输入模式(MODE:00 CNF:10)
    // 3.1 配置模式
    GPIOF->CRH &= ~GPIO_CRH_MODE8;
    GPIOF->CRH &= ~GPIO_CRH_CNF8_0;
    GPIOF->CRH |= GPIO_CRH_CNF8_1;

    // 3.2 选择上拉还是下拉
    GPIOF->ODR |= GPIO_ODR_ODR8;

    // 4. 配置AFIO，将PF8映射为外部中断线(多位的情况下 先清0 后设置)
    AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI8;
    AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PF;

    // 5. 配置EXTI中断配置
    // 5.1 开启对应中断使能位
    EXTI->IMR |= EXTI_IMR_MR8;
    // 5.2 配置中断类型为下降沿触发
    EXTI->FTSR |= EXTI_FTSR_TR8;

    // 6. 配置NVIC 优先级配置
    // 6.1 配置整个项目的优先级组
    NVIC_SetPriorityGrouping(3);
    // 6.2 根据中断号设置优先级
    NVIC_SetPriority(EXTI9_5_IRQn, 5);
    // 6.3 开启中断使能
    NVIC_EnableIRQ(EXTI9_5_IRQn);
}
