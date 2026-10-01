#include "dri_spi.h"

/**
 * @brief SPI初始化
 */
void dri_spi_init(void)
{
    // 1. SCK-PA5 MISO->PA6 MOSI->PA7 CS->PC13
    // 1. 开启时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2. 工作模式 SCK->通用推挽(MODE:11 CNF:00) MISO->浮空输入(MODE:00 CNF:01) MOSI->通用推挽 CS->通用推挽
    GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
    GPIOC->CRH |= GPIO_CRH_MODE13;
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
    GPIOC->CRH &= ~GPIO_CRH_CNF13;

    GPIOA->CRL &= ~GPIO_CRL_MODE6;
    GPIOA->CRL &= ~GPIO_CRL_CNF6_1;
    GPIOA->CRL |= GPIO_CRL_CNF6_0;

    // 3. SPI工作模式(模式0)
    // 初始把时钟线拉低
    SCK_LOW;
}

/**
 * @brief 交换数据(收发一体的方法)
 */
uint8_t dri_spi_swap(uint8_t byte)
{
    // 思路:SCK为低,把byte的高位取出来,放在MOSI上边
    //  SCK拉高 等待从机去MOSI采样
    //  顺便 读取MISO的值 并放在byte的低位

    for (uint8_t i = 0; i < 8; i++)
    {
        // 1. 取byte的高位
        if (byte & 0x80)
        {
            MOSI_HIGH;
        }
        else
        {
            MOSI_LOW;
        }
        // 2. 准备完一位数据后 把byte左移1位
        byte <<= 1;

        // 3. 拉高时钟
        SCK_HIGH;

        // 4. 主机去MISO上采样就行
        byte |= MISO_READ ? 1 : 0;

        // 5. 全部结束之后 拉低时钟
        SCK_LOW;
    }

    return byte;
}