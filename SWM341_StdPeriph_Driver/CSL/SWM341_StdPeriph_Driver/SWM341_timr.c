/*******************************************************************************************************************************
* @brief	Timer driver
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
#include "SWM341_timr.h"


/*******************************************************************************************************************************
* @brief	Timer init
* @param	TIMRx is timer to init, can be TIMR0~4 and BTIMR0~11
* @param	mode for TIMR0~4 can be TIMR_MODE_TIMER, TIMR_MODE_COUNTER, TIMR_MODE_OC and TIMR_MODE_IC
*			mode for BTIMR0~11 can be TIMR_MODE_TIMER and TIMR_MODE_OC
* @param	prediv is prescale for counting clock, can be 1-256
* @param	period is counting period, can be 1-16777216
* @param	int_en is interrupt enable
* @return
*******************************************************************************************************************************/
void TIMR_Init(TIMR_TypeDef * TIMRx, uint32_t mode, uint16_t prediv, uint32_t period, uint32_t int_en)
{
	if((TIMRx == TIMR0) || (TIMRx == TIMR1) || (TIMRx == TIMR2) || (TIMRx == TIMR3) || (TIMRx == TIMR4))
	{
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_TIMR_Pos);
	}
	else
	{
		SYS->CLKEN1 |= (0x01 << SYS_CLKEN1_BTIMR_Pos);
	}
	
	TIMR_Stop(TIMRx);
	
	TIMRx->CR &= ~(TIMR_CR_MODE_Msk | TIMR_CR_CLKSRC_Msk);
	TIMRx->CR |= (mode << TIMR_CR_CLKSRC_Pos);
	
	TIMRx->PREDIV = prediv - 1;
	
	TIMRx->LOAD = period - 1;
	
	TIMRx->IF = (1 << TIMR_IF_TO_Pos);		// clear interrupt flag
	if(int_en) TIMRx->IE |=  (1 << TIMR_IE_TO_Pos);
	else	   TIMRx->IE &= ~(1 << TIMR_IE_TO_Pos);
	
	switch((uint32_t)TIMRx)
	{
	case ((uint32_t)TIMR0):
		if(int_en) NVIC_EnableIRQ(TIMR0_IRQn);
		break;
	
	case ((uint32_t)TIMR1):
		if(int_en) NVIC_EnableIRQ(TIMR1_IRQn);
		break;
	
	case ((uint32_t)TIMR2):
		if(int_en) NVIC_EnableIRQ(TIMR2_IRQn);
		break;
	
	case ((uint32_t)TIMR3):
		if(int_en) NVIC_EnableIRQ(TIMR3_IRQn);
		break;
	
	case ((uint32_t)TIMR4):
		if(int_en) NVIC_EnableIRQ(TIMR4_IRQn);
		break;
	
	case ((uint32_t)BTIMR0):
		if(int_en) NVIC_EnableIRQ(BTIMR0_IRQn);
		break;
	
	case ((uint32_t)BTIMR1):
		if(int_en) NVIC_EnableIRQ(BTIMR1_IRQn);
		break;
	
	case ((uint32_t)BTIMR2):
		if(int_en) NVIC_EnableIRQ(BTIMR2_IRQn);
		break;
	
	case ((uint32_t)BTIMR3):
		if(int_en) NVIC_EnableIRQ(BTIMR3_IRQn);
		break;
	
	case ((uint32_t)BTIMR4):
		if(int_en) NVIC_EnableIRQ(BTIMR4_IRQn);
		break;
	
	case ((uint32_t)BTIMR5):
		if(int_en) NVIC_EnableIRQ(BTIMR5_IRQn);
		break;
	
	case ((uint32_t)BTIMR6):
		if(int_en) NVIC_EnableIRQ(BTIMR6_IRQn);
		break;
	
	case ((uint32_t)BTIMR7):
		if(int_en) NVIC_EnableIRQ(BTIMR7_IRQn);
		break;
	
	case ((uint32_t)BTIMR8):
		if(int_en) NVIC_EnableIRQ(BTIMR8_IRQn);
		break;
	
	case ((uint32_t)BTIMR9):
		if(int_en) NVIC_EnableIRQ(BTIMR9_IRQn);
		break;
	
	case ((uint32_t)BTIMR10):
		if(int_en) NVIC_EnableIRQ(BTIMR10_IRQn);
		break;
	
	case ((uint32_t)BTIMR11):
		if(int_en) NVIC_EnableIRQ(BTIMR11_IRQn);
		break;
	}
}

