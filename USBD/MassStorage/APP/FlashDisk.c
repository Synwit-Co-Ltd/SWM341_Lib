#include <string.h>
#include "SWM341.h"
#include "FlashDisk.h"


#define FLASH_BLOCK_SIZE	4096


static uint64_t Flash_Block_Addr = 0xFFFFFFFF;
static uint32_t FLASH_Cache_Dirty = 0;
static uint8_t  FLASH_Block_Cache[FLASH_BLOCK_SIZE] __attribute__((aligned(8)));

static __INLINE uint32_t min(uint32_t a, uint32_t b)
{
	return (a < b) ? a : b;
}


void FlashDiskRead(uint64_t addr, uint32_t size, uint8_t *buff)
{
    uint64_t block_addr_start = addr / FLASH_BLOCK_SIZE * FLASH_BLOCK_SIZE;
	uint64_t block_addr_end = (addr + size) / FLASH_BLOCK_SIZE * FLASH_BLOCK_SIZE;
	
	for(uint64_t block_addr = block_addr_start; block_addr <= block_addr_end; block_addr += FLASH_BLOCK_SIZE)
	{
		if(Flash_Block_Addr != block_addr)
		{
			FlashDiskFlush();
			
			Flash_Block_Addr = block_addr;
			
#if (MSC_MEDIUM == MSC_MEDIUM_FLASH)
			memcpy(FLASH_Block_Cache, (uint8_t *)(DATA_FLASH_BASE + Flash_Block_Addr), FLASH_BLOCK_SIZE);
#elif (MSC_MEDIUM == MSC_MEDIUM_SFLASH)
			SFC_Read(DATA_FLASH_BASE + Flash_Block_Addr, (uint32_t *)FLASH_Block_Cache, FLASH_BLOCK_SIZE/4);
#elif (MSC_MEDIUM == MSC_MEDIUM_SDCARD)
			SDIO_MultiBlockRead(Flash_Block_Addr/512, FLASH_BLOCK_SIZE/512, (uint32_t *)FLASH_Block_Cache);
#endif
		}
		
		uint32_t in_block_offset = addr - Flash_Block_Addr;
		uint32_t in_block_size = min(FLASH_BLOCK_SIZE - in_block_offset, size);
		
		memcpy(buff, &FLASH_Block_Cache[in_block_offset], in_block_size);
		buff += in_block_size;
		addr += in_block_size;
		size -= in_block_size;
		
		if(size == 0) break;
	}
}

void FlashDiskWrite(uint64_t addr, uint32_t size, const uint8_t *buff)
{
	uint64_t block_addr_start = addr / FLASH_BLOCK_SIZE * FLASH_BLOCK_SIZE;
	uint64_t block_addr_end = (addr + size) / FLASH_BLOCK_SIZE * FLASH_BLOCK_SIZE;
		
	for(uint64_t block_addr = block_addr_start; block_addr <= block_addr_end; block_addr += FLASH_BLOCK_SIZE)
	{
		if(Flash_Block_Addr != block_addr)
		{
			FlashDiskFlush();
			
			Flash_Block_Addr = block_addr;
			
#if (MSC_MEDIUM == MSC_MEDIUM_FLASH)
			memcpy(FLASH_Block_Cache, (uint8_t *)(DATA_FLASH_BASE + Flash_Block_Addr), FLASH_BLOCK_SIZE);
#elif (MSC_MEDIUM == MSC_MEDIUM_SFLASH)
			SFC_Read(DATA_FLASH_BASE + Flash_Block_Addr, (uint32_t *)FLASH_Block_Cache, FLASH_BLOCK_SIZE/4);
#elif (MSC_MEDIUM == MSC_MEDIUM_SDCARD)
			SDIO_MultiBlockRead(Flash_Block_Addr/512, FLASH_BLOCK_SIZE/512, (uint32_t *)FLASH_Block_Cache);
#endif
		}
		
		uint32_t in_block_offset = addr - Flash_Block_Addr;
		uint32_t in_block_size = min(FLASH_BLOCK_SIZE - in_block_offset, size);
		
		memcpy(&FLASH_Block_Cache[in_block_offset], buff, in_block_size);
		buff += in_block_size;
		addr += in_block_size;
		size -= in_block_size;
		
		FLASH_Cache_Dirty = 1;
		
		if(size == 0) break;
	}
}

void FlashDiskFlush(void)
{	
    // Write back the cache if it's dirty
    if(FLASH_Cache_Dirty)
    {
#if (MSC_MEDIUM == MSC_MEDIUM_FLASH)
		FLASH_Erase(DATA_FLASH_BASE + Flash_Block_Addr);
		
		FLASH_Write(DATA_FLASH_BASE + Flash_Block_Addr, (uint32_t *)FLASH_Block_Cache, FLASH_BLOCK_SIZE/4);
#elif (MSC_MEDIUM == MSC_MEDIUM_SFLASH)
        SFC_Erase(DATA_FLASH_BASE + Flash_Block_Addr, 1);
		
		for(int i = 0; i < FLASH_BLOCK_SIZE; i += 256)
			SFC_Write(DATA_FLASH_BASE + Flash_Block_Addr + i, (uint32_t *)&FLASH_Block_Cache[i], 256/4);
#elif (MSC_MEDIUM == MSC_MEDIUM_SDCARD)
		SDIO_MultiBlockWrite(Flash_Block_Addr/512, FLASH_BLOCK_SIZE/512, (uint32_t *)FLASH_Block_Cache);
#endif
		
        FLASH_Cache_Dirty = 0;
    }
}
