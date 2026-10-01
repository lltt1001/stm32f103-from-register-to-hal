#include <stdint.h>
#include <stm32f10x.h>
#include "dri_uart.h"
#include "inf_w24c02.h"

int main()
{
	// 1. 初始化uart
	dri_uart_init();

	// 2. 初始化W24C02
	inf_w24c02_init();

	// 3. 测试单个字符的写入
	inf_w24c02_write_byte(0x00, 'a');
	inf_w24c02_write_byte(0x01, 'b');
	inf_w24c02_write_byte(0x02, 'c');

	// 4. 测试单个字符的读取
	uint8_t byte1 = 0, byte2 = 0, byte3 = 0;
	inf_w24c02_read_byte(0x00, &byte1);
	inf_w24c02_read_byte(0x01, &byte2);
	inf_w24c02_read_byte(0x02, &byte3);

	// 5. 打印测试
	printf("get char is : %c,%c,%c \r\n", byte1, byte2, byte3);

	// 6. 测试写入多个字符
	inf_w24c02_write_bytes(0x00, "abcdefg", 7);

	// 7. 测试获取多个字符
	uint8_t data[100];
	inf_w24c02_read_bytes(0x00, data, 7);

	// 8. 打印测试
	printf("get string is : %.*s\r\n", 7, data);

	while (1)
	{
	}
}
