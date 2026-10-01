#include "../inc/inf_led.h"

/*
 * ============================================================================
 * LED引脚映射表（支持不同GPIO端口）
 * ============================================================================
 * 
 * 【为什么需要这个数组？】
 * ---------------------------------------------------------------------------
 * 问题：你的LED分布在不同的GPIO端口
 *   - LED1在PB5（GPIOB端口的第5脚）
 *   - LED2在PE5（GPIOE端口的第5脚）
 * 
 * 老师的代码：所有LED都在GPIOA，所以可以直接写死端口
 *   HAL_GPIO_WritePin(GPIOA, led, GPIO_PIN_SET);  // 固定用GPIOA
 * 
 * 我们的情况：需要动态处理不同端口，所以用数组存储映射关系
 * 
 * 【结构体数组的作用】
 * ---------------------------------------------------------------------------
 * led_pins[] 是一个结构体数组，每个元素包含两个字段：
 *   - port: GPIO端口指针（指向GPIOB、GPIOE等）
 *   - pin:  引脚号（如GPIO_PIN_5表示第5脚）
 * 
 * 映射关系：
 *   led_pins[0] = {GPIOB, GPIO_PIN_5}  -> LED1连接在GPIOB的第5脚
 *   led_pins[1] = {GPIOE, GPIO_PIN_5}  -> LED2连接在GPIOE的第5脚
 * 
 * 使用方法：
 *   led_pins[i].port  -> 获取第i个LED的GPIO端口
 *   led_pins[i].pin   -> 获取第i个LED的引脚号
 * 
 * 好处：
 *   - close_all()和open_all()可以通过遍历数组，自动处理所有LED
 *   - 即使以后增加LED3在PC6，只需在数组中添加一行即可
 */
static const struct {
    GPIO_TypeDef *port;  // GPIO端口指针（指向GPIOB、GPIOE等寄存器基地址）
    uint16_t pin;        // 引脚号（如GPIO_PIN_5 = 0x0020，表示第5脚）
} led_pins[] = {
    {GPIOB, GPIO_PIN_5},  // LED1: 连接在GPIOB端口的第5脚（PB5）
    {GPIOE, GPIO_PIN_5}   // LED2: 连接在GPIOE端口的第5脚（PE5）
};

#define LED_COUNT 2  // LED总数，用于循环遍历时控制次数

/**
 * @brief 初始化LED系统
 * 
 * 【执行流程】
 * ---------------------------------------------------------------------------
 * 1. MX_GPIO_Init() - 调用CubeMX生成的GPIO初始化函数
 *    - 配置PB5和PE5为推挽输出模式
 *    - 使能GPIOB和GPIOE的时钟（RCC_APB2ENR寄存器）
 *    - 设置引脚速度、上下拉等参数
 * 
 * 2. inf_led_close_all() - 关闭所有LED
 *    - 确保上电后LED处于熄灭状态，避免乱闪
 *    - 让系统从一个已知的初始状态开始运行
 * 
 * 【为什么不需要传参数？】
 * ---------------------------------------------------------------------------
 * 因为初始化是全局操作，不需要指定具体哪个LED
 * 内部会自动处理所有LED（通过led_pins数组）
 * 
 * 使用示例：
 *   int main(void) {
 *       HAL_Init();
 *       SystemClock_Config();
 *       inf_led_init();  // 只需调用一次，无需传参
 *       // ... 后续可以使用其他LED函数
 *   }
 */
void inf_led_init(void)
{
    MX_GPIO_Init();           // 初始化GPIO配置（由CubeMX生成）
    inf_led_close_all();      // 关闭所有LED，设置初始状态为"全灭"
}

/**
 * @brief 点亮指定引脚的LED
 * 
 * @param GPIOx    GPIO端口指针（如GPIOB、GPIOE）
 * @param GPIO_Pin GPIO引脚号（如GPIO_PIN_5）
 * 
 * 【工作原理】
 * ---------------------------------------------------------------------------
 * HAL_GPIO_WritePin() 是HAL库提供的GPIO写函数，它会修改ODR寄存器：
 *   - GPIO_PIN_RESET = 0，将对应位清零，引脚输出低电平
 *   - 因为开发板LED是低电平点亮（阴极接GPIO，阳极接VCC通过限流电阻）
 *   - 所以输出低电平(0)时，LED两端有电压差，LED亮
 * 
 * 【与老师代码的区别】
 * ---------------------------------------------------------------------------
 * 老师：void inf_led_open(uint16_t led)
 *         HAL_GPIO_WritePin(GPIOA, led, GPIO_PIN_RESET);  // 固定用GPIOA
 * 
 * 我们：void inf_led_open(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
 *         HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);  // 动态指定端口
 * 
 * 原因：老师的LED都在GPIOA，可以写死；我们的LED在不同端口，必须传入端口参数
 * 
 * 【使用示例】
 * ---------------------------------------------------------------------------
 *   inf_led_open(GPIOB, LED1_Pin);  // 点亮PB5上的LED1
 *   inf_led_open(GPIOE, LED2_Pin);  // 点亮PE5上的LED2
 */
