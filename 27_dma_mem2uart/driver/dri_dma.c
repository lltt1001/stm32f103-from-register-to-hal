#include "dri_dma.h"

/**
 * @brief DMA1的初始化
 */
void dri_dma_init(void)
{
    // 1. 开启DMA时钟
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    // 2. DMA配置
    // 2.1 传输方向(存储器到存储器 把flash称作为外设)
    DMA1_Channel4->CCR &= ~DMA_CCR4_MEM2MEM;
    DMA1_Channel4->CCR |= DMA_CCR4_DIR;

    // 2.2 通道优先级
    DMA1_Channel4->CCR &= ~DMA_CCR4_PL;

    // 2.3 数据宽度
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE;

    // 2.4 地址增量模式
    DMA1_Channel4->CCR |= DMA_CCR4_MINC;
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC;

    // 2.5 循环模式
    DMA1_Channel4->CCR &= ~DMA_CCR4_CIRC;

    // 3. 中断配置
    DMA1_Channel4->CCR |= DMA_CCR4_TCIE;
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 6);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

/**
 * @brief 启动DMA传输
 */
void dri_dma_start(uint32_t p_addr, uint32_t m_addr, uint16_t size)
{
    // 1. 外设地址
    DMA1_Channel4->CPAR = p_addr;

    // 2. 存储器地址
    DMA1_Channel4->CMAR = m_addr;

    // 3. 传输数据长度
    DMA1_Channel4->CNDTR = size;

    // 4. 使能
    USART1->CR3 |= USART_CR3_DMAT;
    DMA1_Channel4->CCR |= DMA_CCR4_EN;
}

/**
 * @brief 中断服务程序
 */
extern uint8_t is_dma_finished;
void DMA1_Channel4_IRQHandler(void)
{
    // 1. 判断标志位
    if (DMA1->ISR & DMA_ISR_TCIF1)
    {
        // 2. 清标志位
        DMA1->IFCR |= DMA_IFCR_CTCIF1;

        // 3.关闭DMA
        DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
        USART1->CR3 &= ~USART_CR3_DMAT;

        // 4.把状态位置1
        is_dma_finished = 1;
    }
}