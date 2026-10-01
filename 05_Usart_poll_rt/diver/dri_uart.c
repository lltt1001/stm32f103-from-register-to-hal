#include "dri_uart.h"

/**
 * @brief uart1的初始化
 * PA9 : 复用推挽输出 MODE:11 CNF:10
 * PA10: 浮空输入 MODE:00 CNF:01
 */
void dri_uart_init(void)
{
    // 1. 开时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

    // 2. 配置引脚的工作模式
    // 2.1 PA9
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;

    // 2.2 PA10
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;

    // 3. usart的配置
    // 3.1 波特率:115200
    // 72M/16/115200 ->除法因子 39.0625 (39-->0x27) (0.0625--> 1 / 16)
    USART1->BRR = 0x271;
    // 3.2 字长
    USART1->CR1 &= ~USART_CR1_M;
    // 3.3 禁用校验位
    USART1->CR1 &= ~USART_CR1_PCE;
    // 3.4 停止位1位
    USART1->CR2 &= ~USART_CR2_STOP;
    // 3.5使能
    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 |= USART_CR1_TE;
    USART1->CR1 |= USART_CR1_RE;

    // 4. 中断的配置(usart中发送是主动的,接收是被动的,所以正常都是接收会配置中断方式,发送中断的话按需使用即可)
    // 4.1 开启接收中断使能和空闲帧中断使能
    USART1->CR1 |= (USART_CR1_RXNEIE | USART_CR1_IDLEIE);

    // 4.2 NVIC配置
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(USART1_IRQn, 7);
    NVIC_EnableIRQ(USART1_IRQn);
}

/**
 * @brief uart1发送一个字节的数据
 */
void dri_uart_sendchar(uint8_t ch)
{
    // 等待发送寄存器为空的时候 才可以发送
    while ((USART1->SR & USART_SR_TXE) == 0)
    {
    }

    // 发送数据
    USART1->DR = ch;
}

// /**
//  * @brief uart1接收一个字节的数据
//  */
// uint8_t dri_uart_receivechar(void)
// {
//     while ((USART1->SR & USART_SR_RXNE) == 0)
//     {
//         // 因为IDLE位出现的慢,所以可能没有数据了,仍然被送进来循环中
//         // 我们可以判断是否有IDLE位 直接退出即可
//         if (USART1->SR & USART_SR_IDLE)
//         {
//             return;
//         }
//     }

//     return USART1->DR;
// }

/**
 * @brief uart1发送一个字符串的数据
 */
void dri_uart_sendstring(uint8_t data[], uint16_t size)
{
    for (uint16_t i = 0; i < size; i++)
    {
        dri_uart_sendchar(data[i]);
    }
}

// /**
//  * @brief uart1接收一个字符串数据
//  */
// void dri_uart_receivestring(uint8_t data[], uint16_t *size)
// {
//     // 计数器
//     uint16_t count = 0;
//     while ((USART1->SR & USART_SR_IDLE) == 0)
//     {
//         data[count] = dri_uart_receivechar();
//         count++;
//     }

//     // 清除IDLE位
//     USART1->SR;
//     USART1->DR;

//     // 设置size
//     *size = --count;
// }

/**
 * usart1 中断服务程序
 */
uint8_t data[16];
uint8_t size;
void USART1_IRQHandler(void)
{
    // USART1中有很多中断类型,所以首先要判断是哪一个中断类型
    if (USART1->SR & USART_SR_RXNE)
    {
        // 说明收到了一个字节的数据
        data[size] = USART1->DR;
        size++;
    }

    if (USART1->SR & USART_SR_IDLE)
    {
        // 清除IDLE标志位 先读SR再读DR
        USART1->SR;
        USART1->DR;

        // 说明数据接收完成
        dri_uart_sendstring(data, size);
        size = 0;
    }
}
