#ifndef __DRI_SPI_H__
#define __DRI_SPI_H__

#include <stm32f10x.h>

#define SCK_HIGH (GPIOA->ODR |= GPIO_ODR_ODR5)
#define SCK_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR5)

#define MOSI_HIGH (GPIOA->ODR |= GPIO_ODR_ODR7)
#define MOSI_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR7)

#define MISO_READ (GPIOA->IDR & GPIO_IDR_IDR6)

#define CS_LOW (GPIOC->ODR |= GPIO_ODR_ODR13) 
#define CS_HIGH (GPIOC->ODR &= ~GPIO_ODR_ODR13)

/**
 * @brief SPI初始化
 */
void dri_spi_init(void);

/**
 * @brief 交换数据
 */
uint8_t dri_spi_swap(uint8_t byte);

#endif /* __DRI_SPI_H__ */
