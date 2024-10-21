/*******************************************************************************************************************************
* @brief	SFC driver, Serial Flash Controller, used to read/write SPI Flash
*
*
********************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology 
*******************************************************************************************************************************/
#include "SWM341.h"
#include "SWM341_sfc.h"


/*******************************************************************************************************************************
* @brief	SFC init
* @param	initStruct is data used to init the SFC
* @return
*******************************************************************************************************************************/
void SFC_Init(SFC_InitStructure * initStruct)
{
	SYS->CLKEN1 |= (1 << SYS_CLKEN1_SFC_Pos);
	
	*((__IO uint32_t *)((uint32_t )&SFC->CFG + 0x3F4)) = 7;
	
	SFC->CFG &= ~(SFC_CFG_CLKDIV_Msk | SFC_CFG_DATA4L_RD_Msk | SFC_CFG_DATA4L_PP_Msk);
	SFC->CFG |= (initStruct->ClkDiv << SFC_CFG_CLKDIV_Pos) |
				(initStruct->Width_Read << SFC_CFG_DATA4L_RD_Pos) |
				(initStruct->Width_PageProgram << SFC_CFG_DATA4L_PP_Pos);
	
	SFC->CFG |= (1 << SFC_CFG_CMDWREN_Pos);
	SFC->CMDAHB &= ~(SFC_CMDAHB_READ_Msk | SFC_CMDAHB_PP_Msk);
	SFC->CMDAHB |= (initStruct->Cmd_Read << SFC_CMDAHB_READ_Pos) |
				   (initStruct->Cmd_PageProgram << SFC_CMDAHB_PP_Pos);
	SFC->CFG &= ~(1 << SFC_CFG_CMDWREN_Pos);
	
	SFC->TIM &= ~(SFC_TIM_WIP_CHK_ITV_Msk | SFC_TIM_WIP_CHK_LMT_Msk);
	SFC->TIM |= ((CyclesPerUs / 10) << SFC_TIM_WIP_CHK_ITV_Pos) |	// 2048 * (CyclesPerUs / 10) / CyclesPerUs us = 0.2 ms
				(255 << SFC_TIM_WIP_CHK_LMT_Pos);
	
	if((initStruct->Width_Read == SFC_RDWIDTH_4) || (initStruct->Width_PageProgram == SFC_PPWIDTH_4))
	{
		if(SFC_QuadState() == 0)
			SFC_QuadSwitch(1);
	}
}

/*******************************************************************************************************************************
* @brief	SFC read SPI Flash JEDEC ID
* @param
* @return	JEDEC ID
*******************************************************************************************************************************/
uint32_t SFC_ReadJEDEC(void)
{
	SFC->CFG &= ~SFC_CFG_CMDTYPE_Msk;
	SFC->CFG |= (1 << SFC_CFG_CMDWREN_Pos) |
				(2 << SFC_CFG_CMDTYPE_Pos);
	SFC->CMD = SFC_CMD_READ_JEDEC;
	
	SFC->GO = 1;
	__DSB(); __ISB();
	while(SFC->GO) __NOP();
	
	return SFC->DATA;
}

/*******************************************************************************************************************************
* @brief	SFC erase SPI Flash sector, 4K bytes per sector
* @param	addr is sector address to erase, must be 4K aligned, i.e. addr % 4096 == 0
* @param	wait = 1: wait for Flash erase operation done, wait = 0: no wait, immediately return
* @return
*******************************************************************************************************************************/
void SFC_Erase(uint32_t addr, uint8_t wait)
{
	SFC_EraseEx(addr, SFC_CMD_ERASE_SECTOR, wait);
}

/*******************************************************************************************************************************
* @brief	SFC erase SPI Flash sector, block, or whole chip
* @param	addr is sector/block address to erase, whole chip when addr == 0xFFFFFFFF
* @param	cmd is SPI Flash erase command opcode, e.g. SFC_CMD_ERASE_SECTOR, SFC_CMD_ERASE_BLOCK64KB, and so on
* @param	wait = 1: wait for Flash erase operation done, wait = 0: no wait, immediately return
* @return
*******************************************************************************************************************************/
void SFC_EraseEx(uint32_t addr, uint8_t cmd, uint8_t wait)
{
	uint8_t type = (addr == 0xFFFFFFFF) ? 5 : 7;
	
	SFC->ADDR = addr;
	
	SFC->CFG &= ~SFC_CFG_CMDTYPE_Msk;
	SFC->CFG |= (1 << SFC_CFG_WREN_Pos) |
				(1 << SFC_CFG_CMDWREN_Pos) |
				(type << SFC_CFG_CMDTYPE_Pos);
	SFC->CMD = cmd;
	
	SFC->GO = 1;
	__DSB(); __ISB();
	while(SFC->GO) __NOP();
	
	SFC->CFG &= ~SFC_CFG_WREN_Msk;
	
	if(wait)
		while(SFC_FlashBusy()) __NOP();
}

