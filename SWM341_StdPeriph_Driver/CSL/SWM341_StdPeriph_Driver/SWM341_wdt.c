/*******************************************************************************************************************************
* @brief	WDT driver
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
#include "SWM341_wdt.h"


/*******************************************************************************************************************************
* @brief	WDT init
* @param	WDTx is the WDT to init
* @param	int_period, interrupt period, range from 1 to pow(2, 16) in unit 1/1024 second. value 0 disable interrupt function
* @param	rst_period, reset period, range from 1 to pow(2, 16) in unit 1/1024 second. value 0 disable reset function
* @return
* @note		can only be called once after the chip is powered on. To reconfigure WDT, use WDT_ReInit().
*******************************************************************************************************************************/
void WDT_Init(WDT_TypeDef * WDTx, uint32_t int_period, uint32_t rst_period)
{
	SYS->CLKEN0 |= (1 << SYS_CLKEN0_WDT_Pos);
	
	WDTx->CR &= ~WDT_CR_CKDIV_Msk;
	WDTx->CR |= (4 << WDT_CR_CKDIV_Pos);	// divide the frequency of the clock source by 32
	
	if(int_period == 0)
	{
		WDTx->CR &= ~(1 << WDT_CR_INTEN_Pos);
		
		NVIC_DisableIRQ(WDT_IRQn);
	}
	else
	{
		WDTx->CR |=  (1 << WDT_CR_INTEN_Pos);
		
		WDTx->INTVAL = int_period;
		
		WDTx->IF = 1;
		NVIC_EnableIRQ(WDT_IRQn);
	}
	
	if(rst_period == 0)
	{
		WDTx->CR &= ~(1 << WDT_CR_RSTEN_Pos);
		
		WDTx->RSTVAL = int_period;
	}
	else
	{
		WDTx->CR |=  (1 << WDT_CR_RSTEN_Pos);
		
		WDTx->RSTVAL = rst_period;
	}
}

/*******************************************************************************************************************************
* @brief	WDT reinit
* @param	WDTx is the WDT to reinit
* @param	int_period, same as WDT_Init()
* @param	rst_period, same as WDT_Init()
* @return
* @note		not execute WDT_Stop() before WDT_ReInit(), because the internal counter cannot be cleared when WDT is stopped
*******************************************************************************************************************************/
void WDT_ReInit(WDT_TypeDef * WDTx, uint32_t int_period, uint32_t rst_period)
{
	int i;
	
	/* If rst_period smaller than current counter value, WDT needs to count to 2^16 to trigger interrupt and reset.
	   Perform a dog feeding operation to ensure that the counter is re-counted from zero to avoid the above problems. */
	WDT_Feed(WDTx);
	
	/* wait for internal dog feeding operation to complete and the counter to clear to zero */
	for(i = 0; i < CyclesPerUs * 300 / 4; i++) __NOP();
	
	WDT_Stop(WDTx);
	
	WDT_Init(WDTx, int_period, rst_period);
}

/*******************************************************************************************************************************
* @brief	WDT start
* @param	WDTx is the WDT to start
* @return
*******************************************************************************************************************************/
void WDT_Start(WDT_TypeDef * WDTx)
{
	WDTx->CR |=  (1 << WDT_CR_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	WDT stop
* @param	WDTx is the WDT to stop
* @return
*******************************************************************************************************************************/
void WDT_Stop(WDT_TypeDef * WDTx)
{
	WDTx->CR &= ~(1 << WDT_CR_EN_Pos);
}

/*******************************************************************************************************************************
* @brief	WDT feed
* @param	WDTx is the WDT to feed
* @return
*******************************************************************************************************************************/
void WDT_Feed(WDT_TypeDef * WDTx)
{
	if(WDTx->CR & WDT_CR_EN_Msk) // When the WDT is stopped, don't feed it
		WDTx->FEED = 0x55;
}

/*******************************************************************************************************************************
* @brief	WDT interrupt flag clear
* @param	WDTx is the WDT to clear
* @return
*******************************************************************************************************************************/
void WDT_INTClr(WDT_TypeDef * WDTx)
{
	WDTx->IF = 1;
}

/*******************************************************************************************************************************
* @brief	WDT interrupt state query
* @param	WDTx is the WDT to query
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t WDT_INTStat(WDT_TypeDef * WDTx)
{
	return WDTx->IF;
}
