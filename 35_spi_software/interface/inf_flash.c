#include "inf_flash.h"

/**
 * @brief flash初始化
 */
void inf_flash_init(void)
{
    dri_spi_init();
}

/**
 * @brief 开启片选
 */
void inf_flash_cs_on(void)
{
    GPIOC->ODR &= ~GPIO_ODR_ODR13;
}

/**
 * @brief 关闭片选
 */
void inf_flash_cs_off(void)
{
    GPIOC->ODR |= GPIO_ODR_ODR13;
}

/**
 * @brief 等待芯片空闲
 */
void inf_fash_wait_busy(void)
{
    // 1. 片选
    inf_flash_cs_on();

    // 2. 循环判断是否忙
    while (1)
    {
        // 3. 发送指令05 读取状态寄存器1 的值
        dri_spi_swap(0x05);

        // 4. 接收芯片给我的寄存器的数据
        uint8_t status = dri_spi_swap(0xFF);

        // 5. 判断
        if (!(status & 0x01))
        {
            // 一旦busy位为0 则直接退出函数
            break;
        }
    }

    // 6. 关闭片选
    inf_flash_cs_off();
}

/**
 * @brief 开启写使能
 */
void inf_flash_write_enable(void)
{
    // 1. 等待空闲
    inf_fash_wait_busy();

    // 2. 片选
    inf_flash_cs_on();

    // 3. 发送指令06 开启写使能
    dri_spi_swap(0x06);

    // 4. 关闭片选
    inf_flash_cs_off();
}

/**
 * @brief 关闭写使能
 */
void inf_flash_write_disable(void)
{
    // 1. 等待空闲
    inf_fash_wait_busy();

    // 2. 片选
    inf_flash_cs_on();

    // 3. 发送指令06 开启写使能
    dri_spi_swap(0x04);

    // 4. 关闭片选
    inf_flash_cs_off();
}

/**
 * @brief 获取设备信息
 */
void inf_flash_chip_id(uint8_t *mid, uint16_t *id)
{
    // 1. 等待空闲
    inf_fash_wait_busy();
    // 2. 片选
    inf_flash_cs_on();

    // 3. 发送指令9F
    dri_spi_swap(0x9F);

    // 4. 接收
    *mid = dri_spi_swap(0xFF);
    *id |= dri_spi_swap(0xFF) << 8;
    *id |= dri_spi_swap(0xFF) << 0;

    // 4. 关闭片选
    inf_flash_cs_off();
}

/**
 * @brief 发送地址
 */
static void inf_flash_send_addr(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr)
{
    uint32_t addr = block_addr << 16 | sector_addr << 12 | page_addr << 8;

    // 发送地址
    dri_spi_swap((uint8_t)(addr >> 16));
    dri_spi_swap((uint8_t)(addr >> 8));
    dri_spi_swap((uint8_t)(addr >> 0));
}

/**
 * @brief 扇区擦除
 * block_addr : 00-3F
 * sector_addr: 00-0F
 */
void inf_flash_erase_sector(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr)
{
    // 1. 等待空闲
    inf_fash_wait_busy();

    // 2. 开启写使能
    inf_flash_write_enable();

    // 3. 开启片选
    inf_flash_cs_on();

    // 4. 发送指令
    dri_spi_swap(0x20);

    // 5. 发送地址
    inf_flash_send_addr(block_addr, sector_addr, page_addr);

    // 6. 关闭片选
    inf_flash_cs_off();

    // 7. 等待擦除完成(文档明确说明,擦除完成以后,会关闭写使能)
    inf_fash_wait_busy();

    // 7. 关闭写使能(不需要 写了也没错)
    // inf_flash_write_disable();
}

/**
 * @brief 页写
 * block_addr : 00-3F
 * sector_addr : 0-F
 * page_addr :0-F
 * data:数据
 * size:数据长度
 */
void inf_flash_write(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *data, uint16_t size)
{
    // 1. 等待空闲
    inf_fash_wait_busy();

    // 2. 开启写使能
    inf_flash_write_enable();

    // 3. 开启片选
    inf_flash_cs_on();

    // 4. 发送指令
    dri_spi_swap(0x02);

    // 5. 发送地址
    inf_flash_send_addr(block_addr, sector_addr, page_addr);

    // 6. 发送数据
    for (uint8_t i = 0; i < size; i++)
    {
        dri_spi_swap(data[i]);
    }

    // 7. 拉高片选(一旦拉高片选,芯片就知道你写完了所有的数据,就要开始真正写入了)
    inf_flash_cs_off();

    // 8. 等待写入完成(文档明确说明,写入完成以后,会关闭写使能)
    inf_fash_wait_busy();
}

/**
 * @brief 读
 * block_addr : 00-3F
 * sector_addr : 0-F
 * page_addr :0-F
 * data:数据
 * size:数据长度
 */
void inf_flash_read(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *data, uint16_t size)
{
    // 1. 等待空闲
    inf_fash_wait_busy();

    // 2. 开启片选
    inf_flash_cs_on();

    // 3. 发送指令
    dri_spi_swap(0x03);

    // 4.发送地址
    inf_flash_send_addr(block_addr, sector_addr, page_addr);

    // 5. 接收数据
    for (uint16_t i = 0; i < size; i++)
    {
        data[i] = dri_spi_swap(0xFF);
    }

    // 6. 关闭片选
    inf_flash_cs_off();
}