/*******************************************************************************************************************************
* @brief	SFC write data to SPI Flash
* @param	addr is address to write to, must be word aligned
* @param	buff is the data to write
* @param	cnt is the number of word to write, up to 64, and all data must be in same page, i.e. addr/256 == (addr+(cnt-1)*4)/256
* @return
*******************************************************************************************************************************/
void SFC_Write(uint32_t addr, uint32_t buff[], uint32_t cnt)
{
	SFC->CFG |= (1 << SFC_CFG_WREN_Pos);
	for(int i = 0; i < cnt; i++)
		*((volatile unsigned int *)(0x70000000+addr+i*4)) = buff[i];
	
	while(SFC->SR & SFC_SR_BUSY_Msk) __NOP();
	SFC->CFG &= ~SFC_CFG_WREN_Msk;
}


#define IOSPI_CS_Low()		GPIO_ClrBit(GPIOD, PIN6); __NOP(); __NOP(); __NOP(); __NOP()
#define IOSPI_CS_High()		__NOP(); __NOP(); __NOP(); __NOP(); GPIO_SetBit(GPIOD, PIN6)
#define IOSPI_CLK_Low()		GPIO_ClrBit(GPIOD, PIN5); __NOP(); __NOP()
#define IOSPI_CLK_High()	__NOP(); __NOP(); GPIO_SetBit(GPIOD, PIN5)
#define IOSPI_MOSI_Low()	GPIO_ClrBit(GPIOD, PIN8)
#define IOSPI_MOSI_High()	GPIO_SetBit(GPIOD, PIN8)
#define IOSPI_MISO_Value()	GPIO_GetBit(GPIOD, PIN7)

static uint8_t IOSPI_ReadWrite(uint8_t data)
{
	uint8_t val = 0;
	
	for(int i = 0; i < 8; i++)
	{
		IOSPI_CLK_Low();
		
		if(data & (1 << (7 - i)))
			IOSPI_MOSI_High();
		else
			IOSPI_MOSI_Low();
		
		IOSPI_CLK_High();
		
		val = (val << 1) | IOSPI_MISO_Value();
	}
	
	return val;
}

/*******************************************************************************************************************************
* @brief	write data to SPI Flash throught simulated SPI by GPIO, faster than SFC_Write() when write large amount of data
* @param	addr is address to write to, must be word aligned
* @param	buff is the data to write
* @param	cnt is the number of word to write, up to 64, and all data must be in same page, i.e. addr/256 == (addr+(cnt-1)*4)/256
* @return
* @note		before and after calling this function, you need to configure the corresponding pin to correct mode
*******************************************************************************************************************************/
void SFC_GPIOWrite(uint32_t addr, uint32_t buff[], uint32_t cnt)
{
	IOSPI_CS_Low();
	IOSPI_ReadWrite(SFC_CMD_WRITE_ENABLE);
	IOSPI_CS_High();
	
	IOSPI_CS_Low();
	IOSPI_ReadWrite(SFC_CMD_PAGE_PROGRAM);
	IOSPI_ReadWrite(addr >> 16);
	IOSPI_ReadWrite(addr >>  8);
	IOSPI_ReadWrite(addr);
	
	for(int i = 0; i < cnt * 4; i++)
	{
		IOSPI_ReadWrite(((uint8_t *)buff)[i]);
	}
	IOSPI_CS_High();
	
	int busy;
	do {
		IOSPI_CS_Low();
		IOSPI_ReadWrite(SFC_CMD_READ_STATUS_REG1);
		busy = IOSPI_ReadWrite(0xFF) & (1 << SFC_STATUS_REG_BUSY_Pos);
		IOSPI_CS_High();
	} while(busy);
}


