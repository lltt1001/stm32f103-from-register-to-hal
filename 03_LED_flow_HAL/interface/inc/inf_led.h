/*
 * ============================================================================
 * LED接口头文件 - inf_led.h
 * ============================================================================
 * 
 * 【为什么这样设计？】
 * ---------------------------------------------------------------------------
 * 老师的代码：所有LED都在GPIOA端口，所以函数只需传引脚号（如LED1_Pin）
 *   void inf_led_open(uint16_t led);  // 内部固定用GPIOA
 * 
 * 我们的代码：LED分布在不同的GPIO端口
 *   - LED1在PB5（GPIOB端口的第5脚）
 *   - LED2在PE5（GPIOE端口的第5脚）
 * 
 * 所以函数需要同时传入"端口"和"引脚"两个参数：
 *   void inf_led_open(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
 *   - GPIOx: 指定哪个端口（GPIOB、GPIOE等）
 *   - GPIO_Pin: 指定哪个引脚（GPIO_PIN_5等）
 * 
 * 【分层架构说明】
 * ---------------------------------------------------------------------------
 * 本代码采用三层架构：
 * 
 *   应用层 (Core/Src/main.c)
 *       ↓ 调用接口层函数
 *   接口层 (interface/inf_led.c)  <- 本文件所在层
 *       ↓ 调用HAL库函数
 *   驱动层 (STM32 HAL库)
 * 
 * 接口层的作用：
 *   - 对上：提供简单易用的API，main.c不需要关心底层细节
 *   - 对下：封装HAL库的GPIO操作，统一处理不同端口的LED
 */

#ifndef __INF_LED_H__
#define __INF_LED_H__

#include "gpio.h"  // 包含GPIO相关定义（GPIO_TypeDef、GPIO_PIN_x等）

/**
 * @brief 初始化LED系统
 * 
 * 功能：
 *   1. 调用MX_GPIO_Init()配置GPIO引脚为输出模式
 *   2. 关闭所有LED，确保上电后处于已知状态
 * 
 * 使用示例：
 *   inf_led_init();  // 在main函数开始时调用一次
 */
void inf_led_init(void);

/**
 * @brief 点亮指定引脚的LED
 * 
 * @param GPIOx    GPIO端口指针（如GPIOB、GPIOE）
 * @param GPIO_Pin GPIO引脚号（如GPIO_PIN_5）
 * 
 * 工作原理：
 *   - 将指定引脚设为低电平（0）
 *   - 因为LED是低电平点亮（阴极接GPIO，阳极接VCC）
 * 
 * 与老师代码的区别：
 *   老师：inf_led_open(LED1_Pin);           // 内部固定用GPIOA
 *   我们：inf_led_open(GPIOB, LED1_Pin);    // 明确指定端口和引脚
 * 
 * 使用示例：
 *   inf_led_open(GPIOB, LED1_Pin);  // 点亮PB5上的LED1
 *   inf_led_open(GPIOE, LED2_Pin);  // 点亮PE5上的LED2
 */
void inf_led_open(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief 关闭指定引脚的LED
 * 
 * @param GPIOx    GPIO端口指针
 * @param GPIO_Pin GPIO引脚号
 * 
 * 工作原理：
 *   - 将指定引脚设为高电平（1）
 *   - 高电平时LED两端无电压差，LED熄灭
 * 
 * 使用示例：
 *   inf_led_close(GPIOB, LED1_Pin);  // 关闭LED1
 */
void inf_led_close(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

/**
 * @brief 关闭所有LED
 * 
 * 功能：
 *   - 遍历所有LED，将它们全部关闭
 *   - 即使LED在不同GPIO端口，也能一次性全部关闭
 * 
 * 应用场景：
 *   - 初始化时确保所有LED处于熄灭状态
 *   - 程序异常时快速关闭所有LED
 * 
 * 使用示例：
 *   inf_led_close_all();  // 关闭LED1和LED2
 */
void inf_led_close_all(void);

/**
 * @brief 点亮所有LED
 * 
 * 功能：
 *   - 遍历所有LED，将它们全部点亮
 * 
 * 应用场景：
 *   - 测试所有LED是否正常工作
 *   - 实现"全亮"效果
 * 
 * 使用示例：
 *   inf_led_open_all();  // 点亮LED1和LED2
 */
void inf_led_open_all(void);

/**
 * @brief 切换指定引脚LED的状态（亮变灭，灭变亮）
 * 
 * @param GPIOx    GPIO端口指针
 * @param GPIO_Pin GPIO引脚号
 * 
 * 工作原理：
 *   - 自动翻转引脚当前电平
 *   - 如果当前是低电平(亮) -> 翻转为高电平(灭)
 *   - 如果当前是高电平(灭) -> 翻转为低电平(亮)
 * 
 * 应用场景：
 *   - 实现LED闪烁效果
 *   - 按钮控制LED开关
 * 
 * 使用示例：
 *   inf_led_toggle(GPIOB, LED1_Pin);  // 翻转LED1状态
 */
void inf_led_toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

#endif /* __INF_LED_H__ */