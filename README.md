# STM32F103 嵌入式学习代码

从寄存器/标准库一路写到 HAL 的 STM32F103 练习记录，共 **35 个独立工程**。
每个主题基本都做了两遍：先用寄存器或标准库手写一遍，再用 STM32CubeMX + HAL 重写一遍，
便于对照两种写法的差异。

## 硬件与工具链

| 项目 | 说明 |
| --- | --- |
| MCU | STM32F103ZETx（Cortex-M3，512 KB Flash，64 KB SRAM） |
| 主频 | 72 MHz，Keil 目标中配置的晶振为 12 MHz |
| 主要 IDE | Keil MDK / uVision（工程由 uVision 5.43.1.0 生成） |
| 部分工程 | EIDE（VS Code 嵌入式插件），见 `02_LED _flew`、`04_Key_LED` |

## 工程结构约定

**寄存器 / 标准库写法**采用分层结构，各层职责清晰：

```
start/        启动文件与 CMSIS 内核支持（core_cm3.c、system_stm32f10x.c）
diver/        驱动层，直接操作外设寄存器（dri_led_gpio.c、dri_uart.c …）
interface/    接口层，对驱动做统一封装供上层调用（inf_led.c、inf_key.c …）
common/       公共组件（common_delay.c 等）
application/  应用逻辑
user/         主程序入口 main.c
RTE/          Keil 运行时环境与器件调试配置
```

**HAL / CubeMX 写法**为 CubeMX 生成的标准布局，业务代码放在 `interface/`：

```
Core/Inc, Core/Src   CubeMX 生成的初始化代码与 main.c
Drivers/             ST 官方 HAL 库与 CMSIS 器件头文件
interface/           自己写的接口层
MDK-ARM/             Keil 工程文件
*.ioc                CubeMX 工程配置
```

## 练习清单

| 序号 | 寄存器 / 标准库 | HAL (CubeMX) | 主题 |
| --- | --- | --- | --- |
| 01 | `01_LED`、`02_LED _flew` | `03_LED_flow_HAL` | GPIO 点灯与流水灯 |
| 02 | `04_Key_LED` | — | 按键控制 LED |
| 03 | `05_Usart_poll_rt` | `06_usart_poll_hall` | 串口轮询收发 |
| 04 | `07_usart_printf` | `08_usart_printf_hal` | 串口 printf 重定向 |
| 05 | `09_iic_software`、`10_iic_hardware` | `11_iic_hardware_hal` | IIC 软件模拟 / 硬件外设 |
| 06 | `12_systick`、`13_systick_plus` | `14_systick_hal` | SysTick 延时（基础版 / 增强版） |
| 07 | `15_base_timer` | `16_base_timer_hal` | 基本定时器 |
| 08 | `17_breath_led` | `18_breath_led_hal` | PWM 呼吸灯 |
| 09 | `19_get_pwm_freq` | `20_get_pwm_freq_hal` | 测量 PWM 频率 |
| 10 | `21_get_pwm_dutycycle` | `22_get_pwm_dutycycle_hal` | 测量 PWM 占空比 |
| 11 | `23_led_focus_fixnum` | `24_led_focus_fixnum_hal` | LED 调光（定点数实现） |
| 12 | `25_dma_mem2mem` | `26_dma_mem2mem_hal` | DMA 内存到内存 |
| 13 | `27_dma_mem2uart` | `28_dma_mem2uart_hal` | DMA 内存到串口 |
| 14 | `29_dma_mem2tim` | `30_dma_mem2tim_hal` | DMA 内存到定时器 |
| 15 | `31_adc_single` | `32_adc_single_hal` | ADC 单通道采集 |
| 16 | `33_adc_double`、`34_adc_double` | — | ADC 双通道采集 |
| 17 | `35_spi_software` | — | 软件模拟 SPI |

## 如何编译与烧录

- **Keil 工程**：用 Keil uVision 打开各工程目录下的 `*.uvprojx`，直接 Build 并下载即可。
- **EIDE 工程**（`02_LED _flew`、`04_Key_LED`）：在 VS Code 中打开该目录，用 EIDE 插件构建。

## 关于仓库内容

仓库只包含源码与工程定义文件，已通过 `.gitignore` 排除以下内容：

- Keil 构建产物（`Objects/`、`Listings/`、`DebugConfig/`、`*.o`、`*.axf`、`*.hex` 等）
- EIDE 构建产物与 CMSIS 器件包缓存（`build/`、`.cmsis/`）
- 每用户的 Keil 界面布局文件（`*.uvguix.*`，其中含本机用户名）
- 编辑器与工具目录（`.vscode/`、`.reasonix/` 等）

因此克隆下来后**需要自己重新编译**才能得到可烧录的固件。
`Drivers/`、`start/` 下的 ST 与 ARM 官方文件按原样保留，以保持工程可直接打开与编译。

> 注：`02_LED _flew` 目录名中间多了一个空格（历史笔误），保留原样以免破坏工程内的相对路径引用。
