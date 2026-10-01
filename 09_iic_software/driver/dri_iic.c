#include "dri_iic.h"

/**
 * @brief 初始化
 */
void dri_iic_init(void)
{
    // 1. 开启GPIOB的时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    // 2. 引脚的模式
    // 2.1 PB6->SCL PB7->SDA: MODE:11 CNF:01
    GPIOB->CRL |= (GPIO_CRL_MODE6 | GPIO_CRL_MODE7);
    GPIOB->CRL |= (GPIO_CRL_CNF6_0 | GPIO_CRL_CNF7_0);
    GPIOB->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1);
}

/**
 * @brief 发送起始信号
 */
void dri_iic_start(void)
{
    // 1. 保证SDA和SCL为高电平
    SDA_HIGH;
    SCL_HIGH;
    IIC_DELAY;

    // 2. SCL为高的时候 拉低SDA
    SDA_LOW;
    IIC_DELAY;

    // 3. 确保起始信号后 SCL为低
    SCL_LOW;
    IIC_DELAY;
}

/**
 * @brief 发送停止信号
 */
void dri_iic_stop(void)
{
    // 1. 先不要着急拉低SDA,先要保证SCL为低 再操作SDA(否则可能被认为是一个起始信号)
    SCL_LOW;
    IIC_DELAY;

    // 2. 拉低SDA,方便稍微发送结束信号时 能拉高SDA
    SDA_LOW;
    IIC_DELAY;

    // 3. 把SCL拉高(结束信号是:SCL为高的时候,SDA有一个上升沿)
    SCL_HIGH;
    IIC_DELAY;

    // 4. 拉高SDA 发送结束信号
    SDA_HIGH;
    IIC_DELAY;
}

/**
 * @brief 发送应答信号(在SCL为低的时候,改变SDA为低,等待别人采样这个应答信号)
 */
void dri_iic_ack(void)
{
    // 1. 拉低SCL
    SCL_LOW;
    IIC_DELAY;

    // 2. 拉低SDA
    SDA_LOW;
    IIC_DELAY;

    // 3. 拉高SCL等待被采样
    SCL_HIGH;
    IIC_DELAY;

    // 4. 结束采样(先把SCL拉低,释放SDA,这样就可以继续让从机操作SDA给我们发数据)
    SCL_LOW;
    IIC_DELAY;

    // 5. 释放SDA
    SDA_HIGH;
    IIC_DELAY;
}

/**
 * @brief 发送非应答信号
 */
void dri_iic_nack(void)
{
    // 1. 拉低SCL
    SCL_LOW;
    IIC_DELAY;

    // 2. 拉低SDA
    SDA_HIGH;
    IIC_DELAY;

    // 3. 拉高SCL等待被采样
    SCL_HIGH;
    IIC_DELAY;

    // 4. 结束采样(先把SCL拉低,释放SDA,这样就可以继续让从机操作SDA给我们发数据)
    SCL_LOW;
    IIC_DELAY;

    // 5. 释放SDA
    SDA_HIGH;
    IIC_DELAY;
}

/**
 * @brief 接收应答
 */
uint8_t dri_iic_receive_ack(void)
{
    // 1. 拉低SCL 确保从机可以修改SDA
    SCL_LOW;
    IIC_DELAY;

    // 2. 释放SDA 方便从机管理
    SDA_HIGH;
    IIC_DELAY;

    // 3. 拉高SCL(确保可以采样)
    SCL_HIGH;
    IIC_DELAY;

    // 4. 采样
    uint8_t result = READ_SDA == 0 ? ACK : NACK;

    // 5. 拉低SCL,方便接收应答后 后续的修改SDA的操作
    SCL_LOW;
    IIC_DELAY;

    return result;
}

/**
 * @brief 发送数据
 */
void dri_iic_sendbyte(uint8_t byte)
{
    // 1. 遍历发送byte中的每一位
    for (uint8_t i = 0; i < 8; i++)
    {
        // 2. 想要改变SDA 就要拉低SCL
        SCL_LOW;
        IIC_DELAY;

        // 3. 取byte的高位优先
        if (byte & 0x80)
        {
            SDA_HIGH;
        }
        else
        {
            SDA_LOW;
        }
        IIC_DELAY;

        // 4. 等待被采样
        SCL_HIGH;
        IIC_DELAY;

        byte <<= 1;
    }

    // 5. 最后结束拉低SCL 结束采样
    SCL_LOW;
    IIC_DELAY;
}

/**
 * @brief 接收数据
 */
uint8_t dri_iic_receivebyte(void)
{
    // 1. 定义一个容器 接收iic的数据
    uint8_t byte = 0;

    // 2. 遍历接收IIC数据
    for (uint8_t i = 0; i < 8; i++)
    {
        // 3. 先拉低SCL 确保从机可以有机会修改SDA值
        SCL_LOW;
        IIC_DELAY;

        // 4. 拉高SCL 开始采样
        SCL_HIGH;
        IIC_DELAY;

        // 5. 采样
        byte <<= 1; // 写到这里之后，有效移位只有7次
        if (READ_SDA)
        {
            byte |= 0x01;
        }
    }

    // 6. 结束采样
    SCL_LOW;
    IIC_DELAY;

    return byte;
}

