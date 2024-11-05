/*******************************************************************************************************************************
* @brief	Flash erase/write driver
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
#include "SWM341_flash.h"


const IAP_Cache_Reset_t IAP_Cache_Reset = (IAP_Cache_Reset_t)0x11000401;
const IAP_Flash_Param_t IAP_Flash_Param = (IAP_Flash_Param_t)0x11000431;
const IAP_Flash_Erase_t IAP_Flash_Erase = (IAP_Flash_Erase_t)0x11000471;
const IAP_Flash_Write_t IAP_Flash_Write = (IAP_Flash_Write_t)0x110004C1;


/*******************************************************************************************************************************
* @brief	Flash sector erase
* @param	addr is address of sector to erase, must be 4KB aligned, i.e. addr % 4096 == 0
* @return	FLASH_RES_OK, FLASH_RES_TO, FLASH_RES_ERR
*******************************************************************************************************************************/
uint32_t FLASH_Erase(uint32_t addr)
{
	if(addr % 4096 != 0) return FLASH_RES_ERR;
	
	__disable_irq();
	
	FLASH_Unlock();
	IAP_Flash_Erase(addr/4096, 0x0B11FFAC);
	FLASH_Lock();
	
	IAP_Cache_Reset((FMC->CACHE | FMC_CACHE_CCLR_Msk), 0x0B11FFAC);	// Cache Clear
	
	__enable_irq();
	
	return FLASH_RES_OK;
}


/*******************************************************************************************************************************
* @brief	Flash data write
* @param	addr is address of Flash to write, must be 4-word aligned, i.e. addr % 16 == 0
* @param	buff is the data to write
* @param	count is the number of data to write, in unit of word, and must be a multiple of 4
* @return	FLASH_RES_OK, FLASH_RES_TO, FLASH_RES_ERR
*******************************************************************************************************************************/
uint32_t FLASH_Write(uint32_t addr, uint32_t buff[], uint32_t count)
{
	if(addr % 16 != 0) return FLASH_RES_ERR;
	if(count % 4 != 0) return FLASH_RES_ERR;
	
	__disable_irq();
	
	FLASH_Unlock();
	IAP_Flash_Write(addr, (uint32_t)buff, count/4, 0x0B11FFAC);
	FLASH_Lock();
	
	IAP_Cache_Reset((FMC->CACHE | FMC_CACHE_CCLR_Msk), 0x0B11FFAC);	// Cache Clear
	
	__enable_irq();
	
	return FLASH_RES_OK;
}


void FLASH_Lock(void)	// Flash erase & write disable
{
	__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();
	
	FMC->CFG2 = 0x00;
}

void FLASH_Unlock(void)
{
	FMC->CFG2 = 0x55FA64B4;
	
	__NOP();__NOP();__NOP();__NOP();
	__NOP();__NOP();__NOP();__NOP();
}


/*******************************************************************************************************************************
* @brief	set Flash parameter to the parameter required for running at specified MHz.
* @param	xMHz == SystemCoreClock / 1000000
* @return
*******************************************************************************************************************************/
void Flash_Param_at_xMHz(uint32_t xMHz)
{
	__disable_irq();
	
	if(xMHz <= 30)
		IAP_Flash_Param(0x8949, 0x0FA1, 0x0B11FFAC);
	else if(xMHz <= 40)
		IAP_Flash_Param(0x8A49, 0x14A9, 0x0B11FFAC);
	else if(xMHz <= 80)
		IAP_Flash_Param(0xD349, 0x294A, 0x0B11FFAC);
	else if(xMHz <= 120)
		IAP_Flash_Param(0x11C89, 0x3D6B, 0x0B11FFAC);
	else
		IAP_Flash_Param(0x16589, 0x4C74, 0x0B11FFAC);
	
	__enable_irq();
}
