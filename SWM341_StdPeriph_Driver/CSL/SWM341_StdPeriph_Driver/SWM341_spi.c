/*******************************************************************************************************************************
* @brief	SPI driver
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
#include "SWM341_spi.h"


/*******************************************************************************************************************************
* @brief	SPI init
* @param	SPIx is the SPI to init
* @param	initStruct is data used to init the SPI
* @return
*******************************************************************************************************************************/
void SPI_Init(SPI_TypeDef * SPIx, SPI_InitStructure * initStruct)
{
	uint32_t fast = 0, no_sync = 0;
	
	switch((uint32_t)SPIx)
	{
	case ((uint32_t)SPI0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_SPI0_Pos);
		break;

	case ((uint32_t)SPI1):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_SPI1_Pos);
		break;
	}
	
	SPI_Close(SPIx);
	
	if(initStruct->clkDiv == SPI_CLKDIV_2)
	{
		fast = 1;
		no_sync = 1;
	}
	else if(initStruct->clkDiv == SPI_CLKDIV_4)
	{
		no_sync = 1;
	}
	
	SPIx->CTRL &= ~(SPI_CTRL_FFS_Msk | SPI_CTRL_CPHA_Msk | SPI_CTRL_CPOL_Msk | SPI_CTRL_SIZE_Msk | SPI_CTRL_MSTR_Msk | SPI_CTRL_FAST_Msk | SPI_CTRL_NSYNC_Msk |
	                SPI_CTRL_CLKDIV_Msk | SPI_CTRL_SSN_H_Msk | SPI_CTRL_RFTHR_Msk | SPI_CTRL_TFTHR_Msk);
	SPIx->CTRL |= (initStruct->FrameFormat     << SPI_CTRL_FFS_Pos)    |
				  (initStruct->SampleEdge      << SPI_CTRL_CPHA_Pos)   |
				  (initStruct->IdleLevel       << SPI_CTRL_CPOL_Pos)   |
				  ((initStruct->WordSize-1)    << SPI_CTRL_SIZE_Pos)   |
				  (initStruct->Master          << SPI_CTRL_MSTR_Pos)   |
				  (fast                        << SPI_CTRL_FAST_Pos)   |
				  (no_sync                     << SPI_CTRL_NSYNC_Pos)  |
				  ((initStruct->clkDiv & 7)    << SPI_CTRL_CLKDIV_Pos) |
				  (0                           << SPI_CTRL_SSN_H_Pos)  |
				  ((initStruct->RXThreshold > 0 ? initStruct->RXThreshold-1 : 0) << SPI_CTRL_RFTHR_Pos)  |
				  (initStruct->TXThreshold     << SPI_CTRL_TFTHR_Pos)  |
				  (1                           << SPI_CTRL_RFCLR_Pos)  |
	              (1                           << SPI_CTRL_TFCLR_Pos);
	SPIx->CTRL &= ~(SPI_CTRL_RFCLR_Msk | SPI_CTRL_TFCLR_Msk);
	
	SPIx->IF = 0x37F;	// clear interrupt flag
	SPIx->IE = 0x000;
	SPIx->IE |= (initStruct->RXThresholdIEn << SPI_IE_RFTHR_Pos) |
				(initStruct->TXThresholdIEn << SPI_IE_TFTHR_Pos) |
	            (initStruct->TXCompleteIEn  << SPI_IE_FTC_Pos);
	
	switch((uint32_t)SPIx)
	{
	case ((uint32_t)SPI0):		
		if(initStruct->RXThresholdIEn | initStruct->TXThresholdIEn | initStruct->TXCompleteIEn)
		{
			NVIC_EnableIRQ(SPI0_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(SPI0_IRQn);
		}
		break;
	
	case ((uint32_t)SPI1):		
		if(initStruct->RXThresholdIEn | initStruct->TXThresholdIEn | initStruct->TXCompleteIEn)
		{
			NVIC_EnableIRQ(SPI1_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(SPI1_IRQn);
		}
		break;
	}
}

/*******************************************************************************************************************************
* @brief	SPI open
* @param	SPIx is the SPI to open
* @return
*******************************************************************************************************************************/
void SPI_Open(SPI_TypeDef * SPIx)
{
	SPIx->CTRL |= (0x01 << SPI_CTRL_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	SPI close
* @param	SPIx is the SPI to close
* @return
*******************************************************************************************************************************/
void SPI_Close(SPI_TypeDef * SPIx)
{
	SPIx->CTRL &= ~SPI_CTRL_EN_Msk;
}

/*******************************************************************************************************************************
* @brief	read out received data
* @param	SPIx is the SPI to use
* @return	received data
*******************************************************************************************************************************/
uint32_t SPI_Read(SPI_TypeDef * SPIx)
{
	return SPIx->DATA;
}

/*******************************************************************************************************************************
* @brief	write a data to send
* @param	SPIx is the SPI to use
* @param	data is the data to send
* @return
*******************************************************************************************************************************/
void SPI_Write(SPI_TypeDef * SPIx, uint32_t data)
{
	SPIx->DATA = data;
}

/*******************************************************************************************************************************
* @brief	write a data to send, and wait sending done
* @param	SPIx is the SPI to use
* @param	data is the data to send
* @return
*******************************************************************************************************************************/
void SPI_WriteWithWait(SPI_TypeDef * SPIx, uint32_t data)
{
	SPIx->STAT |= (1 << SPI_STAT_WTC_Pos);
	
	SPIx->DATA = data;
	
	while((SPIx->STAT & SPI_STAT_WTC_Msk) == 0);
}

/*******************************************************************************************************************************
* @brief	write a data to send, and wait sending done, finally return received data
* @param	SPIx is the SPI to use
* @param	data is the data to send
* @return	received data
*******************************************************************************************************************************/
uint32_t SPI_ReadWrite(SPI_TypeDef * SPIx, uint32_t data)
{
	SPIx->DATA = data;
	while(!(SPIx->STAT & SPI_STAT_RFNE_Msk));
	
	return SPIx->DATA;
}

/*******************************************************************************************************************************
* @brief	SPI RX FIFO empty query
* @param	SPIx is the SPI to query
* @return	1 RX FIFO empty, 0 RX FIFO not empty, can read data out by SPI_Read()
*******************************************************************************************************************************/
uint32_t SPI_IsRXEmpty(SPI_TypeDef * SPIx)
{
	return (SPIx->STAT & SPI_STAT_RFNE_Msk) ? 0 : 1;
}

/*******************************************************************************************************************************
* @brief	SPI TX FIFO full query
* @param	SPIx is the SPI to query
* @return	1 TX FIFO full, 0 TX FIFO not full, can write data by SPI_Write()
*******************************************************************************************************************************/
uint32_t SPI_IsTXFull(SPI_TypeDef * SPIx)
{
	return (SPIx->STAT & SPI_STAT_TFNF_Msk) ? 0 : 1;
}

/*******************************************************************************************************************************
* @brief	SPI TX FIFO empty query
* @param	SPIx is the SPI to query
* @return	1 TX FIFO empty, 0 TX FIFO not empty
*******************************************************************************************************************************/
uint32_t SPI_IsTXEmpty(SPI_TypeDef * SPIx)
{
	return (SPIx->STAT & SPI_STAT_TFE_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	SPI interrupt enable
* @param	SPIx is the SPI to set
* @param	it is interrupt type, can be SPI_IT_RX_OVF, SPI_IT_RX_FULL, SPI_IT_RX_HFULL, SPI_IT_TX_EMPTY, SPI_IT_TX_HFULL, 
*			SPI_IT_RX_THRES, SPI_IT_TX_THRES, SPI_IT_TX_DONE, SPI_IT_CS_FALL, SPI_IT_CS_RISE and their '|' operation
* @return
*******************************************************************************************************************************/
void SPI_INTEn(SPI_TypeDef * SPIx, uint32_t it)
{
	SPIx->IE |= it;
}

/*******************************************************************************************************************************
* @brief	SPI interrupt disable
* @param	SPIx is the SPI to set
* @param	it is interrupt type, can be SPI_IT_RX_OVF, SPI_IT_RX_FULL, SPI_IT_RX_HFULL, SPI_IT_TX_EMPTY, SPI_IT_TX_HFULL, 
*			SPI_IT_RX_THRES, SPI_IT_TX_THRES, SPI_IT_TX_DONE, SPI_IT_CS_FALL, SPI_IT_CS_RISE and their '|' operation
* @return
*******************************************************************************************************************************/
void SPI_INTDis(SPI_TypeDef * SPIx, uint32_t it)
{
	SPIx->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	SPI interrupt flag clear
* @param	SPIx is the SPI to clear
* @param	it is interrupt type, can be SPI_IT_RX_OVF, SPI_IT_RX_FULL, SPI_IT_RX_HFULL, SPI_IT_TX_EMPTY, SPI_IT_TX_HFULL, 
*			SPI_IT_RX_THRES, SPI_IT_TX_THRES, SPI_IT_TX_DONE, SPI_IT_CS_FALL, SPI_IT_CS_RISE and their '|' operation
* @return
*******************************************************************************************************************************/
void SPI_INTClr(SPI_TypeDef * SPIx, uint32_t it)
{
	SPIx->IF = it;
}

/*******************************************************************************************************************************
* @brief	SPI interrupt state query
* @param	SPIx is the SPI to query
* @param	it is interrupt type, can be SPI_IT_RX_OVF, SPI_IT_RX_FULL, SPI_IT_RX_HFULL, SPI_IT_TX_EMPTY, SPI_IT_TX_HFULL, 
*			SPI_IT_RX_THRES, SPI_IT_TX_THRES, SPI_IT_TX_DONE, SPI_IT_CS_FALL, SPI_IT_CS_RISE and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t SPI_INTStat(SPI_TypeDef * SPIx, uint32_t it)
{
	return (SPIx->IF & it) ? 1 : 0;
}


/*******************************************************************************************************************************
* @brief	I2S init
* @param	SPIx is the I2S to init, SPI works in I2S mode
* @param	initStruct is data used to init the I2S
* @return
*******************************************************************************************************************************/
void I2S_Init(SPI_TypeDef * SPIx, I2S_InitStructure * initStruct)
{
	switch((uint32_t)SPIx)
	{
	case ((uint32_t)SPI0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_SPI0_Pos);
		break;

	case ((uint32_t)SPI1):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_SPI1_Pos);
		break;
	}
	
	I2S_Close(SPIx);
	
	SPIx->CTRL &= ~(SPI_CTRL_FFS_Msk | SPI_CTRL_RFTHR_Msk | SPI_CTRL_TFTHR_Msk);
	SPIx->CTRL |= (SPI_FORMAT_I2S     		   << SPI_CTRL_FFS_Pos)    |
				  ((initStruct->RXThreshold > 0 ? initStruct->RXThreshold-1 : 0) << SPI_CTRL_RFTHR_Pos)  |
				  (initStruct->TXThreshold     << SPI_CTRL_TFTHR_Pos)  |
				  (1                           << SPI_CTRL_RFCLR_Pos)  |
	              (1                           << SPI_CTRL_TFCLR_Pos);
	SPIx->CTRL &= ~(SPI_CTRL_RFCLR_Msk | SPI_CTRL_TFCLR_Msk);
	
	SPIx->I2SCR &= ~(SPI_I2SCR_MSTR_Msk | SPI_I2SCR_DIEN_Msk | SPI_I2SCR_DOEN_Msk | SPI_I2SCR_FFMT_Msk | SPI_I2SCR_DLEN_Msk | SPI_I2SCR_CHLEN_Msk | SPI_I2SCR_PCMSYNW_Msk);
	SPIx->I2SCR |= ((initStruct->Mode & 0x04 ? 1 : 0) << SPI_I2SCR_MSTR_Pos) |
				   ((initStruct->Mode & 0x02 ? 1 : 0) << SPI_I2SCR_DOEN_Pos) |
				   ((initStruct->Mode & 0x01 ? 1 : 0) << SPI_I2SCR_DIEN_Pos) |
				   ((initStruct->FrameFormat & 0x03)  << SPI_I2SCR_FFMT_Pos) |
				   (initStruct->DataLen               << SPI_I2SCR_DLEN_Pos) |
				   (initStruct->ChannelLen			  << SPI_I2SCR_CHLEN_Pos) |
				   ((initStruct->FrameFormat & 0x04 ? 1 : 0) << SPI_I2SCR_PCMSYNW_Pos);
	
	SPIx->I2SPR &= ~SPI_I2SPR_SCLKDIV_Msk;
	SPIx->I2SPR |= (SystemCoreClock / initStruct->ClkFreq / 2 - 1) << SPI_I2SPR_SCLKDIV_Pos;
	
	SPIx->IF = 0x37F;	// clear interrupt flag
	SPIx->IE = 0x000;
	SPIx->IE |= (initStruct->RXThresholdIEn << SPI_IE_RFTHR_Pos) |
				(initStruct->TXThresholdIEn << SPI_IE_TFTHR_Pos) |
	            (initStruct->TXCompleteIEn  << SPI_IE_FTC_Pos);
	
	switch((uint32_t)SPIx)
	{
	case ((uint32_t)SPI0):		
		if(initStruct->RXThresholdIEn | initStruct->TXThresholdIEn | initStruct->TXCompleteIEn)
		{
			NVIC_EnableIRQ(SPI0_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(SPI0_IRQn);
		}
		break;
	
	case ((uint32_t)SPI1):		
		if(initStruct->RXThresholdIEn | initStruct->TXThresholdIEn | initStruct->TXCompleteIEn)
		{
			NVIC_EnableIRQ(SPI1_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(SPI1_IRQn);
		}
		break;
	}
}

/*******************************************************************************************************************************
* @brief	I2S open
* @param	SPIx is the I2S to open, SPI works in I2S mode
* @return
*******************************************************************************************************************************/
void I2S_Open(SPI_TypeDef * SPIx)
{
	SPIx->CTRL |= (0x01 << SPI_CTRL_EN_Pos);	// TX Complete interrupts can only be used if SPI is enabled
	SPIx->I2SCR |= (0x01 << SPI_I2SCR_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	I2S close
* @param	SPIx is the I2S to close, SPI works in I2S mode
* @return
*******************************************************************************************************************************/
void I2S_Close(SPI_TypeDef * SPIx)
{
	SPIx->CTRL &= ~SPI_CTRL_EN_Msk;
	SPIx->I2SCR &= ~SPI_I2SCR_EN_Msk;
}
