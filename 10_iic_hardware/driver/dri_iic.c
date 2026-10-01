#include "dri_iic.h"

/**
 * @brief 初始化
 */
void dri_iic_init(void)
{
    // 1. 开启GPIOB的时钟 开启IIC2的外设时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // 2. 引脚的模式
    // 2.1 PB10->SCL PB11->SDA:复用开漏 MODE:11 CNF:11
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11);

    // 3. IIC的配置
    // 3.1. 配置i2c时钟频率
    I2C2->CR2 &= ~I2C_CR2_FREQ;
    I2C2->CR2 |= 36;

    // 3.2. 配置I2C速度模式
    I2C2->CCR &= ~I2C_CCR_FS;

    // 3.3. 配置I2C分频系数
    I2C2->CCR |= 180;

    // 3.4. 配置I2C的最大上升事件寄存器
    I2C2->TRISE |= 37;

    // 3.5. 开启i2c外设使能
    I2C2->CR1 |= I2C_CR1_PE;
}

/**
 * @brief 发送起始信号
 */
uint8_t dri_iic_start(void)
{
    // 1. 产生起始信号
    I2C2->CR1 |= I2C_CR1_START;

    // 2. 等待起始信号发送完成
    uint16_t timeout = 0xFFFF;
    while (((I2C2->SR1 & I2C_SR1_SB) == 0) && timeout)
    {
        timeout--;
    }

    // 3. 不需要手动清除标志位,只要start发送成功,start会硬件置0, 只要我们开发发送数据,SB也会硬件置0

    // 4. 返回值
    return timeout ? 0 : 1;
}

/**
 * @brief 发送停止信号
 */
void dri_iic_stop(void)
{
    // 在寄存器中没有关于主机发送停止信号的判断,直接发送停止信号即可
    I2C2->CR1 |= I2C_CR1_STOP;
}

/**
 * @brief 发送应答信号(开启应答使能)
 */
void dri_iic_ack(void)
{
    I2C2->CR1 |= I2C_CR1_ACK;
}

/**
 * @brief 发送非应答信号(关闭应答使能)
 */
void dri_iic_nack(void)
{
    I2C2->CR1 &= ~I2C_CR1_ACK;
}

/**
 * @brief 发送地址
 */
uint8_t dri_iic_sendaddr(uint8_t addr)
{
    // 1. 给DR寄存器写入数据
    I2C2->DR = addr;

    // 2. 判断发送成功(ADDR标志位会在接收到地址消息的ACK应答后置1)或者超时
    uint16_t timeout = 0xFFFF;
    while (((I2C2->SR1 & I2C_SR1_ADDR) == 0) && timeout)
    {
        timeout--;
    }

    // 3. ADDR位的清除:在软件读取SR1寄存器后，对SR2寄存器的读操作将清除该位
    if (timeout > 0)
    {
        I2C2->SR2;
    }

    // 4. 返回值
    return timeout ? 0 : 1;
}

/**
 * @brief 发送数据
 */
uint8_t dri_iic_sendbyte(uint8_t byte)
{
    // 1. 写之前要判断是否可以发送数据
    uint16_t timeout = 0xFFFF;
    while (((I2C2->SR1 & I2C_SR1_TXE) == 0) && timeout)
    {
        timeout--;
    }

    // 2. 判断是否是超时
    if (timeout == 0)
    {
        return 1;
    }

    // 3. 给DR写入数据
    I2C2->DR = byte;

    // 4. 循环等待数据发送完成
    timeout = 0xFFFF;
    while (((I2C2->SR1 & I2C_SR1_BTF) == 0) && timeout)
    {
        timeout--;
    }

    // 5.返回
    return timeout ? 0 : 1;
}

/**
 * @brief 接收数据
 */
uint8_t dri_iic_receivebyte(uint8_t *byte)
{
    // 1. 循环等待数据数据寄存器收到数据
    uint16_t timeout = 0xFFFF;
    while (((I2C2->SR1 & I2C_SR1_RXNE) == 0) && timeout)
    {
        timeout--;
    }

    // 2. 读取 并返回
    if (timeout)
    {
        *byte = I2C2->DR;
        return 0;
    }
    else
    {
        return 1;
    }
}