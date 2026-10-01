#include "dri_adc.h"

/**
 * @brief adc初始化
 */
void dri_adc_init(void)
{
    // 1. 开时钟 ADC1 GPIOC
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2. GPIOC PC0 PC2 配置 MODE:00 CNF:00
    GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0 | GPIO_CRL_MODE2 | GPIO_CRL_CNF2);

    // 3. 配置ADC分频
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;

    // 4. adc初始化
    // 4.1 开启扫描模式
    ADC1->CR1 |= ADC_CR1_SCAN;
    // 4.2 配置为软件触发模式
    ADC1->CR2 |= ADC_CR2_EXTTRIG;
    ADC1->CR2 |= ADC_CR2_EXTSEL;
    // 4.3 开启连续转换
    ADC1->CR2 |= ADC_CR2_CONT;

    // 4.4 通道12和通道10的采样时间
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10 | ADC_SMPR1_SMP12);
    ADC1->SMPR1 |= (1 << 0 | 1 << 6);

    // 4.5 给规则序列配置通道数量
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SQR1_L_0;

    // 4.6 把通道号放在规则序列中
    ADC1->SQR3 &= ~(ADC_SQR3_SQ1 | ADC_SQR3_SQ2);
    ADC1->SQR3 |= (10 << 0 | 12 << 5);
}

/**
 * @brief adc所使用的DMA初始化
 */
void dri_adc_dma_init(void)
{
    // 1. 开启时钟
    RCC->AHBENR = RCC_AHBENR_DMA1EN;

    // 2. 设置数据宽度是16位(因为我们采集的adc数据 12位)
    DMA1_Channel1->CCR &= ~(DMA_CCR1_MSIZE | DMA_CCR1_PSIZE);
    DMA1_Channel1->CCR |= (DMA_CCR1_PSIZE_0 | DMA_CCR1_MSIZE_0);

    // 3. 外设地址不增  存储器地址增
    DMA1_Channel1->CCR |= DMA_CCR1_MINC;

    // 4. 开启循环模式
    DMA1_Channel1->CCR |= DMA_CCR1_CIRC;
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
void dri_adc_start_dma(uint32_t maddr, uint16_t size)
{
    // 1. dma的地址 和 数量
    DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
    DMA1_Channel1->CMAR = maddr;
    DMA1_Channel1->CNDTR = size; 

    // 2. 开启DMA通道使能
    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    // 3. 开启adc的dma使能
    ADC1->CR2 |= ADC_CR2_DMA;

    // 4. 启动转换
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

