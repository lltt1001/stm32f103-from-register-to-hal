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
    dri_iic_start();

    // 2. 发送设备地址
    dri_iic_sendbyte(DEV_W_ADD);

    // 3. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NOT_DEVIDE;
    }

    // 4. 发送内部书写地址
    dri_iic_sendbyte(inner_addr);

    // 5. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NACK;
    }

    // 6. 发送数据
    dri_iic_sendbyte(byte);

    // 7. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NACK;
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
    // 1. 发送假写起始信号
    dri_iic_start();

    // 2. 发送设备地址
    dri_iic_sendbyte(DEV_W_ADD);

    // 3. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NOT_DEVIDE;
    }

    // 4. 发送内部书写地址
    dri_iic_sendbyte(inner_addr);

    // 5. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NACK;
    }

    // 6. 发送真读起始信号
    dri_iic_start();

    // 7.发送设备地址
    dri_iic_sendbyte(DEV_R_ADD);

    // 8. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NOT_DEVIDE;
    }

    // 9. 读取一个字节数据
    *byte = dri_iic_receivebyte();

    // 10. 发送非应答信号
    dri_iic_nack();

    // 11. 发送停止信号
    dri_iic_stop();

    return W24C02_RESULT_OK;
}

/**
 * @brief 页写(写字符串)
 */
W24C02_Result_type inf_w24c02_write_bytes(uint8_t inner_addr, uint8_t *data, uint8_t size)
{
    // 1. 发送起始信号
    dri_iic_start();

    // 2. 发送设备地址
    dri_iic_sendbyte(DEV_W_ADD);

    // 3. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NOT_DEVIDE;
    }

    // 4. 发送内部书写地址
    dri_iic_sendbyte(inner_addr);

    // 5. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NACK;
    }

    // 6. 遍历发送一个字节的数据
    for (uint8_t i = 0; i < size; i++)
    {
        dri_iic_sendbyte(data[i]);
        if (dri_iic_receive_ack() == NACK)
        {
            // 发送停止信号
            dri_iic_stop();
            return W24C02_RESULT_NACK;
        }
    }

    // 7.发送停止信号
    dri_iic_stop();

    // 8. 写周期
    common_delay_ms(5);

    return W24C02_RESULT_OK;
}

/**
 * @brief 读多个(读字符串)
 */
W24C02_Result_type inf_w24c02_read_bytes(uint8_t inner_addr, uint8_t *data, uint8_t size)
{
    // 1. 发送假写起始信号
    dri_iic_start();

    // 2. 发送设备地址
    dri_iic_sendbyte(DEV_W_ADD);

    // 3. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NOT_DEVIDE;
    }

    // 4. 发送内部书写地址
    dri_iic_sendbyte(inner_addr);

    // 5. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NACK;
    }

    // 6. 发送真读起始信号
    dri_iic_start();

    // 7.发送设备地址
    dri_iic_sendbyte(DEV_R_ADD);

    // 8. 等待应答
    if (dri_iic_receive_ack() == NACK)
    {
        // 发送停止信号
        dri_iic_stop();
        return W24C02_RESULT_NOT_DEVIDE;
    }

    // 9. 遍历接收数据
    for (uint8_t i = 0; i < size; i++)
    {
        data[i] = dri_iic_receivebyte();

        // 前边我们都要给从机响应一个ack,最后要给从机响应一个nack
        if (i < size - 1)
        {
            dri_iic_ack();
        }
        else
        {
            dri_iic_nack();
        }
    }

    // 10.发送结束信号
    dri_iic_stop();

    return W24C02_RESULT_OK;
}
