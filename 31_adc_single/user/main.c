#include <stdint.h>
#include <stm32f10x.h>
#include "dri_uart.h"
#include "dri_systick.h"
#include "dri_adc.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2. 初始化systick
	dri_systick_init();

	// 3. 初始化adc
	dri_adc_init();

	// 4. adc上电
	dri_adc_on();

	// 5. 开启转换
	dri_adc_start();

	while (1)
	{
		printf("read v is : %.1f", dri_adc_read());
		my_delay(1000);
	}
}