/*******************************************************************************************************************************
* @brief	Timer start
* @param	TIMRx is timer to start, can be TIMR0~4 and BTIMR0~11
* @return
*******************************************************************************************************************************/
void TIMR_Start(TIMR_TypeDef * TIMRx)
{
	switch((uint32_t)TIMRx)
	{
	case ((uint32_t)TIMR0):
		TIMRG->EN |= (1 << TIMRG_EN_TIMR0_Pos);
		break;
	
	case ((uint32_t)TIMR1):
		TIMRG->EN |= (1 << TIMRG_EN_TIMR1_Pos);
		break;
	
	case ((uint32_t)TIMR2):
		TIMRG->EN |= (1 << TIMRG_EN_TIMR2_Pos);
		break;
	
	case ((uint32_t)TIMR3):
		TIMRG->EN |= (1 << TIMRG_EN_TIMR3_Pos);
		break;
	
	case ((uint32_t)TIMR4):
		TIMRG->EN |= (1 << TIMRG_EN_TIMR4_Pos);
		break;
	
	case ((uint32_t)BTIMR0):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR0_Pos);
		break;
	
	case ((uint32_t)BTIMR1):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR1_Pos);
		break;
	
	case ((uint32_t)BTIMR2):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR2_Pos);
		break;
	
	case ((uint32_t)BTIMR3):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR3_Pos);
		break;
	
	case ((uint32_t)BTIMR4):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR4_Pos);
		break;
	
	case ((uint32_t)BTIMR5):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR5_Pos);
		break;
	
	case ((uint32_t)BTIMR6):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR6_Pos);
		break;
	
	case ((uint32_t)BTIMR7):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR7_Pos);
		break;
	
	case ((uint32_t)BTIMR8):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR8_Pos);
		break;
	
	case ((uint32_t)BTIMR9):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR9_Pos);
		break;
	
	case ((uint32_t)BTIMR10):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR10_Pos);
		break;
	
	case ((uint32_t)BTIMR11):
		BTIMRG->EN |= (1 << TIMRG_EN_TIMR11_Pos);
		break;
	}
}

/*******************************************************************************************************************************
* @brief	Timer stop
* @param	TIMRx is timer to stop, can be TIMR0~4 and BTIMR0~11
* @return
*******************************************************************************************************************************/
void TIMR_Stop(TIMR_TypeDef * TIMRx)
{
	switch((uint32_t)TIMRx)
	{
	case ((uint32_t)TIMR0):
		TIMRG->EN &= ~(1 << TIMRG_EN_TIMR0_Pos);
		break;
	
	case ((uint32_t)TIMR1):
		TIMRG->EN &= ~(1 << TIMRG_EN_TIMR1_Pos);
		break;
	
	case ((uint32_t)TIMR2):
		TIMRG->EN &= ~(1 << TIMRG_EN_TIMR2_Pos);
		break;
	
	case ((uint32_t)TIMR3):
		TIMRG->EN &= ~(1 << TIMRG_EN_TIMR3_Pos);
		break;
	
	case ((uint32_t)TIMR4):
		TIMRG->EN &= ~(1 << TIMRG_EN_TIMR4_Pos);
		break;
	
	case ((uint32_t)BTIMR0):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR0_Pos);
		break;
	
	case ((uint32_t)BTIMR1):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR1_Pos);
		break;
	
	case ((uint32_t)BTIMR2):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR2_Pos);
		break;
	
	case ((uint32_t)BTIMR3):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR3_Pos);
		break;
	
	case ((uint32_t)BTIMR4):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR4_Pos);
		break;
	
	case ((uint32_t)BTIMR5):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR5_Pos);
		break;
	
	case ((uint32_t)BTIMR6):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR6_Pos);
		break;
	
	case ((uint32_t)BTIMR7):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR7_Pos);
		break;
	
	case ((uint32_t)BTIMR8):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR8_Pos);
		break;
	
	case ((uint32_t)BTIMR9):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR9_Pos);
		break;
	
	case ((uint32_t)BTIMR10):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR10_Pos);
		break;
	
	case ((uint32_t)BTIMR11):
		BTIMRG->EN &= ~(1 << TIMRG_EN_TIMR11_Pos);
		break;
	}
}

