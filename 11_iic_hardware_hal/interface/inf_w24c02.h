#ifndef __INF_W24C02_H__
#define __INF_W24C02_H__

#include <i2c.h>
#include "stdio.h"

#define DEV_ADD (0X50 << 1)

typedef enum
{
    W24C02_RESULT_OK = 0,
    W24C02_RESULT_TIMEOUT,
    W24C02_RESULT_NOT_DEVIDE,
    W24C02_RESULT_NACK,
} W24C02_Result_type;

/**
 * @brief 初始化设备
 */
void inf_w24c02_init(void);

/**
 * @brief 向EEPROM中写入1个字节
 */
W24C02_Result_type inf_w24c02_write_byte(uint8_t inner_addr, uint8_t byte);

/**
 * @brief 从EEPROM中读取1个字节出来
 */
W24C02_Result_type inf_w24c02_read_byte(uint8_t inner_addr, uint8_t *byte);

/**
 * @brief 页写(写字符串)
 */
W24C02_Result_type inf_w24c02_write_bytes(uint8_t inner_addr, uint8_t *data, uint8_t size);

/**
 * @brief 读多个(读字符串)
 */
W24C02_Result_type inf_w24c02_read_bytes(uint8_t inner_addr, uint8_t *data, uint8_t size);
#endif /* __INF_W24C02_H__ */
