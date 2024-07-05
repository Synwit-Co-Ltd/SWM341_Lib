/*******************************************************************************************************************************
* @brief	CAN driver
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
#include "SWM341_can.h"


/*******************************************************************************************************************************
* @brief	CAN init
* @param	CANx is the CAN to init
* @param	initStruct is data used to init the CAN
* @return
*******************************************************************************************************************************/
void CAN_Init(CAN_TypeDef * CANx, CAN_InitStructure * initStruct)
{	
	uint32_t brp = (SystemCoreClock/2)/2/initStruct->Baudrate/(1 + (initStruct->CAN_bs1 + 1) + (initStruct->CAN_bs2 + 1)) - 1;
	
	switch((uint32_t)CANx)
	{
	case ((uint32_t)CAN0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_CAN0_Pos);
		break;
	
	case ((uint32_t)CAN1):
		SYS->CLKEN1 |= (0x01 << SYS_CLKEN1_CAN1_Pos);
		break;
	}
	
	CAN_Close(CANx);
	
	CANx->CR &= ~(CAN_CR_LOM_Msk | CAN_CR_STM_Msk);
	CANx->CR |= (initStruct->Mode << CAN_CR_LOM_Pos);
	
	CANx->BT1 = (0 << CAN_BT1_SAM_Pos) |
				(initStruct->CAN_bs1 << CAN_BT1_TSEG1_Pos) |
				(initStruct->CAN_bs2 << CAN_BT1_TSEG2_Pos);
	
	CANx->BT0 = (initStruct->CAN_sjw << CAN_BT0_SJW_Pos) |
				((brp & 0x3F) << CAN_BT0_BRP_Pos);
	
	CANx->BT2 = ((brp >> 6) << CAN_BT2_BRP_Pos);
	
	CANx->RXERR = 0;	// can only be cleared in reset mode
	CANx->TXERR = 0;
	
	CANx->IE = (initStruct->RXNotEmptyIEn << CAN_IE_RXDA_Pos)    |
			   (initStruct->ArbitrLostIEn << CAN_IE_ARBLOST_Pos) |
			   (initStruct->ErrPassiveIEn << CAN_IE_ERRPASS_Pos);
	
	switch((uint32_t)CANx)
	{
	case ((uint32_t)CAN0):
		if(initStruct->RXNotEmptyIEn | initStruct->ArbitrLostIEn | initStruct->ErrPassiveIEn)
		{
			NVIC_EnableIRQ(CAN0_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(CAN0_IRQn);
		}
		break;
	
	case ((uint32_t)CAN1):
		if(initStruct->RXNotEmptyIEn | initStruct->ArbitrLostIEn | initStruct->ErrPassiveIEn)
		{
			NVIC_EnableIRQ(CAN1_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(CAN1_IRQn);
		}
		break;
	}
}

/*******************************************************************************************************************************
* @brief	CAN open, exit from reset mode and enter working mode
* @param	CANx is the CAN to open
* @return
*******************************************************************************************************************************/
void CAN_Open(CAN_TypeDef * CANx)
{
	CANx->CR &= ~(0x01 << CAN_CR_RST_Pos);
}

/*******************************************************************************************************************************
* @brief	CAN close, enter reset mode
* @param	CANx is the CAN to close
* @return
*******************************************************************************************************************************/
void CAN_Close(CAN_TypeDef * CANx)
{
	CANx->CR |= (0x01 << CAN_CR_RST_Pos);
}

/*******************************************************************************************************************************
* @brief	CAN send a data frame
* @param	CANx is the CAN to use
* @param	format: CAN_FRAME_STD, CAN_FRAME_EXT
* @param	id is message ID
* @param	data is the data to send
* @param	size is number of byte to be sent
* @param	once = 1: only send once, even if sending fails (arbitration lost, sending error, NAK) no retry is attempted
* @return
*******************************************************************************************************************************/
void CAN_Transmit(CAN_TypeDef * CANx, uint32_t format, uint32_t id, uint8_t data[], uint32_t size, uint32_t once)
{
	uint32_t i;
	
	if(format == CAN_FRAME_STD)
	{
		CANx->FRAME.INFO = (0 << CAN_INFO_FF_Pos)  |
							 (0 << CAN_INFO_RTR_Pos) |
							 (size << CAN_INFO_DLC_Pos);
		
		CANx->FRAME.DATA[0] = id >> 3;
		CANx->FRAME.DATA[1] = id << 5;
		
		for(i = 0; i < size; i++)
		{
			CANx->FRAME.DATA[i+2] = data[i];
		}
	}
	else //if(format == CAN_FRAME_EXT)
	{
		CANx->FRAME.INFO = (1 << CAN_INFO_FF_Pos)  |
							 (0 << CAN_INFO_RTR_Pos) |
							 (size << CAN_INFO_DLC_Pos);
		
		CANx->FRAME.DATA[0] = id >> 21;
		CANx->FRAME.DATA[1] = id >> 13;
		CANx->FRAME.DATA[2] = id >>  5;
		CANx->FRAME.DATA[3] = id <<  3;
		
		for(i = 0; i < size; i++)
		{
			CANx->FRAME.DATA[i+4] = data[i];
		}
	}
	
	if(CANx->CR & CAN_CR_STM_Msk)
	{
		CANx->CMD = (1 << CAN_CMD_SRR_Pos);
	}
	else
	{
		if(once == 0)
		{
			CANx->CMD = (1 << CAN_CMD_TXREQ_Pos);
		}
		else
		{
			CANx->CMD = (1 << CAN_CMD_TXREQ_Pos) | (1 << CAN_CMD_ABTTX_Pos);
		}
	}
}

/*******************************************************************************************************************************
* @brief	CAN send a remote frame
* @param	CANx is the CAN to use
* @param	format: CAN_FRAME_STD, CAN_FRAME_EXT
* @param	id is message ID
* @param	once = 1: only send once, even if sending fails (arbitration lost, sending error, NAK) no retry is attempted
* @return
*******************************************************************************************************************************/
void CAN_TransmitRequest(CAN_TypeDef * CANx, uint32_t format, uint32_t id, uint32_t once)
{
	if(format == CAN_FRAME_STD)
	{
		CANx->FRAME.INFO = (0 << CAN_INFO_FF_Pos)  |
							 (1 << CAN_INFO_RTR_Pos) |
							 (0 << CAN_INFO_DLC_Pos);
		
		CANx->FRAME.DATA[0] = id >> 3;
		CANx->FRAME.DATA[1] = id << 5;
	}
	else //if(format == CAN_FRAME_EXT)
	{
		CANx->FRAME.INFO = (1 << CAN_INFO_FF_Pos)  |
							 (1 << CAN_INFO_RTR_Pos) |
							 (0 << CAN_INFO_DLC_Pos);
		
		CANx->FRAME.DATA[0] = id >> 21;
		CANx->FRAME.DATA[1] = id >> 13;
		CANx->FRAME.DATA[2] = id >>  5;
		CANx->FRAME.DATA[3] = id <<  3;
	}
	
	if(once == 0)
	{
		CANx->CMD = (1 << CAN_CMD_TXREQ_Pos);
	}
	else
	{
		CANx->CMD = (1 << CAN_CMD_TXREQ_Pos) | (1 << CAN_CMD_ABTTX_Pos);
	}
}

/*******************************************************************************************************************************
* @brief	read out a received CAN Frame
* @param	CANx is the CAN to use
* @param	msg is used to save received CAN Frame data
* @return
*******************************************************************************************************************************/
void CAN_Receive(CAN_TypeDef * CANx, CAN_RXMessage *msg)
{
	uint32_t i;
	msg->format = (CANx->FRAME.INFO & CAN_INFO_FF_Msk) >> CAN_INFO_FF_Pos;
	
	msg->remote = (CANx->FRAME.INFO & CAN_INFO_RTR_Msk) >> CAN_INFO_RTR_Pos;
	msg->size = (CANx->FRAME.INFO & CAN_INFO_DLC_Msk) >> CAN_INFO_DLC_Pos;
	
	if(msg->format == CAN_FRAME_STD)
	{
		msg->id = (CANx->FRAME.DATA[0] << 3) | (CANx->FRAME.DATA[1] >> 5);
		
		for(i = 0; i < msg->size; i++)
		{
			msg->data[i] = CANx->FRAME.DATA[i+2];
		}
	}
	else //if(msg->format == CAN_FRAME_EXT)
	{
		msg->id = (CANx->FRAME.DATA[0] << 21) | (CANx->FRAME.DATA[1] << 13) | (CANx->FRAME.DATA[2] << 5) | (CANx->FRAME.DATA[3] >> 3);
		
		for(i = 0; i < msg->size; i++)
		{
			msg->data[i] = CANx->FRAME.DATA[i+4];
		}
	}
	
	CANx->CMD = (1 << CAN_CMD_RRB_Pos);
}

/*******************************************************************************************************************************
* @brief	CAN transmit complete query
* @param	CANx is the CAN to query
* @return	1 transmit complete, 0 not complete
* @note		Sending abort also triggers sending completion, but does not trigger sending success
*******************************************************************************************************************************/
uint32_t CAN_TXComplete(CAN_TypeDef * CANx)
{
	return (CANx->SR & CAN_SR_TXBR_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	CAN transmit success query
* @param	CANx is the CAN to query
* @return	1 transmit success, 0 transmit fail
*******************************************************************************************************************************/
uint32_t CAN_TXSuccess(CAN_TypeDef * CANx)
{
	return (CANx->SR & CAN_SR_TXOK_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	CAN abort transmit
* @param	CANx is the CAN to abort
* @return
* @note		The sending in progress cannot be terminated, but if the sending fails, the sending will not be repeated
*******************************************************************************************************************************/
void CAN_AbortTransmit(CAN_TypeDef * CANx)
{
	CANx->CMD = (1 << CAN_CMD_ABTTX_Pos);
}

/*******************************************************************************************************************************
* @brief	CAN transmit buffer ready query
* @param	CANx is the CAN to query
* @return	1 transmit buffer ready, can write now, 0 not ready
*******************************************************************************************************************************/
uint32_t CAN_TXBufferReady(CAN_TypeDef * CANx)
{
	return (CANx->SR & CAN_SR_TXBR_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	CAN receive buffer data available query
* @param	CANx is the CAN to query
* @return	1 receive buffer has data, can read now, 0 receive buffer empty
*******************************************************************************************************************************/
uint32_t CAN_RXDataAvailable(CAN_TypeDef * CANx)
{
	return (CANx->SR & CAN_SR_RXDA_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	CAN baudrate set
* @param	CANx is the CAN to set
* @param	baudrate is new baudrate value to set
* @param	CAN_bs1: CAN_BS1_1tq, CAN_BS1_2tq, ... ... , CAN_BS1_16tq
* @param	CAN_bs2: CAN_BS2_1tq, CAN_BS2_2tq, ... ... , CAN_BS2_8tq
* @param	CAN_sjw: CAN_SJW_1tq, CAN_SJW_2tq, CAN_SJW_3tq, CAN_SJW_4tq
* @return
* @note		baudrate can only be changed in reset mode, use CAN_Close() to enter reset mode
*******************************************************************************************************************************/
void CAN_SetBaudrate(CAN_TypeDef * CANx, uint32_t baudrate, uint32_t CAN_bs1, uint32_t CAN_bs2, uint32_t CAN_sjw)
{
	uint32_t brp = (SystemCoreClock/2)/2/baudrate/(1 + (CAN_bs1 + 1) + (CAN_bs2 + 1)) - 1;
	
	CANx->BT1 = (0 << CAN_BT1_SAM_Pos) |
				(CAN_bs1 << CAN_BT1_TSEG1_Pos) |
				(CAN_bs2 << CAN_BT1_TSEG2_Pos);
	
	CANx->BT0 = (CAN_sjw << CAN_BT0_SJW_Pos) |
				((brp & 0x3F) << CAN_BT0_BRP_Pos);
	
	CANx->BT2 = ((brp >> 6) << CAN_BT2_BRP_Pos);
}

/*******************************************************************************************************************************
* @brief	Configure specified filter as one 32-bit receive filter, used to filter Extended Frame
* @param	CANx is the CAN to set
* @param	filter is the filter to configure, can be CAN_FILTER_1, CAN_FILTER_2, ..., CAN_FILTER_16
* @param	check work with mask to filter received message, accepte message when check & mask == ID & mask
* @param	mask
* @return
* @note		filter can only be changed in reset mode, use CAN_Close() to enter reset mode
*******************************************************************************************************************************/
void CAN_SetFilter32b(CAN_TypeDef * CANx, uint32_t filter, uint32_t check, uint32_t mask)
{	
	CANx->AFM |= (1 << filter);
	
	CANx->ACR[filter] = __REV(check << 3);
	CANx->AMR[filter] = __REV(~(mask << 3));
	
	CANx->AFE |= (1 << filter);
}

/*******************************************************************************************************************************
* @brief	Configure specified filter as two 16-bit receive filter, used to filter Standard Frame
* @param	CANx is the CAN to set
* @param	filter is the filter to configure, can be CAN_FILTER_1, CAN_FILTER_2, ..., CAN_FILTER_16
* @param	check1 work with mask1 to filter received message, accepte message when check1 & mask1 == ID & mask1
* @param	mask1
* @param	check2
* @param	mask2
* @return
* @note		filter can only be changed in reset mode, use CAN_Close() to enter reset mode
*******************************************************************************************************************************/
void CAN_SetFilter16b(CAN_TypeDef * CANx, uint32_t filter, uint16_t check1, uint16_t mask1, uint16_t check2, uint16_t mask2)
{
	CANx->AFM &= ~(1 << filter);
	
	CANx->ACR[filter] = __REV((check1 << 5) | (check2 << 21));
	CANx->AMR[filter] = __REV(~((mask1 << 5) | (mask2 << 21)));
	
	CANx->AFE |= (1 << filter);
}

/*******************************************************************************************************************************
* @brief	CAN interrupt enable
* @param	CANx is the CAN to set
* @param	it is interrupt type, can be CAN_IT_RX_NOTEMPTY, CAN_IT_RX_OVERFLOW, CAN_IT_TX_EMPTY, CAN_IT_ARBLOST, CAN_IT_ERR, 
*			CAN_IT_ERR_WARN, CAN_IT_ERR_PASS, CAN_IT_WAKEUP and their '|' operation
* @return
*******************************************************************************************************************************/
void CAN_INTEn(CAN_TypeDef * CANx, uint32_t it)
{
	CANx->IE |= it;
}

/*******************************************************************************************************************************
* @brief	CAN interrupt disable
* @param	CANx is the CAN to set
* @param	it is interrupt type, can be CAN_IT_RX_NOTEMPTY, CAN_IT_RX_OVERFLOW, CAN_IT_TX_EMPTY, CAN_IT_ARBLOST, CAN_IT_ERR, 
*			CAN_IT_ERR_WARN, CAN_IT_ERR_PASS, CAN_IT_WAKEUP and their '|' operation
* @return
*******************************************************************************************************************************/
void CAN_INTDis(CAN_TypeDef * CANx, uint32_t it)
{
	CANx->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	CAN interrupt flag clear
* @param	CANx is the CAN to clear
* @param	it is interrupt type, can be CAN_IT_RX_OVERFLOW
* @return
*******************************************************************************************************************************/
void CAN_INTClr(CAN_TypeDef * CANx, uint32_t it)
{
	CANx->CMD = (1 << CAN_CMD_CLROV_Pos);
}

/*******************************************************************************************************************************
* @brief	CAN interrupt state query
* @param	CANx is the CAN to query
* @return	current interupt state
* @note		CANx->IF reads clear to zero, so in ISR can only be read once, not multiple times
*******************************************************************************************************************************/
uint32_t CAN_INTStat(CAN_TypeDef * CANx)
{
	return CANx->IF;
}
