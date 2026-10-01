#ifndef __DRI_DMA_H__
#define __DRI_DMA_H__

#include <stm32f10x.h>

/**
 * @brief DMA1的初始化
 */
void dri_dma_init(void);

/**
 * @brief 启动DMA传输
 */
void dri_dma_start(uint32_t p_addr, uint32_t m_addr, uint16_t size);

#endif /* __DRI_DMA_H__ */
