#include <stdint.h>
#include <stm32f10x.h>
#include "inf_led.h"
#include "dri_uart.h"
#include "dri_systick.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2. 初始化LED
	inf_led_init();

	// 2. 初始化systick
	dri_systick_init();

	while (1)
	{
	}
}
