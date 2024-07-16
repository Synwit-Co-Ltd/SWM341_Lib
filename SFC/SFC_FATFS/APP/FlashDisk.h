#ifndef __FLASHDISK_H__
#define __FLASHDISK_H__


#define FLASH_SECTOR_SIZE	512
#define FLASH_BLOCK_SIZE	4096

#define DATA_FLASH_BASE		(1024 * 1024 *  2)
#define DATA_FLASH_SIZE		(1024 * 1024 * 16 - DATA_FLASH_BASE)


void FlashDiskRead(uint32_t addr, uint32_t size, uint8_t *buff);
void FlashDiskWrite(uint32_t addr, uint32_t size, const uint8_t *buff);
void FlashDiskFlush(void);


#endif
