#include <stdint.h>
#include <stm32f10x.h>
#include "dri_uart.h"
#include "dri_tim1.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2. 初始化TIM1
	dri_tim1_init();

	// 3. 启动TIM1
	dri_tim1_enable();

	while (1)
	{
	}
}
