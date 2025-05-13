#include <string.h>
#include "SWM341.h"
#include "FlashDisk.h"


#define FLASH_BLOCK_SIZE	(1024 * 4)
#define MIN(a, b)	(a < b) ? a : b


static uint64_t Flash_Block_Addr = 0xFFFFFFFF;
static uint32_t FLASH_Cache_Dirty = 0;
static uint8_t  FLASH_Block_Cache[FLASH_BLOCK_SIZE] __attribute__((aligned(8)));

void FlashDiskInit(void)
{
#if (MSC_MEDIUM == MSC_MEDIUM_SFLASH)
	SFC_InitStructure SFC_initStruct;
	
	PORT_Init(PORTD, PIN5, PORTD_PIN5_FSPI_SCLK,  0);
	PORT_Init(PORTD, PIN6, PORTD_PIN6_FSPI_SSEL,  0);
	PORT_Init(PORTD, PIN8, PORTD_PIN8_FSPI_MOSI,  1);
	PORT_Init(PORTD, PIN7, PORTD_PIN7_FSPI_MISO,  1);
	PORT_Init(PORTD, PIN3, PORTD_PIN3_FSPI_DATA2, 1);
	PORT_Init(PORTD, PIN4, PORTD_PIN4_FSPI_DATA3, 1);
	
	SFC_initStruct.ClkDiv = SFC_CLKDIV_4;
	SFC_initStruct.Cmd_Read = 0xEB;
	SFC_initStruct.Width_Read = SFC_RDWIDTH_4;
	SFC_initStruct.Cmd_PageProgram = 0x32;
	SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_4;
	SFC_Init(&SFC_initStruct);

#elif (MSC_MEDIUM == MSC_MEDIUM_SDCARD)
	PORT_Init(PORTM, PIN2, PORTM_PIN2_SD_CLK, 0);
	PORT_Init(PORTM, PIN4, PORTM_PIN4_SD_CMD, 1);
	PORT_Init(PORTM, PIN5, PORTM_PIN5_SD_D0,  1);
	PORT_Init(PORTM, PIN6, PORTM_PIN6_SD_D1,  1);
	PORT_Init(PORTN, PIN0, PORTN_PIN0_SD_D2,  1);
	PORT_Init(PORTN, PIN1, PORTN_PIN1_SD_D3,  1);
	
	SDIO_Init(20000000);
#endif
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
		uint32_t in_block_size = MIN(FLASH_BLOCK_SIZE - in_block_offset, size);
		
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
		uint32_t in_block_size = MIN(FLASH_BLOCK_SIZE - in_block_offset, size);
		
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
