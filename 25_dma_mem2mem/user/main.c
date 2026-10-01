#include <stdint.h>
#include <stm32f10x.h>
#include "dri_uart.h"
#include "dri_dma.h"

const uint8_t p_arr[] = {1, 2, 3, 4, 5};
uint8_t m_arr[5] = {0};

uint8_t is_dma_finished = 0;
int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2.初始化dma
	dri_dma_init();

	// 3. 启动DMA传输
	dri_dma_start((uint32_t)p_arr, (uint32_t)m_arr, 5);

	while (1)
	{
		if (is_dma_finished)
		{
			is_dma_finished = 0;
			for (uint16_t i = 0; i < 5; i++)
			{
				printf("%d \r\n", m_arr[i]);
			}
		}
	}
}
