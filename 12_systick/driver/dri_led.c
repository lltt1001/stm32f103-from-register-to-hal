#include "dri_led.h"

/**
 * @brief LED Driver 初始化
 */
void dri_led_init(void)
{

    // 1. 开启GPIOA的时钟使能
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // 2. 设置PA0 PA1 PA8引脚为推挽输出模式
    GPIOA->CRL &= ~GPIO_CRL_MODE0;
    GPIOA->CRL |= GPIO_CRL_MODE0_1;
    GPIOA->CRL &= ~GPIO_CRL_MODE0_0;
    GPIOA->CRL &= ~GPIO_CRL_CNF0;

    GPIOA->CRL &= ~GPIO_CRL_MODE1;
    GPIOA->CRL |= GPIO_CRL_MODE1_1;
    GPIOA->CRL &= ~GPIO_CRL_MODE1_0;
    GPIOA->CRL &= ~GPIO_CRL_CNF1;

    GPIOA->CRH &= ~GPIO_CRH_MODE8;
    GPIOA->CRH |= GPIO_CRH_MODE8_1;
    GPIOA->CRH &= ~GPIO_CRH_MODE8_0;
    GPIOA->CRH &= ~GPIO_CRH_CNF8;
}
