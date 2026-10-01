#include <stdint.h>
#include <stm32f10x.h>
#include "dri_uart.h"
#include "dri_tim5.h"
#include "dri_systick.h"
#include "dri_tim4.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();
	dri_systick_init();

	// 2. 初始化TIM5 TIM4
	dri_tim5_init();
	dri_tim4_init();

	// 3. 使能TIM5 TIM4
	dri_tim5_enable();
	dri_tim4_enable();

	// 4. 配置TIM5比较寄存器的值 设置占空比50%
	dri_tim5_set_compare(200);

	while (1)
	{
		printf("cycle : %d us \r\n", dri_tim4_get_pwmcycle());
		printf("freq : %.1f hz\r\n", dri_tim4_get_pwmfreq());
		printf("dutycycle: %.1f%% \r\n", dri_tim4_get_dutycycle() * 100);

		my_delay(1000);
	}
}
