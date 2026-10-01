#include <stdint.h>
#include <stm32f10x.h>
#include "dri_uart.h"
#include "dri_tim2.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2. 初始化tim2
	dri_tim2_init();

	// 3. 使能
	dri_tim2_enable();

	while (1)
	{
	}
}
