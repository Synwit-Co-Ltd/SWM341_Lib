#ifndef __FLASHDISK_H__
#define __FLASHDISK_H__


#define MSC_MEDIUM_FLASH	0	// on-chip Flash
#define MSC_MEDIUM_SFLASH	1	// off-chip SPI Flash
#define MSC_MEDIUM_SDCARD	2	// sd card
#define MSC_MEDIUM_SDRAM	3	// SDRAM, used to test the transmission speed of the USB interface

#define MSC_MEDIUM  MSC_MEDIUM_SFLASH


#if (MSC_MEDIUM == MSC_MEDIUM_FLASH)

#define DATA_FLASH_BASE		(1024 * 128)
#define DATA_FLASH_SIZE		(1024 * 512 - DATA_FLASH_BASE)

#elif (MSC_MEDIUM == MSC_MEDIUM_SFLASH)

#define DATA_FLASH_BASE		(1024 * 1024 * 2)
#define DATA_FLASH_SIZE		(1024 * 1024 * 16 - DATA_FLASH_BASE)

/* 0: use 4KB Sector Erase command
 * 1: use 64KB Block Erase command. When large amounts of data are written, the average erase time is shorter and the overall write speed is faster.
 *	  However, 64KB Buffer is required to store the data of the block to be erased, so it can only be used with SDRAM
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

#endif
