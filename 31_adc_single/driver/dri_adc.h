#ifndef __DRI_ADC_H__
#define __DRI_ADC_H__

#include <stm32f10x.h>

/**
 * @brief adc初始化
 */
void dri_adc_init(void);

/**
 * @brief adc上电
 */
void dri_adc_on(void);

/**
 * @brief adc启动转换
 */
void dri_adc_start(void);

/**
 * @brief 读adc的结果
 */
double dri_adc_read(void);

#endif /* __DRI_ADC_H__ */
