#include <stdint.h>
#include <stm32f10x.h>
#include "inf_led.h"
#include "dri_uart.h"
#include "dri_systick.h"
#include "dri_tim6.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2. 初始化LED
	inf_led_init();

	// 3. 初始化TIM6
	dri_tim6_init();

	// 4. 使能TIM6
	dri_tim6_enable();

	while (1)
	{
	}
}
