#ifndef __FLASHDISK_H__
#define __FLASHDISK_H__


#define MSC_MEDIUM_FLASH	0	//Æ¬ÄÚFlash
#define MSC_MEDIUM_SFLASH	1	//Æ¬ÍâSPI Flash
#define MSC_MEDIUM_SDCARD	2	//SD¿¨

#define MSC_MEDIUM  MSC_MEDIUM_SFLASH


#if (MSC_MEDIUM == MSC_MEDIUM_FLASH)

#define DATA_FLASH_BASE		(1024 * 128)
#define DATA_FLASH_SIZE		(1024 * 512 - DATA_FLASH_BASE)

#elif (MSC_MEDIUM == MSC_MEDIUM_SFLASH)

#define DATA_FLASH_BASE		(1024 * 1024 * 2)
#define DATA_FLASH_SIZE		(1024 * 1024 * 16 - DATA_FLASH_BASE)

#elif (MSC_MEDIUM == MSC_MEDIUM_SDCARD)

#define DATA_FLASH_SIZE		SD_cardInfo.CardCapacity

#else

#error "MSC_MEDIUM Setting Error"

#endif


void FlashDiskRead(uint64_t addr, uint32_t size, uint8_t *buff);
void FlashDiskWrite(uint64_t addr, uint32_t size, const uint8_t *buff);
void FlashDiskFlush(void);

#endif	// __FLASHDISK_H__
