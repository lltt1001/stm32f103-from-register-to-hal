#include <stdint.h>
#include <stm32f10x.h>
#include "dri_uart.h"
#include "dri_tim3.h"
#include "dri_systick.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();
	dri_systick_init();

	// 2. 初始化TIM3
	dri_tim3_init();

	// 3. 使能TIM3
	dri_tim3_enable();

	// 4. 配置比较寄存器的值
	// dri_tim3_set_compare(50);

	// 5. 定义标志位表示当前是 变量还是变暗
	uint8_t flag_up = 1;
	// 6. 定义变量 保存当前的比较值
	uint16_t compare_value = 0;
	while (1)
	{
		if (flag_up == 1)
		{
			compare_value++;
			if (compare_value == 999)
			{
				flag_up = 0;
			}
		}
		else
		{
			compare_value--;
			if (compare_value == 0)
			{
				flag_up = 1;
			}
		}

		dri_tim3_set_compare(compare_value);

		my_delay(1);
	}
}