/*******************************************************************************************************************************
* @brief	Timer halt, counter keep not change
* @param	TIMRx is timer to halt, can be TIMR0~4
* @return
*******************************************************************************************************************************/
void TIMR_Halt(TIMR_TypeDef * TIMRx)
{
	TIMRx->HALT = 1;
}

/*******************************************************************************************************************************
* @brief	Timer resume, continue counting from the halt
* @param	TIMRx is timer to resume, can be TIMR0~4
* @return
*******************************************************************************************************************************/
void TIMR_Resume(TIMR_TypeDef * TIMRx)
{
	TIMRx->HALT = 0;
}

/*******************************************************************************************************************************
* @brief	Timer current count value query
* @param	TIMRx is timer to query, can be TIMR0~4 and BTIMR0~11
* @return	current count value
*******************************************************************************************************************************/
uint32_t TIMR_GetCurValue(TIMR_TypeDef * TIMRx)
{
	return TIMRx->VALUE;
}

/*******************************************************************************************************************************
* @brief	Timer interrupt enable
* @param	TIMRx is timer to set, can be TIMR0~4 and BTIMR0~11
* @param	it for TIMR0~4 can be TIMR_IT_TO, TIMR_IT_OC, TIMR_IT_IC_LOW, TIMR_IT_IC_HIGH and their '|' operation
*			it for BTIMR0~11 can be TIMR_IT_TO, TIMR_IT_OC and their '|' operation
* @return
*******************************************************************************************************************************/
void TIMR_INTEn(TIMR_TypeDef * TIMRx, uint32_t it)
{
	TIMRx->IE |= it;
}

