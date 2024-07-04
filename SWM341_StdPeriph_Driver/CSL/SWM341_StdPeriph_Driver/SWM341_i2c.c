/*******************************************************************************************************************************
* @brief	I2C driver
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
#include "SWM341_i2c.h"


/*******************************************************************************************************************************
* @brief	I2C init
* @param	I2Cx is the I2C to init
* @param	initStruct is data used to init the I2C
* @return
*******************************************************************************************************************************/
void I2C_Init(I2C_TypeDef * I2Cx, I2C_InitStructure * initStruct)
{
	switch((uint32_t)I2Cx)
	{
	case ((uint32_t)I2C0):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_I2C0_Pos);
		break;
	
	case((uint32_t)I2C1):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_I2C1_Pos);
		break;
	}
	
	__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	
	I2C_Close(I2Cx);
	
	if(initStruct->Master == 1)
	{
		int total_clkdiv, clkdiv, scl_hi = 0;
		
		I2Cx->CR |= (1 << I2C_CR_MASTER_Pos);
		
		total_clkdiv = (SystemCoreClock / 2) / initStruct->MstClk;
		if(total_clkdiv < 17)
			total_clkdiv = 17;
		
		do {
			scl_hi++;
			clkdiv = (total_clkdiv - 14) / (scl_hi + scl_hi * 2);
		} while(clkdiv > 256);
		
		I2Cx->CLK = ((scl_hi * 2 - 1) << I2C_CLK_SCLL_Pos) |
					((scl_hi     - 1) << I2C_CLK_SCLH_Pos) |
					((clkdiv     - 1) << I2C_CLK_DIV_Pos);
		
		I2Cx->IF = 0xFFFFFFFF;
		I2Cx->IE = (initStruct->TXEmptyIEn << I2C_IE_TXE_Pos) |
				   (initStruct->RXNotEmptyIEn << I2C_IE_RXNE_Pos);
		
		switch((uint32_t)I2Cx)
		{
		case ((uint32_t)I2C0):		
			if(initStruct->TXEmptyIEn | initStruct->RXNotEmptyIEn)
			{
				NVIC_EnableIRQ(I2C0_IRQn);
			}
			else
			{
				NVIC_DisableIRQ(I2C0_IRQn);
			}
			break;
		
		case ((uint32_t)I2C1):		
			if(initStruct->TXEmptyIEn | initStruct->RXNotEmptyIEn)
			{
				NVIC_EnableIRQ(I2C1_IRQn);
			}
			else
			{
				NVIC_DisableIRQ(I2C1_IRQn);
			}
			break;
		}
	}
	else
	{
		I2Cx->CR &= ~(1 << I2C_CR_MASTER_Pos);
		
		I2Cx->SCR &= ~I2C_SCR_ADDR10_Msk;
		I2Cx->SCR |= (initStruct->Addr10b << I2C_SCR_ADDR10_Pos);
		
		if(initStruct->Addr10b)
			I2Cx->SADDR = (initStruct->SlvAddr    << I2C_SADDR_ADDR10_Pos) |
						  (initStruct->SlvAddrMsk << I2C_SADDR_MASK10_Pos);
		else
			I2Cx->SADDR = (initStruct->SlvAddr    << I2C_SADDR_ADDR7_Pos) |
						  (initStruct->SlvAddrMsk << I2C_SADDR_MASK7_Pos);
		
		I2Cx->IF = 0xFFFFFFFF;
		I2Cx->IE = (initStruct->TXEmptyIEn    << I2C_IE_TXE_Pos) |
				   (initStruct->RXNotEmptyIEn << I2C_IE_RXNE_Pos) |
				   (initStruct->SlvSTADetIEn  << I2C_IE_RXSTA_Pos) |
				   (initStruct->SlvSTODetIEn  << I2C_IE_RXSTO_Pos);
	
		switch((uint32_t)I2Cx)
		{
		case ((uint32_t)I2C0):
			if(initStruct->SlvSTADetIEn | initStruct->SlvSTODetIEn | initStruct->TXEmptyIEn | initStruct->RXNotEmptyIEn)
			{
				NVIC_EnableIRQ(I2C0_IRQn);
			}
			else
			{
				NVIC_DisableIRQ(I2C0_IRQn);
			}
			break;
			
		case ((uint32_t)I2C1):
			if(initStruct->SlvSTADetIEn | initStruct->SlvSTODetIEn | initStruct->TXEmptyIEn | initStruct->RXNotEmptyIEn)
			{
				NVIC_EnableIRQ(I2C1_IRQn);
			}
			else
			{
				NVIC_DisableIRQ(I2C1_IRQn);
			}
			break;
		}
	}
}