void inf_led_open(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);  // 输出低电平，LED亮
}

/**
 * @brief 关闭指定引脚的LED
 * 
 * @param GPIOx    GPIO端口指针
 * @param GPIO_Pin GPIO引脚号
 * 
 * 【工作原理】
 * ---------------------------------------------------------------------------
 *   - GPIO_PIN_SET = 1，将对应位置1，引脚输出高电平
 *   - 高电平时，LED两端都是高电平（阳极也是VCC），无电压差，LED熄灭
 * 
 * 【使用示例】
 * ---------------------------------------------------------------------------
 *   inf_led_close(GPIOB, LED1_Pin);  // 关闭PB5上的LED1
 *   inf_led_close(GPIOE, LED2_Pin);  // 关闭PE5上的LED2
 */
void inf_led_close(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);  // 输出高电平，LED灭
}

/**
 * @brief 关闭所有LED
 * 
 * 【工作原理】
 * ---------------------------------------------------------------------------
 * 通过遍历led_pins数组，对每个LED执行关闭操作。
 * 即使LED在不同GPIO端口，也能一次性全部关闭。
 * 
 * 【循环过程详解】
 * ---------------------------------------------------------------------------
 *   i=0: led_pins[0] = {GPIOB, GPIO_PIN_5}
 *        -> HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
 *        -> PB5输出高电平，LED1灭
 *   
 *   i=1: led_pins[1] = {GPIOE, GPIO_PIN_5}
 *        -> HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET)
 *        -> PE5输出高电平，LED2灭
 * 
 * 【与老师代码的对比】
 * ---------------------------------------------------------------------------
 * 老师：uint16_t leds[3] = {LED1_Pin, LED2_Pin, LED3_Pin};
 *       for (i=0; i<3; i++) 
 *           HAL_GPIO_WritePin(GPIOA, leds[i], GPIO_PIN_SET);  // 固定GPIOA
 * 
 * 我们：for (i=0; i<LED_COUNT; i++)
 *           HAL_GPIO_WritePin(led_pins[i].port, led_pins[i].pin, GPIO_PIN_SET);
 *       // 通过数组动态获取端口和引脚
 * 
 * 【应用场景】
 * ---------------------------------------------------------------------------
 *   - 初始化时确保所有LED处于已知状态（熄灭）
 *   - 程序复位或异常时快速关闭所有LED
 *   - 切换工作模式前清空指示灯状态
 */
void inf_led_close_all(void)
{
    for (uint16_t i = 0; i < LED_COUNT; i++)
    {
        // 通过数组访问每个LED的端口和引脚
        HAL_GPIO_WritePin(led_pins[i].port, led_pins[i].pin, GPIO_PIN_SET);
    }
}

/**
 * @brief 点亮所有LED
 * 
 * 【工作原理】
 * ---------------------------------------------------------------------------
 * 与close_all()相反，将所有LED设为低电平（点亮）。
 * 
 * 【循环过程】
 * ---------------------------------------------------------------------------
 *   i=0: GPIOB->PB5输出低电平，LED1亮
 *   i=1: GPIOE->PE5输出低电平，LED2亮
 * 
 * 【应用场景】
 * ---------------------------------------------------------------------------
 *   - 测试所有LED是否正常工作（上电自检）
 *   - 实现"全亮"效果（如警报状态）
 *   - 调试时确认GPIO配置是否正确
 */
void inf_led_open_all(void)
{
    for (uint16_t i = 0; i < LED_COUNT; i++)
    {
        HAL_GPIO_WritePin(led_pins[i].port, led_pins[i].pin, GPIO_PIN_RESET);
    }
}

/**
 * @brief 切换指定引脚LED的状态（亮变灭，灭变亮）
 * 
 * @param GPIOx    GPIO端口指针
 * @param GPIO_Pin GPIO引脚号
 * 
 * 【工作原理】
 * ---------------------------------------------------------------------------
 * HAL_GPIO_TogglePin() 会自动翻转引脚当前状态：
 *   - 如果当前是低电平(亮) -> 翻转为高电平(灭)
 *   - 如果当前是高电平(灭) -> 翻转为低电平(亮)
 * 
 * 内部实现逻辑（简化版）：
 *   if (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET)
 *       HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);   // 亮->灭
 *   else
 *       HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET); // 灭->亮
 * 
 * 【应用场景】
 * ---------------------------------------------------------------------------
 *   - 实现LED闪烁效果（配合定时器或延时）
 *   - 按钮控制LED开关（每次按下翻转状态）
 *   - 状态指示（如：网络连接成功/失败交替闪烁）
 * 
 * 【使用示例】
 * ---------------------------------------------------------------------------
 *   // 实现1Hz闪烁（周期1秒）
 *   while(1) {
 *       inf_led_toggle(GPIOB, LED1_Pin);  // 翻转LED1
 *       HAL_Delay(500);                    // 延时500ms
 *   }
 */
void inf_led_toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);  // 自动翻转引脚电平
}

