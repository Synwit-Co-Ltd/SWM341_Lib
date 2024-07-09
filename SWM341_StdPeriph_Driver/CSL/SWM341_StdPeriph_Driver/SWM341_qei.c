/*******************************************************************************************************************************
* @brief	QEI driver
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
#include "SWM341_qei.h"


/*******************************************************************************************************************************
* @brief	QEI init
* @param	QEIx is the QEI to init
* @param	initStruct is data used to init the QEI
* @return
*******************************************************************************************************************************/
void QEI_Init(QEI_TypeDef * QEIx, QEI_InitStructure * initStruct)
{
	switch((uint32_t)QEIx)
	{
	case ((uint32_t)QEI):
		SYS->CLKEN1 |= (1 << SYS_CLKEN1_QEI_Pos);
		break;
	}
	
	QEI_Stop(QEIx);
	
	QEIx->CR = (initStruct->swapAB << QEI_CR_ABSWAP_Pos) |
		       (initStruct->mode   << QEI_CR_X2X4_Pos)   |
		       (1                  << QEI_CR_RSTSRC_Pos) |
		       (1                  << QEI_CR_MODE_Pos);

	QEIx->POSCNT = 0;
	QEIx->MAXCNT = initStruct->maxcnt;

	QEIx->IC = 0x0F;
	QEIx->IE = 0x0F;
	QEIx->IM = (initStruct->intINDEXEn << QEI_IM_INDEX_Pos) |
			   (initStruct->intMATCHEn << QEI_IM_MATCH_Pos) |
			   (initStruct->intCNTOVEn << QEI_IM_CNTOV_Pos) |
			   (initStruct->intERROREn << QEI_IM_ERROR_Pos);

	if(initStruct->intINDEXEn | initStruct->intMATCHEn | initStruct->intCNTOVEn | initStruct->intERROREn)
	{
		NVIC_EnableIRQ(QEI_IRQn);
	}
}

/*******************************************************************************************************************************
* @brief	QEI start
* @param	QEIx is the QEI to start
* @return
*******************************************************************************************************************************/
void QEI_Start(QEI_TypeDef * QEIx)
{
	QEIx->CR |= (1 << QEI_CR_ENA_Pos);
}

/*******************************************************************************************************************************
* @brief	QEI stop
* @param	QEIx is the QEI to stop
* @return
*******************************************************************************************************************************/
void QEI_Stop(QEI_TypeDef * QEIx)
{
	QEIx->CR &= ~(1 << QEI_CR_ENA_Pos);
}

/*******************************************************************************************************************************
* @brief	QEI interrupt enable
* @param	QEIx is the QEI to set
* @param	it is interrupt type, can be QEI_IT_INDEX, QEI_IT_MATCH, QEI_IT_CNTOV, QEI_IT_ERROR and their '|' operation
* @return
*******************************************************************************************************************************/
void QEI_IntEn(QEI_TypeDef * QEIx, uint32_t it)
{
	QEIx->IM |= it;
	
	switch((uint32_t)QEIx)
	{
	case ((uint32_t)QEI):
		NVIC_EnableIRQ(QEI_IRQn);
		break;
	}
}

/*******************************************************************************************************************************
* @brief	QEI interrupt disable
* @param	QEIx is the QEI to set
* @param	it is interrupt type, can be QEI_IT_INDEX, QEI_IT_MATCH, QEI_IT_CNTOV, QEI_IT_ERROR and their '|' operation
* @return
*******************************************************************************************************************************/
void QEI_IntDis(QEI_TypeDef * QEIx, uint32_t it)
{
	QEIx->IM &= ~it;
}

/*******************************************************************************************************************************
* @brief	QEI interrupt flag clear
* @param	QEIx is the QEI to clear
* @param	it is interrupt type, can be QEI_IT_INDEX, QEI_IT_MATCH, QEI_IT_CNTOV, QEI_IT_ERROR and their '|' operation
* @return
*******************************************************************************************************************************/
void QEI_IntClr(QEI_TypeDef * QEIx, uint32_t it)
{
	QEIx->IC = it;
}

/*******************************************************************************************************************************
* @brief	QEI interrupt state query
* @param	QEIx is the QEI to query
* @param	it is interrupt type, can be QEI_IT_INDEX, QEI_IT_MATCH, QEI_IT_CNTOV, QEI_IT_ERROR and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t QEI_IntStat(QEI_TypeDef * QEIx, uint32_t it)
{
	return (QEIx->IF & it);
}
