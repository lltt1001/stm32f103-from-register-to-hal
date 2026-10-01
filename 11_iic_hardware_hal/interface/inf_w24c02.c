#include "inf_w24c02.h"

/**
 * @brief 初始化设备
 */
void inf_w24c02_init(void)
{
    // 1. 初始化驱动
    // dri_iic_init();
}

/**
 * @brief 向EEPROM中写入1个字节
 */
W24C02_Result_type inf_w24c02_write_byte(uint8_t inner_addr, uint8_t byte)
{
    // 发送数据
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(&hi2c2, DEV_ADD, inner_addr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 1000);

    if (result != HAL_OK)
    {
        printf("write data is error ...\r\n");
        return W24C02_RESULT_NACK;
    }

    // 写周期
    HAL_Delay(5);

    return W24C02_RESULT_OK;
}

/**
 * @brief 从EEPROM中读取1个字节出来
 */
W24C02_Result_type inf_w24c02_read_byte(uint8_t inner_addr, uint8_t *byte)
{
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(&hi2c2, DEV_ADD, inner_addr, I2C_MEMADD_SIZE_8BIT, byte, 1, 1000);

    if (result != HAL_OK)
    {
        printf("read byte is error ...\r\n");
        return W24C02_RESULT_NACK;
    }

    return W24C02_RESULT_OK;
}

/**
 * @brief 页写(写字符串)
 */
W24C02_Result_type inf_w24c02_write_bytes(uint8_t inner_addr, uint8_t *data, uint8_t size)
{
    // 发送数据
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(&hi2c2, DEV_ADD, inner_addr, I2C_MEMADD_SIZE_8BIT, data, size, 1000);

    if (result != HAL_OK)
    {
        printf("write data is error ...\r\n");
        return W24C02_RESULT_NACK;
    }

    // 写周期
    HAL_Delay(5);

    return W24C02_RESULT_OK;
}

/**
 * @brief 读多个(读字符串)
 */
W24C02_Result_type inf_w24c02_read_bytes(uint8_t inner_addr, uint8_t *data, uint8_t size)
{
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(&hi2c2, DEV_ADD, inner_addr, I2C_MEMADD_SIZE_8BIT, data, size, 1000);

    if (result != HAL_OK)
    {
        printf("read data is error ...\r\n");
        return W24C02_RESULT_NACK;
    }

    return W24C02_RESULT_OK;
}
