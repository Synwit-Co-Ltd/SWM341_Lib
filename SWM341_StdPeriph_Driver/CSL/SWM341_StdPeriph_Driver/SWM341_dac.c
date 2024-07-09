/*******************************************************************************************************************************
* @brief	DAC driver
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
#include "SWM341_dac.h"


/*******************************************************************************************************************************
* @brief	DAC init
* @param	DACx is the DAC to init
* @param	format is data format, can be DAC_FORMAT_LSB12B, DAC_FORMAT_MSB12B and DAC_FORMAT_8B
* @return
*******************************************************************************************************************************/
void DAC_Init(DAC_TypeDef * DACx, uint32_t format)
{
	switch((uint32_t)DACx)
	{
	case ((uint32_t)DAC):
		SYS->CLKEN1 |= (0x01 << SYS_CLKEN1_DAC_Pos);
		break;
	}
	
	SYS->DACCR &= ~SYS_DACCR_VRADJ_Msk;
	SYS->DACCR |= ((SYS->BACKUP[2] & 0x1F) << SYS_DACCR_VRADJ_Pos);
	
	DACx->CR = (format << DAC_CR_DHRFMT_Pos);
}

/*******************************************************************************************************************************
* @brief	DAC open
* @param	DACx is the DAC to open
* @return
*******************************************************************************************************************************/
void DAC_Open(DAC_TypeDef * DACx)
{
	DACx->CR |= (1 << DAC_CR_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	DAC close
* @param	DACx is the DAC to close
* @return
*******************************************************************************************************************************/
void DAC_Close(DAC_TypeDef * DACx)
{
	DACx->CR &= ~DAC_CR_EN_Msk;
}
