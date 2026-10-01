#ifndef __INF_FLASH_H__
#define __INF_FLASH_H__

#include "dri_spi.h"

/**
 * @brief flash初始化
 */
void inf_flash_init(void);

/**
 * @brief 开启片选
 */
void inf_flash_cs_on(void);

/**
 * @brief 关闭片选
 */
void inf_flash_cs_off(void);

/**
 * @brief 开启写使能
 */
void inf_flash_write_enable(void);

/**
 * @brief 关闭写使能
 */
void inf_flash_write_disable(void);

/**
 * @brief 等待芯片空闲
 */
void inf_fash_wait_busy(void);

/**
 * @brief 获取设备信息
 */
void inf_flash_chip_id(uint8_t *mid, uint16_t *id);

/**
 * @brief 扇区擦除
 * block_addr : 00-3F
 * sector_addr: 00-0F
 */
void inf_flash_erase_sector(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr);

/**
 * @brief 页写
 * block_addr : 00-3F
 * sector_addr : 0-F
 * page_addr :0-F
 * data:数据
 * size:数据长度
 */
void inf_flash_write(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *data, uint16_t size);

/**
 * @brief 读
 *
 */
void inf_flash_read(uint8_t block_addr, uint8_t sector_addr, uint8_t page_addr, uint8_t *data, uint16_t size);

#endif /* __INF_FLASH_H__ */
