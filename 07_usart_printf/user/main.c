#include <stdint.h>
#include <stm32f10x.h>
#include "inf_led.h"
#include "common_delay.h"
#include "inf_key.h"
#include "dri_uart.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2. 测试printf

	while (1)
	{
		printf("hello printf...\r\n");
		common_delay_ms(1000);
	}
}
