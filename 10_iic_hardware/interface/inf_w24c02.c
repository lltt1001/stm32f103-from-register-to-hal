#include "inf_w24c02.h"

/**
 * @brief 初始化设备
 */
void inf_w24c02_init(void)
{
    // 1. 初始化驱动
    dri_iic_init();
}

/**
 * @brief 向EEPROM中写入1个字节
 */
W24C02_Result_type inf_w24c02_write_byte(uint8_t inner_addr, uint8_t byte)
{
    // 1. 发送起始信号
    uint8_t result = dri_iic_start();
    if (result != 0)
    {
        printf("start timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 2. 发送设备地址
    result = dri_iic_sendaddr(DEV_W_ADD);
    if (result != 0)
    {
        printf("addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 3. 发送内部书写地址
    result = dri_iic_sendbyte(inner_addr);
    if (result != 0)
    {
        printf("inner addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 6. 发送数据
    result = dri_iic_sendbyte(byte);
    if (result != 0)
    {
        printf("data timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 8.发送停止信号
    dri_iic_stop();

    // 9. 写周期
    common_delay_ms(5);

    return W24C02_RESULT_OK;
}

/**
 * @brief 从EEPROM中读取1个字节出来
 */
W24C02_Result_type inf_w24c02_read_byte(uint8_t inner_addr, uint8_t *byte)
{
    // 1. 发送起始信号
    uint8_t result = dri_iic_start();
    if (result != 0)
    {
        printf("start timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 2. 发送设备地址
    result = dri_iic_sendaddr(DEV_W_ADD);
    if (result != 0)
    {
        printf("addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 3. 发送内部书写地址
    result = dri_iic_sendbyte(inner_addr);
    if (result != 0)
    {
        printf("inner addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 4. 发送真读起始信号
    result = dri_iic_start();
    if (result != 0)
    {
        printf("start timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 5. 发送设备地址
    result = dri_iic_sendaddr(DEV_R_ADD);
    if (result != 0)
    {
        printf("addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 6. 关闭应答使能(表示 未来收到数据以后不做应答)
    dri_iic_nack();

    // 7. 读取一个字节数据
    result = dri_iic_receivebyte(byte);
    if (result != 0)
    {
        printf("recevice data timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 8. 发送停止信号
    dri_iic_stop();

    return W24C02_RESULT_OK;
}

/**
 * @brief 页写(写字符串)
 */
W24C02_Result_type inf_w24c02_write_bytes(uint8_t inner_addr, uint8_t *data, uint8_t size)
{
    // 1. 发送起始信号
    uint8_t result = dri_iic_start();
    if (result != 0)
    {
        printf("start timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 2. 发送设备地址
    result = dri_iic_sendaddr(DEV_W_ADD);
    if (result != 0)
    {
        printf("addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 3. 发送内部书写地址
    result = dri_iic_sendbyte(inner_addr);
    if (result != 0)
    {
        printf("inner addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 6. 发送数据
    for (uint16_t i = 0; i < size; i++)
    {
        result = dri_iic_sendbyte(data[i]);
        if (result != 0)
        {
            printf("data timeout....\r\n");
            return W24C02_RESULT_TIMEOUT;
        }
    }

    // 8.发送停止信号
    dri_iic_stop();

    // 9. 写周期
    common_delay_ms(5);

    return W24C02_RESULT_OK;
}

/**
 * @brief 读多个(读字符串)
 */
W24C02_Result_type inf_w24c02_read_bytes(uint8_t inner_addr, uint8_t *data, uint8_t size)
{
    // 1. 发送起始信号
    uint8_t result = dri_iic_start();
    if (result != 0)
    {
        printf("start timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 2. 发送设备地址
    result = dri_iic_sendaddr(DEV_W_ADD);
    if (result != 0)
    {
        printf("addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 3. 发送内部书写地址
    result = dri_iic_sendbyte(inner_addr);
    if (result != 0)
    {
        printf("inner addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 4. 发送真读起始信号
    result = dri_iic_start();
    if (result != 0)
    {
        printf("start timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 5. 发送设备地址
    result = dri_iic_sendaddr(DEV_R_ADD);
    if (result != 0)
    {
        printf("addr timeout....\r\n");
        return W24C02_RESULT_TIMEOUT;
    }

    // 7. 遍历读取数据
    for (uint16_t i = 0; i < size; i++)
    {
        // 8. 先确定应答的是什么?
        if (i < size - 1)
        {
            dri_iic_ack();
        }
        else
        {
            dri_iic_nack();
        }
        uint8_t byte = 0;
        result = dri_iic_receivebyte(&byte);
        if (result != 0)
        {
            printf("recevice data timeout....\r\n");
            return W24C02_RESULT_TIMEOUT;
        }
        data[i] = byte;
    }

    // 8. 发送停止信号
    dri_iic_stop();

    return W24C02_RESULT_OK;
}
