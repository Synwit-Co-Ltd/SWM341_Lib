/*******************************************************************************************************************************
* @brief	DMA driver
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
#include "SWM341_dma.h"


/*******************************************************************************************************************************
* @brief	DMA channel init
* @param	chn is the DMA channel to init, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @param	initStruct is data used to init the DMA channel
* @return
*******************************************************************************************************************************/
void DMA_CH_Init(uint32_t chn, DMA_InitStructure * initStruct)
{
	DMA->EN = 1;
	
	DMA_CH_Close(chn);
	
	DMA->CH[chn].CR = (initStruct->Mode << DMA_CR_AUTORE_Pos) | 
					 ((initStruct->Count ? initStruct->Count - 1 : 0) << DMA_CR_LEN_Pos);
	
	DMA->CH[chn].SRC = initStruct->SrcAddr;
	DMA->CH[chn].DST = initStruct->DstAddr;
	
	DMA->CH[chn].AM = (initStruct->SrcAddrInc << DMA_AM_SRCAM_Pos)  |
					  (initStruct->DstAddrInc << DMA_AM_DSTAM_Pos)  |
					  (initStruct->Unit       << DMA_AM_SRCBIT_Pos) |
					  (initStruct->Unit       << DMA_AM_DSTBIT_Pos);
	
	switch(initStruct->Handshake & DMA_HS_MSK)
	{
	case DMA_HS_NO:
		DMA->CH[chn].MUX = 0;
		break;
	
	case DMA_HS_SRC:
		DMA->CH[chn].MUX = ((initStruct->Handshake & 0xF) << DMA_MUX_SRCHSSIG_Pos) | (1 << DMA_MUX_SRCHSEN_Pos);
		break;
	
	case DMA_HS_DST:
		DMA->CH[chn].MUX = ((initStruct->Handshake & 0xF) << DMA_MUX_DSTHSSIG_Pos) | (1 << DMA_MUX_DSTHSEN_Pos);
		break;
	
	case DMA_HS_EXT:
		DMA->CH[chn].MUX = ((initStruct->Handshake & 0xF) << DMA_MUX_EXTHSSIG_Pos) | (1 << DMA_MUX_EXTHSEN_Pos);
		DMA->CH[chn].CR |= (1 << DMA_CR_STEPOP_Pos);
		break;
	
	default:
		break;
	}
	
	int totalBytes = initStruct->Count * (1 << initStruct->Unit);
	
	if(initStruct->DstAddrInc == 2)		// Destination Scatter-Gather Transfer
	{
		DMA->CH[chn].DSTSGADDR1 = initStruct->DstAddr + totalBytes / 4 * 1;
		DMA->CH[chn].DSTSGADDR2 = initStruct->DstAddr + totalBytes / 4 * 2;
		DMA->CH[chn].DSTSGADDR3 = initStruct->DstAddr + totalBytes / 4 * 3;
	}
	if(initStruct->SrcAddrInc == 2)		// Source      Scatter-Gather Transfer
	{
		DMA->CH[chn].SRCSGADDR1 = initStruct->SrcAddr + totalBytes / 4 * 1;
		DMA->CH[chn].SRCSGADDR2 = initStruct->SrcAddr + totalBytes / 4 * 2;
		DMA->CH[chn].SRCSGADDR3 = initStruct->SrcAddr + totalBytes / 4 * 3;
	}

	DMA->PRI &= ~(1 << chn);
	DMA->PRI |= (initStruct->Priority << chn);
	
	DMA->IM |= (1 << chn);	// default all off
	DMA->DSTSGIM |= (3 << (chn * 2));
	DMA->SRCSGIM |= (3 << (chn * 2));
	DMA->IE |= (1 << chn);	// flag is always checkable
	DMA->DSTSGIE |= (3 << (chn * 2));
	DMA->SRCSGIE |= (3 << (chn * 2));
	
	DMA_CH_INTClr(chn, initStruct->INTEn);
	DMA_CH_INTEn(chn, initStruct->INTEn);
	
	if(initStruct->INTEn) NVIC_EnableIRQ(DMA_IRQn);
}

/*******************************************************************************************************************************
* @brief	DMA channel open
* @param	chn is the DMA channel to open, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @return
*******************************************************************************************************************************/
void DMA_CH_Open(uint32_t chn)
{
	DMA->CH[chn].CR |= (1 << DMA_CR_RXEN_Pos);
}