/*******************************************************************************************************************************
* @brief	Timer interrupt disable
* @param	TIMRx is timer to set, can be TIMR0~4 and BTIMR0~11
* @param	it for TIMR0~4 can be TIMR_IT_TO, TIMR_IT_OC, TIMR_IT_IC_LOW, TIMR_IT_IC_HIGH and their '|' operation
*			it for BTIMR0~11 can be TIMR_IT_TO, TIMR_IT_OC and their '|' operation
* @return
*******************************************************************************************************************************/
void TIMR_INTDis(TIMR_TypeDef * TIMRx, uint32_t it)
{
	TIMRx->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	Timer interrupt clear
* @param	TIMRx is timer to set, can be TIMR0~4 and BTIMR0~11
* @param	it for TIMR0~4 can be TIMR_IT_TO, TIMR_IT_OC, TIMR_IT_IC_LOW, TIMR_IT_IC_HIGH and their '|' operation
*			it for BTIMR0~11 can be TIMR_IT_TO, TIMR_IT_OC and their '|' operation
* @return
*******************************************************************************************************************************/
void TIMR_INTClr(TIMR_TypeDef * TIMRx, uint32_t it)
{
	TIMRx->IF = it;
}

/*******************************************************************************************************************************
* @brief	Timer interrupt state query
* @param	TIMRx is timer to set, can be TIMR0~4 and BTIMR0~11
* @param	it for TIMR0~4 can be TIMR_IT_TO, TIMR_IT_OC, TIMR_IT_IC_LOW, TIMR_IT_IC_HIGH and their '|' operation
*			it for BTIMR0~11 can be TIMR_IT_TO, TIMR_IT_OC and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t TIMR_INTStat(TIMR_TypeDef * TIMRx, uint32_t it)
{
	return (TIMRx->IF & it) ? 1 : 0;
}


/*******************************************************************************************************************************
* @brief	Timer output compare function init
* @param	TIMRx is timer to set, can be TIMR0~4 and BTIMR0~11
* @param	match: output level flip when the counter value decrease to match
* @param	match_int_en: whether generate interrupt when the counter value decrease to match
* @param	init_lvl is initial output level
* @return
*******************************************************************************************************************************/
void TIMR_OC_Init(TIMR_TypeDef * TIMRx, uint32_t match, uint32_t match_int_en, uint32_t init_lvl)
{
	TIMRx->OCMAT = match;
	if(init_lvl) TIMRx->OCCR |=  (1 << TIMR_OCCR_INITLVL_Pos);
	else         TIMRx->OCCR &= ~(1 << TIMR_OCCR_INITLVL_Pos);
	
	TIMRx->IF = (1 << TIMR_IF_OC0_Pos);		// clear interrupt flag
	if(match_int_en) TIMRx->IE |=  (1 << TIMR_IE_OC0_Pos);
	else             TIMRx->IE &= ~(1 << TIMR_IE_OC0_Pos);
	
	switch((uint32_t)TIMRx)
	{
	case ((uint32_t)TIMR0):
		if(match_int_en) NVIC_EnableIRQ(TIMR0_IRQn);
		break;
	
	case ((uint32_t)TIMR1):
		if(match_int_en) NVIC_EnableIRQ(TIMR1_IRQn);
		break;
	
	case ((uint32_t)TIMR2):
		if(match_int_en) NVIC_EnableIRQ(TIMR2_IRQn);
		break;
	
	case ((uint32_t)TIMR3):
		if(match_int_en) NVIC_EnableIRQ(TIMR3_IRQn);
		break;
	
	case ((uint32_t)TIMR4):
		if(match_int_en) NVIC_EnableIRQ(TIMR4_IRQn);
		break;
	
	case ((uint32_t)BTIMR0):
		if(match_int_en) NVIC_EnableIRQ(BTIMR0_IRQn);
		break;
	
	case ((uint32_t)BTIMR1):
		if(match_int_en) NVIC_EnableIRQ(BTIMR1_IRQn);
		break;
	
	case ((uint32_t)BTIMR2):
		if(match_int_en) NVIC_EnableIRQ(BTIMR2_IRQn);
		break;
	
	case ((uint32_t)BTIMR3):
		if(match_int_en) NVIC_EnableIRQ(BTIMR3_IRQn);
		break;
	
	case ((uint32_t)BTIMR4):
		if(match_int_en) NVIC_EnableIRQ(BTIMR4_IRQn);
		break;
	
	case ((uint32_t)BTIMR5):
		if(match_int_en) NVIC_EnableIRQ(BTIMR5_IRQn);
		break;
	
	case ((uint32_t)BTIMR6):
		if(match_int_en) NVIC_EnableIRQ(BTIMR6_IRQn);
		break;
	
	case ((uint32_t)BTIMR7):
		if(match_int_en) NVIC_EnableIRQ(BTIMR7_IRQn);
		break;
	
	case ((uint32_t)BTIMR8):
		if(match_int_en) NVIC_EnableIRQ(BTIMR8_IRQn);
		break;
	
	case ((uint32_t)BTIMR9):
		if(match_int_en) NVIC_EnableIRQ(BTIMR9_IRQn);
		break;
	
	case ((uint32_t)BTIMR10):
		if(match_int_en) NVIC_EnableIRQ(BTIMR10_IRQn);
		break;
	
	case ((uint32_t)BTIMR11):
		if(match_int_en) NVIC_EnableIRQ(BTIMR11_IRQn);
		break;
	}
}

/*******************************************************************************************************************************
* @brief	Timer output enable
* @param	TIMRx is timer to set, can be TIMR0~4 and BTIMR0~11
* @return
*******************************************************************************************************************************/
void TIMR_OC_OutputEn(TIMR_TypeDef * TIMRx)
{
	TIMRx->OCCR &= ~(TIMR_OCCR_FORCEEN_Pos);
}

/*******************************************************************************************************************************
* @brief	Timer output disable
* @param	TIMRx is timer to set, can be TIMR0~4 and BTIMR0~11
* @param	level is pin keep level after timer output disable
* @return
*******************************************************************************************************************************/
void TIMR_OC_OutputDis(TIMR_TypeDef * TIMRx, uint32_t level)
{
	if(level) TIMRx->OCCR |=  (1 << TIMR_OCCR_FORCELVL_Pos);
	else      TIMRx->OCCR &= ~(1 << TIMR_OCCR_FORCELVL_Pos);
	
	TIMRx->OCCR |= (TIMR_OCCR_FORCEEN_Pos);
}

/*******************************************************************************************************************************
* @brief	Timer match value set
* @param	TIMRx is timer to set, can be TIMR0~4 and BTIMR0~11
* @param	match is match value
* @return
*******************************************************************************************************************************/
void TIMR_OC_SetMatch(TIMR_TypeDef * TIMRx, uint32_t match)
{
	TIMRx->OCMAT = match;
}

/*******************************************************************************************************************************
* @brief	Timer match value query
* @param	TIMRx is timer to query, can be TIMR0~4 and BTIMR0~11
* @return	match value
*******************************************************************************************************************************/
uint32_t TIMR_OC_GetMatch(TIMR_TypeDef * TIMRx)
{
	return TIMRx->OCMAT;
}


/*******************************************************************************************************************************
* @brief	Timer input capture init
* @param	TIMRx is timer to init, can be TIMR0~4
* @param	captureH_int_en is high level capture interrupt enable
* @param	captureL_int_en is low level capture interrupt enable
* @return
*******************************************************************************************************************************/
void TIMR_IC_Init(TIMR_TypeDef * TIMRx, uint32_t captureH_int_en, uint32_t captureL_int_en)
{
	TIMRx->IF = (TIMR_IF_ICR_Msk | TIMR_IF_ICF_Msk);
	if(captureH_int_en) TIMRx->IE |=  (1 << TIMR_IE_ICF_Pos);
	else                TIMRx->IE &= ~(1 << TIMR_IE_ICF_Pos);
	if(captureL_int_en) TIMRx->IE |=  (1 << TIMR_IE_ICR_Pos);
	else                TIMRx->IE &= ~(1 << TIMR_IE_ICR_Pos);
	
	switch((uint32_t)TIMRx)
	{
	case ((uint32_t)TIMR0):
		if(captureH_int_en | captureL_int_en) NVIC_EnableIRQ(TIMR0_IRQn);
		break;
	
	case ((uint32_t)TIMR1):
		if(captureH_int_en | captureL_int_en) NVIC_EnableIRQ(TIMR1_IRQn);
		break;
	
	case ((uint32_t)TIMR2):
		if(captureH_int_en | captureL_int_en) NVIC_EnableIRQ(TIMR2_IRQn);
		break;
	
	case ((uint32_t)TIMR3):
		if(captureH_int_en | captureL_int_en) NVIC_EnableIRQ(TIMR3_IRQn);
		break;
	
	case ((uint32_t)TIMR4):
		if(captureH_int_en | captureL_int_en) NVIC_EnableIRQ(TIMR4_IRQn);
		break;
	}
}

/*******************************************************************************************************************************
* @brief	Timer input capture high level length result query
* @param	TIMRx is timer to init, can be TIMR0~4
* @return	high level length
*******************************************************************************************************************************/
uint32_t TIMR_IC_GetCaptureH(TIMR_TypeDef * TIMRx)
{
	return TIMRx->ICHIGH;
}

/*******************************************************************************************************************************
* @brief	Timer input capture low level length result query
* @param	TIMRx is timer to init, can be TIMR0~4
* @return	low level length
*******************************************************************************************************************************/
uint32_t TIMR_IC_GetCaptureL(TIMR_TypeDef * TIMRx)
{
	return TIMRx->ICLOW;
}
