#include "SWM341.h"

#include "GD25Q21.h"


/*******************************************************************************************************************************
* @brief	init the SPI used to access SPI Flash, and read chip ID to determine whether it is a GD25Q21 chip
* @param
* @return
*******************************************************************************************************************************/
void GD25Q21_Init(void)
{
	uint16_t chip_id;
	uint8_t manufacture_id, device_id;
	SPI_InitStructure SPI_initStruct;
	
	GPIO_Init(GPIOM, PIN3, 1, 0, 0, 0);
#define GD25Q21_Assert()	GPIO_ClrBit(GPIOM, PIN3)
#define GD25Q21_Deassert()	GPIO_SetBit(GPIOM, PIN3)
	GD25Q21_Deassert();
	
	PORT_Init(PORTM, PIN2, PORTM_PIN2_SPI0_SCLK, 0);
	PORT_Init(PORTM, PIN4, PORTM_PIN4_SPI0_MISO, 1);
	PORT_Init(PORTM, PIN5, PORTM_PIN5_SPI0_MOSI, 0);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_32;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_FIRST_EDGE;
	SPI_initStruct.IdleLevel = SPI_LOW_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThreshold = 0;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold = 0;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	SPI_Open(SPI0);
	
	chip_id = GD25Q21_ReadChipID();
	manufacture_id = chip_id >> 8;
	device_id = chip_id & 0xFF;
	
	if((manufacture_id == GD25Q21_MANUFACTOR_ID) && (device_id == GD25Q21_DEVICE_ID))
	{
		printf("GD25Q21 found\r\n");
		
		while(GD25Q21_IsBusy()) __NOP();
	}
	else
	{
		printf("Manufactor ID: %02X, Device ID: %02X, fail to identify\r\n", manufacture_id, device_id);
	}
}

/*******************************************************************************************************************************
* @brief	read chip id
* @param
* @return	chip id, the high 8 bits are the manufacturer ID and the low 8 bits are the Device ID
*******************************************************************************************************************************/
uint16_t GD25Q21_ReadChipID(void)
{
	uint8_t manufacture_id, device_id;
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_READ_CHIP_ID);
	SPI_ReadWrite(SPI0, 0x00);
	SPI_ReadWrite(SPI0, 0x00);
	SPI_ReadWrite(SPI0, 0x00);
	
	manufacture_id = SPI_ReadWrite(SPI0, 0xFF);
	device_id      = SPI_ReadWrite(SPI0, 0xFF);
	
	GD25Q21_Deassert();
	
	return (manufacture_id << 8) | device_id;
}

/*******************************************************************************************************************************
* @brief	read status register from SPI Flash
* @param
* @return	status register value
*******************************************************************************************************************************/
uint16_t GD25Q21_ReadStatus(void)
{
	uint16_t status;
	
	GD25Q21_Assert();
		
	SPI_ReadWrite(SPI0, GD25Q21_CMD_READ_STATUS_L);
	status = SPI_ReadWrite(SPI0, 0xFF);
	
	GD25Q21_Deassert();
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_READ_STATUS_H);
	status |= SPI_ReadWrite(SPI0, 0xFF) << 8;
	
	GD25Q21_Deassert();
		
	return status;
}

/*******************************************************************************************************************************
* @brief	GD25Q21 busy query
* @param
* @return	1 busy for internal operation, 0 idle, operation done
*******************************************************************************************************************************/
uint32_t GD25Q21_IsBusy(void)
{
	return (GD25Q21_ReadStatus() & GD25Q21_STATUS_WIP_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	GD25Q21 sector erase, 4KB per sector
* @param	addr is the address of the sector to erase
* @return
*******************************************************************************************************************************/
void GD25Q21_EraseSector(uint32_t addr)
{
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_WRITE_ENABLE);
	
	GD25Q21_Deassert();
	
	GD25Q21_Assert();
		
	SPI_ReadWrite(SPI0, GD25Q21_CMD_ERASE_SECTOR);
	SPI_ReadWrite(SPI0, (addr >> 16) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  8) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  0) & 0xFF);
	
	GD25Q21_Deassert();
}

/*******************************************************************************************************************************
* @brief	GD25Q21 data read
* @param	addr is the address to read from
* @param	buff is used to save read data
* @param	cnt is the number of byte to read
* @return
*******************************************************************************************************************************/
void GD25Q21_ReadData(uint32_t addr, uint8_t buff[], uint32_t cnt)
{
	uint32_t i;
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_READ_DATA);
	SPI_ReadWrite(SPI0, (addr >> 16) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  8) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  0) & 0xFF);
	
	for(i = 0; i < cnt; i++)
	{
		buff[i] = SPI_ReadWrite(SPI0, 0xFF);
	}
	
	GD25Q21_Deassert();
}

/*******************************************************************************************************************************
* @brief	GD25Q21 data write, 256 bytes per page
* @param	addr is the address to write to
* @param	data is the data to write
* @param	cnt is the number of byte to write, all data must be within the address range of the same page
* @return
*******************************************************************************************************************************/
void GD25Q21_WritePage(uint32_t addr, uint8_t data[], uint16_t cnt)
{
	uint32_t i;
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_WRITE_ENABLE);
	
	GD25Q21_Deassert();
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_WRITE_PAGE);
	SPI_ReadWrite(SPI0, (addr >> 16) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  8) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  0) & 0xFF);
	
	for(i = 0; i < cnt; i++)
	{
		SPI_ReadWrite(SPI0, data[i]);
	}
	
	GD25Q21_Deassert();
}