/*******************************************************************************************************************************
* @brief	DMA channel close
* @param	chn is the DMA channel to close, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @return
*******************************************************************************************************************************/
void DMA_CH_Close(uint32_t chn)
{
	DMA->CH[chn].CR &= ~(DMA_CR_TXEN_Msk | DMA_CR_RXEN_Msk);
}

/*******************************************************************************************************************************
* @brief	set transfer count
* @param	chn is the DMA channel to set, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @param	count is transfer count to set
* @return
*******************************************************************************************************************************/
void DMA_CH_SetCount(uint32_t chn, uint32_t count)
{
	DMA->CH[chn].CR &= ~DMA_CR_LEN_Msk;
	DMA->CH[chn].CR |= ((count - 1) << DMA_CR_LEN_Pos);
}

/*******************************************************************************************************************************
* @brief	query remaining transfer count
* @param	chn is the DMA channel to query, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @return	remaining transfer count
*******************************************************************************************************************************/
uint32_t DMA_CH_GetRemaining(uint32_t chn)
{
	return (DMA->CH[chn].DSTSR & DMA_DSTSR_LEN_Msk);
}

/*******************************************************************************************************************************
* @brief	set transfer source address
* @param	chn is the DMA channel to set, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @param	address is transfer source address to set
* @return
*******************************************************************************************************************************/
void DMA_CH_SetSrcAddress(uint32_t chn, uint32_t address)
{
	DMA->CH[chn].SRC = address;
}

/*******************************************************************************************************************************
* @brief	set transfer destination address
* @param	chn is the DMA channel to set, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @param	address is transfer destination address to set
* @return
*******************************************************************************************************************************/
void DMA_CH_SetDstAddress(uint32_t chn, uint32_t address)
{
	DMA->CH[chn].DST = address;
}

/*******************************************************************************************************************************
* @brief	DMA interrupt enable
* @param	chn is the DMA channel to set, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @param	it is interrupt type, can be DMA_IT_DONE, DMA_IT_DSTSG_HALF, DMA_IT_DSTSG_DONE, DMA_IT_SRCSG_HALF, DMA_IT_SRCSG_DONE and their '|' operation
* @return
*******************************************************************************************************************************/
void DMA_CH_INTEn(uint32_t chn, uint32_t it)
{
	DMA->IM &= ~(it << chn);
	DMA->DSTSGIM &= ~((it >>  8) << (chn * 2));
	DMA->SRCSGIM &= ~((it >> 16) << (chn * 2));
}

/*******************************************************************************************************************************
* @brief	DMA interrupt disable
* @param	chn is the DMA channel to set, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @param	it is interrupt type, can be DMA_IT_DONE, DMA_IT_DSTSG_HALF, DMA_IT_DSTSG_DONE, DMA_IT_SRCSG_HALF, DMA_IT_SRCSG_DONE and their '|' operation
* @return
*******************************************************************************************************************************/
void DMA_CH_INTDis(uint32_t chn, uint32_t it)
{
	DMA->IM |=  (it << chn);
	DMA->DSTSGIM |=  ((it >>  8) << (chn * 2));
	DMA->SRCSGIM |=  ((it >> 16) << (chn * 2));
}

/*******************************************************************************************************************************
* @brief	DMA interrupt flag clear
* @param	chn is the DMA channel to set, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @param	it is interrupt type, can be DMA_IT_DONE, DMA_IT_DSTSG_HALF, DMA_IT_DSTSG_DONE, DMA_IT_SRCSG_HALF, DMA_IT_SRCSG_DONE and their '|' operation
* @return
*******************************************************************************************************************************/
void DMA_CH_INTClr(uint32_t chn, uint32_t it)
{
	DMA->IF = (it << chn);
	DMA->DSTSGIF = ((it >>  8) << (chn * 2));
	DMA->SRCSGIF = ((it >> 16) << (chn * 2));
}

/*******************************************************************************************************************************
* @brief	DMA interrupt state query
* @param	chn is the DMA channel to query, can be DMA_CH0, DMA_CH1, DMA_CH2 and DMA_CH3
* @param	it is interrupt type, can be DMA_IT_DONE, DMA_IT_DSTSG_HALF, DMA_IT_DSTSG_DONE, DMA_IT_SRCSG_HALF, DMA_IT_SRCSG_DONE and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t DMA_CH_INTStat(uint32_t chn, uint32_t it)
{
	return ((DMA->IF & (it << chn)) ||
			(DMA->DSTSGIF & ((it >>  8) << (chn * 2))) ||
			(DMA->SRCSGIF & ((it >> 16) << (chn * 2))));
}
