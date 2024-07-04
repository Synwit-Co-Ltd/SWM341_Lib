/*******************************************************************************************************************************
* @brief	UART driver
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
#include "SWM341_uart.h"


/*******************************************************************************************************************************
* @brief	UART init
* @param	UARTx is the UART to init
* @param	initStruct is data used to init the UART
* @return
*******************************************************************************************************************************/
void UART_Init(UART_TypeDef * UARTx, UART_InitStructure * initStruct)
{	
	switch((uint32_t)UARTx)
	{
	case ((uint32_t)UART0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_UART0_Pos);
		break;
	
	case ((uint32_t)UART1):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_UART1_Pos);
		break;
	
	case ((uint32_t)UART2):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_UART2_Pos);
		break;
	
	case ((uint32_t)UART3):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_UART3_Pos);
		break;
	}
	
	UART_Close(UARTx);
	
	UARTx->BAUD &= ~(UART_BAUD_BAUD_Msk | UART_BAUD_FRAC_Msk);
	UARTx->BAUD |= (((SystemCoreClock/initStruct->Baudrate - 1) / 16) << UART_BAUD_BAUD_Pos) |
				   (((SystemCoreClock/initStruct->Baudrate - 1) % 16) << UART_BAUD_FRAC_Pos);
	
	UARTx->CTRL &= ~(UART_CTRL_DATA9b_Msk | UART_CTRL_PARITY_Msk | UART_CTRL_STOP2b_Msk);
	UARTx->CTRL |= (initStruct->DataBits << UART_CTRL_DATA9b_Pos) |
				   (initStruct->Parity   << UART_CTRL_PARITY_Pos) |
				   (initStruct->StopBits << UART_CTRL_STOP2b_Pos);
	
	UARTx->FIFO &= ~(UART_FIFO_RXTHR_Msk | UART_FIFO_TXTHR_Msk);
	UARTx->FIFO |= (initStruct->RXThreshold << UART_FIFO_RXTHR_Pos) | 
				   (initStruct->TXThreshold << UART_FIFO_TXTHR_Pos);
	
	UARTx->TOCR &= ~UART_TOCR_TIME_Msk;
	UARTx->TOCR |= (1 << UART_TOCR_MODE_Pos) |
				   (initStruct->TimeoutTime << UART_TOCR_TIME_Pos);
	
	UARTx->CTRL &= ~(UART_CTRL_RXIE_Msk | UART_CTRL_TXIE_Msk | UART_CTRL_TOIE_Msk);
	UARTx->CTRL |= (initStruct->RXThresholdIEn << UART_CTRL_RXIE_Pos) |
				   (initStruct->TXThresholdIEn << UART_CTRL_TXIE_Pos) |
				   (initStruct->TimeoutIEn << UART_CTRL_TOIE_Pos);
	
	switch((uint32_t)UARTx)
	{
	case ((uint32_t)UART0):		
		if(initStruct->RXThresholdIEn | initStruct->TXThresholdIEn | initStruct->TimeoutIEn)
		{
			NVIC_EnableIRQ(UART0_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(UART0_IRQn);
		}
		break;
	
	case ((uint32_t)UART1):		
		if(initStruct->RXThresholdIEn | initStruct->TXThresholdIEn | initStruct->TimeoutIEn)
		{
			NVIC_EnableIRQ(UART1_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(UART1_IRQn);
		}
		break;
	
	case ((uint32_t)UART2):		
		if(initStruct->RXThresholdIEn | initStruct->TXThresholdIEn | initStruct->TimeoutIEn)
		{
			NVIC_EnableIRQ(UART2_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(UART2_IRQn);
		}
		break;
	
	case ((uint32_t)UART3):		
		if(initStruct->RXThresholdIEn | initStruct->TXThresholdIEn | initStruct->TimeoutIEn)
		{
			NVIC_EnableIRQ(UART3_IRQn);
		}
		else
		{
			NVIC_DisableIRQ(UART3_IRQn);
		}
		break;
	}
}

/*******************************************************************************************************************************
* @brief	UART open
* @param	UARTx is the UART to open
* @return
*******************************************************************************************************************************/
void UART_Open(UART_TypeDef * UARTx)
{
	UARTx->CTRL |= (0x01 << UART_CTRL_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	UART close
* @param	UARTx is the UART to close
* @return
*******************************************************************************************************************************/
void UART_Close(UART_TypeDef * UARTx)
{
	UARTx->CTRL &= ~(0x01 << UART_CTRL_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	send one byte of data
* @param	UARTx is the UART to send data out
* @param	data is the byte to send
* @return
*******************************************************************************************************************************/
void UART_WriteByte(UART_TypeDef * UARTx, uint8_t data)
{
	UARTx->DATA = data;
}

/*******************************************************************************************************************************
* @brief	read one byte of data and indicate whether the data is valid
* @param	UARTx is the UART to read data from
* @param	data is the byte received
* @return	0 data valid, UART_ERR_PARITY data parity error
*******************************************************************************************************************************/
uint32_t UART_ReadByte(UART_TypeDef * UARTx, uint32_t * data)
{
	uint32_t reg = UARTx->DATA;
	
	*data = (reg & UART_DATA_DATA_Msk);
	
	if(reg & UART_DATA_PAERR_Msk) return UART_ERR_PARITY;
	
	return 0;
}

/*******************************************************************************************************************************
* @brief	whether UART is sending data
* @param	UARTx is the UART to query
* @return	0 UART is sending data, 0 UART transmitter idle
*******************************************************************************************************************************/
uint32_t UART_IsTXBusy(UART_TypeDef * UARTx)
{
	return (UARTx->CTRL & UART_CTRL_TXIDLE_Msk) ? 0 : 1;
}

/*******************************************************************************************************************************
* @brief	whether RX FIFO is empty, if not, you can read data from it
* @param	UARTx is the UART to query
* @return	1 RX FIFO empty, 0 RX FIFO not empty
*******************************************************************************************************************************/
uint32_t UART_IsRXFIFOEmpty(UART_TypeDef * UARTx)
{
	return (UARTx->CTRL & UART_CTRL_RXNE_Msk) ? 0 : 1;
}

/*******************************************************************************************************************************
* @brief	whether TX FIFO is full, if not, you can write data to it
* @param	UARTx is the UART to query
* @return	1 TX FIFO full, 0 TX FIFO not full
*******************************************************************************************************************************/
uint32_t UART_IsTXFIFOFull(UART_TypeDef * UARTx)
{
	return (UARTx->CTRL & UART_CTRL_TXFF_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	set new baudrate
* @param	UARTx is the UART to set
* @param	baudrate is the new baudrate to use
* @return
*******************************************************************************************************************************/
void UART_SetBaudrate(UART_TypeDef * UARTx, uint32_t baudrate)
{
	UARTx->BAUD &= ~(UART_BAUD_BAUD_Msk | UART_BAUD_FRAC_Msk);
	UARTx->BAUD |= (((SystemCoreClock/baudrate - 1) / 16) << UART_BAUD_BAUD_Pos) |
				   (((SystemCoreClock/baudrate - 1) % 16) << UART_BAUD_FRAC_Pos);
}

/*******************************************************************************************************************************
* @brief	query current baudrate
* @param	UARTx is the UART to query
* @return	current baudrate
*******************************************************************************************************************************/
uint32_t UART_GetBaudrate(UART_TypeDef * UARTx)
{
	return SystemCoreClock/(((UARTx->BAUD & UART_BAUD_BAUD_Msk) >> UART_BAUD_BAUD_Pos) * 16 +
	                        ((UARTx->BAUD & UART_BAUD_FRAC_Msk) >> UART_BAUD_FRAC_Pos) + 1);
}

/*******************************************************************************************************************************
* @brief	UART CTS flow control configure
* @param	UARTx is the UART to configure
* @param	enable is CTS flow control switch
* @param	polarity is CTS signal polarity, 0 data can be sent when CTS is low
* @return
*******************************************************************************************************************************/
void UART_CTSConfig(UART_TypeDef * UARTx, uint32_t enable, uint32_t polarity)
{
	UARTx->CTSCR &= ~(UART_CTSCR_EN_Msk | UART_CTSCR_POL_Msk);
	UARTx->CTSCR |= (enable   << UART_CTSCR_EN_Pos) |
					(polarity << UART_CTSCR_POL_Pos);
}

/*******************************************************************************************************************************
* @brief	UART CTS line state query
* @param	UARTx is the UART to query
* @return	0 CTS is low level, 1 CTS is high level
*******************************************************************************************************************************/
uint32_t UART_CTSLineState(UART_TypeDef * UARTx)
{
	return (UARTx->CTSCR & UART_CTSCR_STAT_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	UART RTS flow control configure
* @param	UARTx is the UART to configure
* @param	enable is RTS flow control switch
* @param	polarity is RTS signal polarity, 0 set RTS to low level when can receive data
* @param	threshold: set RTS active when space in RX FIFO <= threshold, can be UART_RTS_1BYTE, UART_RTS_2BYTE, ...
* @return
*******************************************************************************************************************************/
void UART_RTSConfig(UART_TypeDef * UARTx, uint32_t enable, uint32_t polarity, uint32_t threshold)
{
	UARTx->RTSCR &= ~(UART_RTSCR_EN_Msk | UART_RTSCR_POL_Msk | UART_RTSCR_THR_Msk);
	UARTx->RTSCR |= (enable    << UART_RTSCR_EN_Pos)  |
					(polarity  << UART_RTSCR_POL_Pos) |
					(threshold << UART_RTSCR_THR_Pos);
}

/*******************************************************************************************************************************
* @brief	UART RTS line state query
* @param	UARTx is the UART to query
* @return	0 RTS is low level, 1 RTS is high level
*******************************************************************************************************************************/
uint32_t UART_RTSLineState(UART_TypeDef * UARTx)
{
	return (UARTx->RTSCR & UART_RTSCR_STAT_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	UART LIN function configure
* @param	UARTx is the UART to configure
* @param	detectedLen is low level length that will be recognized as break signal, can be 12--16
* @param	detectedIEn is break signal detected interrupt enable
* @param	generatedLen is low level length of break signal to send, can be 13-15
* @param	generatedIEn is break signal sent done interrupt enable
* @return
*******************************************************************************************************************************/
void UART_LINConfig(UART_TypeDef * UARTx, uint32_t detectedLen, uint32_t detectedIEn, uint32_t generatedLen, uint32_t generatedIEn)
{
	UARTx->CFG &= ~(UART_CFG_BRKTXLEN_Msk | UART_CFG_BRKRXLEN_Msk);
	UARTx->CFG |= ((detectedLen-1) << UART_CFG_BRKRXLEN_Pos) |
				  (generatedLen    << UART_CFG_BRKTXLEN_Pos);
	UARTx->LINCR &= ~(UART_LINCR_BRKDETIE_Msk | UART_LINCR_GENBRKIE_Msk);
	UARTx->LINCR |= (detectedIEn  << UART_LINCR_BRKDETIE_Pos) |
					(generatedIEn << UART_LINCR_GENBRKIE_Pos);
}

/*******************************************************************************************************************************
* @brief	UART LIN break signal send
* @param	UARTx is the UART to send break signal
* @return
*******************************************************************************************************************************/
void UART_LINGenerate(UART_TypeDef * UARTx)
{
	UARTx->LINCR |= (1 << UART_LINCR_GENBRK_Pos);
}

/*******************************************************************************************************************************
* @brief	UART LIN interrupt enable
* @param	UARTx is the UART to set
* @param	it is interrupt type, can be UART_IT_LIN_DET, UART_IT_LIN_GEN and their '|' operation
* @return
*******************************************************************************************************************************/
void UART_LININTEn(UART_TypeDef * UARTx, uint32_t it)
{
	UARTx->LINCR |= it;
}

/*******************************************************************************************************************************
* @brief	UART LIN interrupt disable
* @param	UARTx is the UART to set
* @param	it is interrupt type, can be UART_IT_LIN_DET, UART_IT_LIN_GEN and their '|' operation
* @return
*******************************************************************************************************************************/
void UART_LININTDis(UART_TypeDef * UARTx, uint32_t it)
{
	UARTx->LINCR &= ~it;
}

/*******************************************************************************************************************************
* @brief	UART LIN interrupt flag clear
* @param	UARTx is the UART to set
* @param	it is interrupt type, can be UART_IT_LIN_DET, UART_IT_LIN_GEN and their '|' operation
* @return
*******************************************************************************************************************************/
void UART_LININTClr(UART_TypeDef * UARTx, uint32_t it)
{
	UARTx->LINCR |= (it << 1);
}

/*******************************************************************************************************************************
* @brief	UART LIN interrupt state query
* @param	UARTx is the UART to query
* @param	it is interrupt type, can be UART_IT_LIN_DET, UART_IT_LIN_GEN and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t UART_LININTStat(UART_TypeDef * UARTx, uint32_t it)
{
	return (UARTx->LINCR & (it << 1)) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	UART LIN ID parity calculate
* @param	lin_id is the LIN ID
* @return	parity for lin_id
*******************************************************************************************************************************/
uint8_t UART_LIN_IDParity(uint8_t lin_id)
{
	struct {
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
	} * bits = (void *)&lin_id;
	
	uint8_t id_P0 =  (bits->b0 ^ bits->b1 ^ bits->b2 ^ bits->b4);
	uint8_t id_P1 = ~(bits->b1 ^ bits->b3 ^ bits->b4 ^ bits->b5);
	
	return (lin_id & 0x3F) | (id_P0 << 6) | (id_P1 << 7);
}

/*******************************************************************************************************************************
* @brief	UART LIN Frame checksum calculate
* @param	lin_id is LIN ID
* @param	data is LIN data
* @param	count is LIN data byte count
* @param	enhanced_checksum is LIN 2.0 checksum enable
* @return	checksum for LIN Frame
*******************************************************************************************************************************/
uint8_t UART_LIN_Checksum(uint8_t lin_id, uint8_t data[], uint32_t count, bool enhanced_checksum)
{
	uint16_t checksum;
	
	if(enhanced_checksum && ((lin_id & 0x3F) != 60) && ((lin_id & 0x3F) != 61))
		checksum = lin_id;
	else
		checksum = 0x00;
	
	for(int i = 0; i < count; i++)
	{
		checksum += data[i];
		checksum = (checksum & 0xFF) + (checksum >> 8);
	}
	
	return ~checksum;
}


/*******************************************************************************************************************************
* @brief	automatic baudrate detection start
* @param	UARTx is the UART to set
* @param	detectChar is char used to automatic baudrate detect
*			when UART_DATA_8BIT, can be 0xFF, 0xFE, 0xF8, or 0x80, indicating that the sender must send 0xFF, 0xFE, 0xF8, or 0x80 respectively
*			when UART_DATA_9BIT, can be 0x1FF, 0x1FE, 0x1F8, or 0x180, indicating that the sender must send 0x1FF, 0x1FE, 0x1F8, or 0x180, respectively
* @return
* @note		must use UART_PARITY_NONE when automatic baudrate detection
*******************************************************************************************************************************/
void UART_ABRStart(UART_TypeDef * UARTx, uint32_t detectChar)
{
	uint32_t bits = 0;
	
	if((detectChar == 0xFF) || (detectChar == 0x1FF))      bits = 0;
	else if((detectChar == 0xFE) || (detectChar == 0x1FE)) bits = 1;
	else if((detectChar == 0xF8) || (detectChar == 0x1F8)) bits = 2;
	else if((detectChar == 0x80) || (detectChar == 0x180)) bits = 3;
	else while(1);
	
	UARTx->BAUD &= ~(UART_BAUD_ABREN_Msk | UART_BAUD_ABRBIT_Msk);
	UARTx->BAUD |= (1    << UART_BAUD_ABREN_Pos) |
				   (bits << UART_BAUD_ABRBIT_Pos);
}

/*******************************************************************************************************************************
* @brief	whether the automatic baudrate detection is complete
* @param	UARTx is the UART to query
* @return	0 not complete, UART_ABR_RES_OK complete and success, UART_ABR_RES_ERR complete but fail
*******************************************************************************************************************************/
uint32_t UART_ABRIsDone(UART_TypeDef * UARTx)
{
	if(UARTx->BAUD & UART_BAUD_ABREN_Msk)
	{
		return 0;
	}
	else if(UARTx->BAUD & UART_BAUD_ABRERR_Msk)
	{
		return UART_ABR_RES_ERR;
	}
	else
	{
		return UART_ABR_RES_OK;
	}
}

/*******************************************************************************************************************************
* @brief	UART interrupt enable
* @param	UARTx is the UART to set
* @param	it is interrupt type, can be UART_IT_RX_THR, UART_IT_RX_TOUT, UART_IT_TX_THR, UART_IT_TX_DONE and their '|' operation
* @return
*******************************************************************************************************************************/
void UART_INTEn(UART_TypeDef * UARTx, uint32_t it)
{
	UARTx->CTRL |= it;
}

/*******************************************************************************************************************************
* @brief	UART interrupt disable
* @param	UARTx is the UART to set
* @param	it is interrupt type, can be UART_IT_RX_THR, UART_IT_RX_TOUT, UART_IT_TX_THR, UART_IT_TX_DONE and their '|' operation
* @return
*******************************************************************************************************************************/
void UART_INTDis(UART_TypeDef * UARTx, uint32_t it)
{
	UARTx->CTRL &= ~it;
}

/*******************************************************************************************************************************
* @brief	UART interrupt flag clear
* @param	UARTx is the UART to set
* @param	it is interrupt type, can be UART_IT_RX_THR, UART_IT_RX_TOUT, UART_IT_TX_THR, UART_IT_TX_DONE and their '|' operation
* @return
*******************************************************************************************************************************/
void UART_INTClr(UART_TypeDef * UARTx, uint32_t it)
{
	if(it & UART_IT_RX_TOUT)
		UARTx->TOCR |= UART_TOCR_IFCLR_Msk;
}

/*******************************************************************************************************************************
* @brief	UART interrupt state query
* @param	UARTx is the UART to query
* @param	it is interrupt type, can be UART_IT_RX_THR, UART_IT_RX_TOUT, UART_IT_TX_THR, UART_IT_TX_DONE and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t UART_INTStat(UART_TypeDef * UARTx, uint32_t it)
{
	return (((it & UART_IT_RX_THR)  && (UARTx->BAUD & UART_BAUD_RXIF_Msk)) ||
			((it & UART_IT_RX_TOUT) && (UARTx->BAUD & UART_BAUD_TOIF_Msk)) ||
			((it & UART_IT_TX_THR)  && (UARTx->BAUD & UART_BAUD_TXIF_Msk)) ||
			((it & UART_IT_TX_DONE) && (UARTx->BAUD & UART_BAUD_TXDOIF_Msk)));
}
