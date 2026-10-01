#include <stdint.h>
#include <stm32f10x.h>
#include "dri_uart.h"
#include "dri_systick.h"
#include "dri_adc.h"

uint16_t data[2] = {0};
int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2. 初始化systick
	dri_systick_init();

	// 3. 初始化adc
	dri_adc_init();

	// 4. 初始化adc的dma
	dri_adc_dma_init();

	// 4. adc上电
	dri_adc_on();

	// 5. 开启转换
	dri_adc_start_dma((uint32_t)data, 2);

	while (1)
	{
		printf("read value is : %.1f, %.1f \r\n", data[0] * 3.3 / 4095, data[1] * 3.3 / 4095);
		my_delay(1000);
	}
}
