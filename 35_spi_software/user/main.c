#include <stdint.h>
#include <stm32f10x.h>
#include "inf_led.h"
#include "dri_uart.h"
#include "dri_systick.h"
#include "inf_flash.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2. 初始化systick
	dri_systick_init();

	// 3. 初始化flash
	inf_flash_init();

	// 4. 获取芯片id
	uint8_t mid = 0;
	uint16_t id = 0;
	inf_flash_chip_id(&mid, &id);
	printf("mid:%02X id:%02X\r\n", mid, id);

	// 5. 擦除芯片
	inf_flash_erase_sector(0x00, 0x00, 0x00);

	// 6. 开始写入
	inf_flash_write(0x00, 0x00, 0x00, "hello sunday!!!", 15);

	// 7. 读取
	uint8_t data[16] = {0};
	inf_flash_read(0x00, 0x00, 0x00, data, 15);
	printf("read data: %.*s\r\n", 15, data);

	while (1)
	{
	}
}
