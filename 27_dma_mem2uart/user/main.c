#include <stdint.h>
#include <stm32f10x.h>
#include "dri_uart.h"
#include "dri_dma.h"

uint8_t str[] = "abcdefh";

uint8_t is_dma_finished = 0;
int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2.初始化dma
	dri_dma_init();

	// 3. 启动DMA传输
	dri_dma_start((uint32_t)&USART1->DR, (uint32_t)str, 7);

	while (1)
	{
		
	}
}
