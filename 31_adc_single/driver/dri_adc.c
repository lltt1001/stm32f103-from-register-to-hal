#include "dri_adc.h"

/**
 * @brief adc初始化
 */
void dri_adc_init(void)
{
    // 1. 开时钟 ADC1 GPIOC
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2. GPIOC PC0 配置 MODE:00 CNF:00
    GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);

    // 3. 配置ADC分频
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    // 4. adc基础配置
    // 4.1 单通道的话-关闭扫描模式
    ADC1->CR1 &= ~ADC_CR1_SCAN;
    // 4.2 触发模式-软件触发
    ADC1->CR2 |= ADC_CR2_EXTTRIG;
    ADC1->CR2 |= ADC_CR2_EXTSEL;
    // 4.3 开启连续转换
    ADC1->CR2 |= ADC_CR2_CONT;
    // 4.4 采样时间
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10;
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_0;

    // 4.5 给规则组配置长度
    ADC1->SQR1 &= ~ADC_SQR1_L;

    // 4.6 给规则组按照顺序配置通道(单通道-就配置第一个就行了)
    ADC1->SQR3 &= ~ADC_SQR3_SQ1;
    ADC1->SQR3 |= (10 << 0);
}

/**
 * @brief adc上电
 */
void dri_adc_on(void)
{
    // 1. 上电
    ADC1->CR2 |= ADC_CR2_ADON;

    // 2. 复位校准
    ADC1->CR2 |= ADC_CR2_RSTCAL;
    while (ADC1->CR2 & ADC_CR2_RSTCAL)
        ;
    // 3. AD校准
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL)
        ;
}

/**
 * @brief adc启动转换
 */
void dri_adc_start(void)
{
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

/**
 * @brief 读adc的结果
 */
double dri_adc_read(void)
{

    return (ADC1->DR / 4095.0 * 3.3);
}