/*******************************************************************************************************************************
* @brief	I2C open
* @param	I2Cx is the I2C to open
* @return
*******************************************************************************************************************************/
void I2C_Open(I2C_TypeDef * I2Cx)
{
	I2Cx->CR |= (0x01 << I2C_CR_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	I2C close
* @param	I2Cx is the I2C to close
* @return
*******************************************************************************************************************************/
void I2C_Close(I2C_TypeDef * I2Cx)
{
	I2Cx->CR &= ~(0x01 << I2C_CR_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	generate I2C start signal and then send slave address
* @param	I2Cx is the I2C to use
* @param	addr is the slave address
* @param	wait: 1 wait operation done, 0 immediately return
* @return	1 ACK received, 0 NAK received
* @note		If wait = 0, the returned value is meaningless.
*			Then, you can use I2C_StartDone() to wait the operation done, and then use I2C_IsAck() to get the ack.
*******************************************************************************************************************************/
uint8_t I2C_Start(I2C_TypeDef * I2Cx, uint8_t addr, uint8_t wait)
{
	I2Cx->TXDATA = addr;
	I2Cx->MCR = (1 << I2C_MCR_STA_Pos) |
				(1 << I2C_MCR_WR_Pos);				// send start bit and slave address
	
	if(wait == 0)
		return 0;
	
	while(I2Cx->MCR & I2C_MCR_WR_Msk) __NOP();		// wait for sending to complete
	
	return (I2Cx->TR & I2C_TR_RXACK_Msk) ? 0 : 1;
}

uint8_t I2C_StartDone(I2C_TypeDef * I2Cx)
{
	return (I2Cx->MCR & I2C_MCR_WR_Msk) ? 0 : 1;
}

uint8_t I2C_IsAck(I2C_TypeDef * I2Cx)
{
	return (I2Cx->TR & I2C_TR_RXACK_Msk) ? 0 : 1;
}

/*******************************************************************************************************************************
* @brief	generate I2C stop signal
* @param	I2Cx is the I2C to use
* @param	wait: 1 wait operation done, 0 immediately return
* @return
* @note		If wait = 0, you can use I2C_StopDone() to wait the operation done.
*******************************************************************************************************************************/
void I2C_Stop(I2C_TypeDef * I2Cx, uint8_t wait)
{
	I2Cx->MCR = (1 << I2C_MCR_STO_Pos);
	
	if(wait == 0)
		return;
	
	while(I2Cx->MCR & I2C_MCR_STO_Msk) __NOP();		// wait for sending to complete
}

uint8_t I2C_StopDone(I2C_TypeDef * I2Cx)
{
	return (I2Cx->MCR & I2C_MCR_STO_Msk) ? 0 : 1;
}

/*******************************************************************************************************************************
* @brief	send one byte data
* @param	I2Cx is the I2C to send data out
* @param	data is the byte to send
* @param	wait: 1 wait for sending done, 0 immediately return
* @return	1 ACK received, 0 NAK received
* @note		If wait = 0, the returned value is meaningless.
*			Then, you can use I2C_WriteDone() to wait the operation done, and then use I2C_IsAck() to get the ack.
*******************************************************************************************************************************/
uint8_t I2C_Write(I2C_TypeDef * I2Cx, uint8_t data, uint8_t wait)
{	
	I2Cx->TXDATA = data;
	I2Cx->MCR = (1 << I2C_MCR_WR_Pos);
	
	if(wait == 0)
		return 0;
	
	while(I2Cx->MCR & I2C_MCR_WR_Msk) __NOP();		// wait for sending to complete
	
	return (I2Cx->TR & I2C_TR_RXACK_Msk) ? 0 : 1;
}

uint8_t I2C_WriteDone(I2C_TypeDef * I2Cx)
{
	return (I2Cx->MCR & I2C_MCR_WR_Msk) ? 0 : 1;
}

/*******************************************************************************************************************************
* @brief	read one byte of data
* @param	I2Cx is the I2C to read data from
* @param	ack: 1 send ACK after data received, 0 send NAK after data received
* @param	wait: 1 wait for receiving done, 0 immediately return
* @return	received data
* @note		If wait = 0, the returned value is meaningless.
*			Then, you can use I2C_ReadDone() to wait the operation done, and then use I2Cx->RXDATA to get received data.
*******************************************************************************************************************************/
uint8_t I2C_Read(I2C_TypeDef * I2Cx, uint8_t ack, uint8_t wait)
{
	I2Cx->TR = ((ack ? 0 : 1) << I2C_TR_TXACK_Pos);
	
	I2Cx->MCR = (1 << I2C_MCR_RD_Pos);
	
	if(wait == 0)
		return 0;
	
	while(I2Cx->MCR & I2C_MCR_RD_Msk) __NOP();		// wait for receiving to complete
    
    return I2Cx->RXDATA;
}

uint8_t I2C_ReadDone(I2C_TypeDef * I2Cx)
{
	return (I2Cx->MCR & I2C_MCR_RD_Msk) ? 0 : 1;
}

/*******************************************************************************************************************************
* @brief	I2C interrupt enable
* @param	I2Cx is the I2C to set
* @param	it is interrupt type, can be I2C_IT_TX_EMPTY, I2C_IT_RX_NOT_EMPTY, I2C_IT_RX_OVF, I2C_IT_TX_DONE, I2C_IT_RX_DONE,
*				I2C_IT_SLV_DET_STA, I2C_IT_SLV_DET_STP, I2C_IT_ARB_LOST, I2C_IT_SCL_LOW_TO and their '|' operation
* @return
*******************************************************************************************************************************/
void I2C_INTEn(I2C_TypeDef * I2Cx, uint32_t it)
{
	I2Cx->IE |= it;
}

/*******************************************************************************************************************************
* @brief	I2C interrupt disable
* @param	I2Cx is the I2C to set
* @param	it is interrupt type, can be I2C_IT_TX_EMPTY, I2C_IT_RX_NOT_EMPTY, I2C_IT_RX_OVF, I2C_IT_TX_DONE, I2C_IT_RX_DONE,
*				I2C_IT_SLV_DET_STA, I2C_IT_SLV_DET_STP, I2C_IT_ARB_LOST, I2C_IT_SCL_LOW_TO and their '|' operation
* @return
*******************************************************************************************************************************/
void I2C_INTDis(I2C_TypeDef * I2Cx, uint32_t it)
{
	I2Cx->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	I2C interrupt flag clear
* @param	I2Cx is the I2C to set
* @param	it is interrupt type, can be I2C_IT_TX_EMPTY, I2C_IT_RX_NOT_EMPTY, I2C_IT_RX_OVF, I2C_IT_TX_DONE, I2C_IT_RX_DONE,
*				I2C_IT_SLV_DET_STA, I2C_IT_SLV_DET_STP, I2C_IT_ARB_LOST, I2C_IT_SCL_LOW_TO and their '|' operation
* @return
*******************************************************************************************************************************/
void I2C_INTClr(I2C_TypeDef * I2Cx, uint32_t it)
{
	I2Cx->IF = it;
}

/*******************************************************************************************************************************
* @brief	I2C interrupt state query
* @param	I2Cx is the I2C to query
* @param	it is interrupt type, can be I2C_IT_TX_EMPTY, I2C_IT_RX_NOT_EMPTY, I2C_IT_RX_OVF, I2C_IT_TX_DONE, I2C_IT_RX_DONE,
*				I2C_IT_SLV_DET_STA, I2C_IT_SLV_DET_STP, I2C_IT_ARB_LOST, I2C_IT_SCL_LOW_TO and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t I2C_INTStat(I2C_TypeDef * I2Cx, uint32_t it)
{
	return (I2Cx->IF & it) ? 1 : 0;
}
