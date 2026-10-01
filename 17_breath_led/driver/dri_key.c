#include "dri_key.h"

/**
 * @brief 初始化按键驱动
 * 
 */
//开启GPIOE时钟
void dri_Key_Init(void){
    //开启GPIOE时钟
    RCC->APB2ENR |= RCC_APB2ENR_IOPEEN; 

    //开启AFIOE时钟
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 

    //配置PE4为上拉输入模式(MODE:00 CNF:10)
    //配置模式
    GPIOE->CRL &= ~GPIO_CRL_MODE4;
    GPIOE->CRL &= ~GPIO_CRL_CNF4_0; 
    GPIOE->CRL |= GPIO_CRL_CNF4_1;

    //选择上拉还是下拉
    GPIOE->ODR |=GPIO_ODR_ODR4;

    //配置AFIO,将PE4映射为外部中断线
    //清0
    /*
    * AFIO->EXTICR[] 数组索引对照：
    *   EXTICR[0] = EXTICR1, 偏移0x08, 管理 EXTI0~3
    *   EXTICR[1] = EXTICR2, 偏移0x0C, 管理 EXTI4~7  ← PE4 在这里
    *   EXTICR[2] = EXTICR3, 偏移0x10, 管理 EXTI8~11
    *   EXTICR[3] = EXTICR4, 偏移0x14, 管理 EXTI12~15
    * 注意：数组从0开始，EXTICR[n] 对应硬件寄存器 EXTICR(n+1)
    */
    AFIO->EXTICR[1] &= ~AFIO_EXTICR2_EXTI4;
    AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI4_PE;


    //EXTI中断配置
    //开启对应中断使能
    EXTI->IMR |= EXTI_IMR_MR4;
    //配置中断类型为下降沿触发
    EXTI->FTSR |=EXTI_FTSR_TR4;


    // 配置NIVC优先级
    //配置整个项目优先级
    NVIC_SetPriorityGrouping(3);
    //根据中断号设置优先级
    /*
    * EXTI 中断向量号（IRQn）对照：
    *   EXTI0_IRQn      = 6    ← 独立中断，引脚0
    *   EXTI1_IRQn      = 7    ← 独立中断，引脚1
    *   EXTI2_IRQn      = 8    ← 独立中断，引脚2
    *   EXTI3_IRQn      = 9    ← 独立中断，引脚3
    *   EXTI4_IRQn      = 10   ← 独立中断，引脚4  (你的PE4按钮)
    *   EXTI9_5_IRQn    = 23   ← 共用中断，引脚5~9
    *   EXTI15_10_IRQn  = 40   ← 共用中断，引脚10~15
    */
    NVIC_SetPriority(EXTI4_IRQn, 5);
    //开启中断使能
    NVIC_EnableIRQ(EXTI4_IRQn);
}