/*******************************************************************************************************************************
* @brief	read SPI Flash UID throught simulated SPI by GPIO
* @param	uid is used to save the read UID
* @param	cnt is the length of UID in byte
* @return
* @note		before and after calling this function, you need to configure the corresponding pin to correct mode
*******************************************************************************************************************************/
void SFC_GPIOReadUID(uint8_t uid[], uint32_t cnt)
{
	IOSPI_CS_Low();
	IOSPI_ReadWrite(SFC_CMD_READ_UID);
	for(int i = 0; i < 4; i++)
		IOSPI_ReadWrite(0x00);	// dummy byte
	for(int i = 0; i < cnt; i++)
		uid[i] = IOSPI_ReadWrite(0x00);
	IOSPI_CS_High();
}


/*******************************************************************************************************************************
* @brief	SFC read data from SPI Flash
* @param	addr is address to read from, must be word aligned
* @param	buff is used to save the read data
* @param	cnt is the number of word to read
* @return
*******************************************************************************************************************************/
void SFC_Read(uint32_t addr, uint32_t buff[], uint32_t cnt)
{
	for(int i = 0; i < cnt; i++)
		buff[i] = *((volatile unsigned int *)(0x70000000+addr+i*4));
}

/*******************************************************************************************************************************
* @brief	SFC read SPI Flash status register
* @param	cmd is command to be sent to SPI Flash to read status register
* @return	value of status register
*******************************************************************************************************************************/
uint8_t SFC_ReadStatusReg(uint8_t cmd)
{
	SFC->CFG &= ~SFC_CFG_CMDTYPE_Msk;
	SFC->CFG |= (1 << SFC_CFG_CMDWREN_Pos) |
				(1 << SFC_CFG_CMDTYPE_Pos);
	SFC->CMD = cmd;
	
	SFC->GO = 1;
	__DSB(); __ISB();
	while(SFC->GO) __NOP();
	
	return SFC->DATA;
}

/*******************************************************************************************************************************
* @brief	SFC write value to SPI Flash status register
* @param	cmd is command to be sent to SPI Flash to write status register
* @param	val is value to be written to status register
* @return
*******************************************************************************************************************************/
void SFC_WriteStatusReg(uint8_t cmd, uint16_t val)
{
	SFC->CFG &= ~SFC_CFG_CMDTYPE_Msk;
	SFC->CFG |= (1 << SFC_CFG_WREN_Pos) |
				(1 << SFC_CFG_CMDWREN_Pos) |
				(6 << SFC_CFG_CMDTYPE_Pos);
	SFC->CMD = cmd;
	
	SFC->DATA = val;
	
	SFC->GO = 1;
	__DSB(); __ISB();
	while(SFC->GO) __NOP();
}

/*******************************************************************************************************************************
* @brief	SFC set SPI Flash Quad Enable bit
* @param	on is value to write to SPI Flash Quad Enable bit
* @return
*******************************************************************************************************************************/
void SFC_QuadSwitch(uint8_t on)
{
	uint16_t reg = (SFC_ReadStatusReg(SFC_CMD_READ_STATUS_REG2) << 8) | 
				   (SFC_ReadStatusReg(SFC_CMD_READ_STATUS_REG1) << 0);
	
	if(on)
		reg |= (1 << SFC_STATUS_REG_QUAD_Pos);
	else
		reg &=~(1 << SFC_STATUS_REG_QUAD_Pos);
	
	SFC_WriteStatusReg(SFC_CMD_WRITE_STATUS_REG1, reg);
}

/*******************************************************************************************************************************
* @brief	SFC query SPI Flash Quad Enable bit
* @param
* @return	SPI Flash Quad Enable bit value
*******************************************************************************************************************************/
uint8_t SFC_QuadState(void)
{
	uint8_t reg = SFC_ReadStatusReg(SFC_CMD_READ_STATUS_REG2);
	
	if(reg & (1 << (SFC_STATUS_REG_QUAD_Pos - 8)))
		return 1;
	else
		return 0;
}

/*******************************************************************************************************************************
* @brief	SFC query SPI Flash busy state
* @param
* @return	1 SPI Flash busy in erase/write operation, 0 SPI Flash idle
*******************************************************************************************************************************/
uint8_t SFC_FlashBusy(void)
{
	uint8_t reg = SFC_ReadStatusReg(SFC_CMD_READ_STATUS_REG1);
	
	if(reg & (1 << SFC_STATUS_REG_BUSY_Pos))
		return 1;
	else
		return 0;
}
