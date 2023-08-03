#include <string.h>
#include "SWM341.h"
#include "FlashDisk.h"


#define FLASH_BLOCK_SIZE	4096
#define MIN(a, b)	(a < b) ? a : b


static uint64_t Flash_Block_Addr = 0xFFFFFFFF;
static uint32_t FLASH_Cache_Dirty = 0;
static uint8_t  FLASH_Block_Cache[FLASH_BLOCK_SIZE] __attribute__((aligned(8)));


void FlashDiskInit(void)
{
#if (MSC_MEDIUM == MSC_MEDIUM_SFLASH)
	SFC_InitStructure SFC_initStruct;
	
	/* SFC使用专用的FSPI（Flash SPI）接口连接SPI Flash */
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
	
#elif (MSC_MEDIUM  == MSC_MEDIUM_SDRAM)
	SDRAM_InitStructure SDRAM_InitStruct;

	PORT_Init(PORTM, PIN13, PORTM_PIN13_SDR_CLK, 0);
	PORT_Init(PORTM, PIN14, PORTM_PIN14_SDR_CKE, 0);
	PORT_Init(PORTB, PIN7,  PORTB_PIN7_SDR_WE,   0);
	PORT_Init(PORTB, PIN8,  PORTB_PIN8_SDR_CAS,  0);
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_SDR_RAS,  0);
	PORT_Init(PORTB, PIN10, PORTB_PIN10_SDR_CS,  0);
	PORT_Init(PORTE, PIN15, PORTE_PIN15_SDR_BA0, 0);
	PORT_Init(PORTE, PIN14, PORTE_PIN14_SDR_BA1, 0);
	PORT_Init(PORTN, PIN14, PORTN_PIN14_SDR_A0,  0);
	PORT_Init(PORTN, PIN13, PORTN_PIN13_SDR_A1,  0);
	PORT_Init(PORTN, PIN12, PORTN_PIN12_SDR_A2,  0);
	PORT_Init(PORTN, PIN11, PORTN_PIN11_SDR_A3,  0);
	PORT_Init(PORTN, PIN10, PORTN_PIN10_SDR_A4,  0);
	PORT_Init(PORTN, PIN9,  PORTN_PIN9_SDR_A5,   0);
	PORT_Init(PORTN, PIN8,  PORTN_PIN8_SDR_A6,   0);
	PORT_Init(PORTN, PIN7,  PORTN_PIN7_SDR_A7,   0);
	PORT_Init(PORTN, PIN6,  PORTN_PIN6_SDR_A8,   0);
	PORT_Init(PORTN, PIN3,  PORTN_PIN3_SDR_A9,   0);
	PORT_Init(PORTN, PIN15, PORTN_PIN15_SDR_A10, 0);
	PORT_Init(PORTN, PIN2,  PORTN_PIN2_SDR_A11,  0);
	PORT_Init(PORTM, PIN15, PORTM_PIN15_SDR_A12, 0);
	PORT_Init(PORTE, PIN7,  PORTE_PIN7_SDR_D0,   1);
	PORT_Init(PORTE, PIN6,  PORTE_PIN6_SDR_D1,   1);
	PORT_Init(PORTE, PIN5,  PORTE_PIN5_SDR_D2,   1);
	PORT_Init(PORTE, PIN4,  PORTE_PIN4_SDR_D3,   1);
	PORT_Init(PORTE, PIN3,  PORTE_PIN3_SDR_D4,   1);
	PORT_Init(PORTE, PIN2,  PORTE_PIN2_SDR_D5,   1);
	PORT_Init(PORTE, PIN1,  PORTE_PIN1_SDR_D6,   1);
	PORT_Init(PORTE, PIN0,  PORTE_PIN0_SDR_D7,   1);
	PORT_Init(PORTE, PIN8,  PORTE_PIN8_SDR_D8,   1);
	PORT_Init(PORTE, PIN9,  PORTE_PIN9_SDR_D9,   1);
	PORT_Init(PORTE, PIN10, PORTE_PIN10_SDR_D10, 1);
	PORT_Init(PORTE, PIN11, PORTE_PIN11_SDR_D11, 1);
	PORT_Init(PORTE, PIN12, PORTE_PIN12_SDR_D12, 1);
	PORT_Init(PORTE, PIN13, PORTE_PIN13_SDR_D13, 1);
	PORT_Init(PORTC, PIN14, PORTC_PIN14_SDR_D14, 1);
	PORT_Init(PORTC, PIN15, PORTC_PIN15_SDR_D15, 1);
	PORT_Init(PORTB, PIN6,  PORTB_PIN6_SDR_LDQM, 0);
	PORT_Init(PORTM, PIN12, PORTM_PIN12_SDR_UDQM,0);
	
	SDRAM_InitStruct.Size = SDRAM_SIZE_8MB;
	SDRAM_InitStruct.ClkDiv = SDRAM_CLKDIV_2;
	SDRAM_InitStruct.CASLatency = SDRAM_CASLATENCY_2;
	SDRAM_InitStruct.TimeTRP  = SDRAM_TRP_2;
	SDRAM_InitStruct.TimeTRCD = SDRAM_TRCD_2;
	SDRAM_InitStruct.TimeTRFC  = SDRAM_TRFC_7;
	SDRAM_Init(&SDRAM_InitStruct);
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
#elif (MSC_MEDIUM  == MSC_MEDIUM_SDRAM)
			memcpy(FLASH_Block_Cache, (uint8_t *)(SDRAMM_BASE + Flash_Block_Addr), FLASH_BLOCK_SIZE);
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
#elif (MSC_MEDIUM  == MSC_MEDIUM_SDRAM)
			memcpy(FLASH_Block_Cache, (uint8_t *)(SDRAMM_BASE + Flash_Block_Addr), FLASH_BLOCK_SIZE);
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
#elif (MSC_MEDIUM  == MSC_MEDIUM_SDRAM)
		memcpy((uint8_t *)(SDRAMM_BASE + Flash_Block_Addr), FLASH_Block_Cache, FLASH_BLOCK_SIZE);
#endif
		
        FLASH_Cache_Dirty = 0;
    }
}
