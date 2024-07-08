/*******************************************************************************************************************************
* @brief	DIV driver, DIV is a hardware divider that can do the square root calculating
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
#include "SWM341_div.h"


/*******************************************************************************************************************************
* @brief	DIV init
* @param	DIVx is the DIV to init
* @param	initStruct is data used to init the DIV
* @return
*******************************************************************************************************************************/
void DIV_Init(DIV_TypeDef * DIVx)
{
	switch((uint32_t)DIVx)
	{
	case ((uint32_t)DIV):
		SYS->CLKEN0 |= (0x01u<< SYS_CLKEN0_DIV_Pos);
		break;
	}
}
