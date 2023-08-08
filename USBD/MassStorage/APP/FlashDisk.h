#ifndef __FLASHDISK_H__
#define __FLASHDISK_H__


#define MSC_MEDIUM_FLASH	0	//片内Flash
#define MSC_MEDIUM_SFLASH	1	//片外SPI Flash
#define MSC_MEDIUM_SDCARD	2	//SD卡
#define MSC_MEDIUM_SDRAM	3	//SDRAM，用于测试 USB 接口传输速度

#define MSC_MEDIUM  MSC_MEDIUM_SFLASH


#if (MSC_MEDIUM == MSC_MEDIUM_FLASH)

#define DATA_FLASH_BASE		(1024 * 128)
#define DATA_FLASH_SIZE		(1024 * 512 - DATA_FLASH_BASE)

#elif (MSC_MEDIUM == MSC_MEDIUM_SFLASH)

#define DATA_FLASH_BASE		(1024 * 1024 * 2)
#define DATA_FLASH_SIZE		(1024 * 1024 * 16 - DATA_FLASH_BASE)

/* 0：使用 4KB Sector Erase 命令
 * 1：使用 64KB Block Erase 命令，大数据量写入时，平均擦除时间更短，整体写入速度更快
 *	  但需要 64KB Buffer 存储待擦除 Block 的数据，因此只能用于有 SDRAM 的情况
 */
#define SFLASH_ERASE_64KB	0

#elif (MSC_MEDIUM == MSC_MEDIUM_SDCARD)

#define DATA_FLASH_SIZE		SD_cardInfo.CardCapacity

#elif (MSC_MEDIUM  == MSC_MEDIUM_SDRAM)

#define DATA_FLASH_SIZE		(1024 * 1024 * 8)

#else

#error "MSC_MEDIUM Setting Error"

#endif


void FlashDiskInit(void);
void FlashDiskRead(uint64_t addr, uint32_t size, uint8_t *buff);
void FlashDiskWrite(uint64_t addr, uint32_t size, const uint8_t *buff);
void FlashDiskFlush(void);

#endif	// __FLASHDISK_H__
