/*******************************************************************************************************************************
* @brief	IOFILT driver, signal filtering between PAD and module input, pulses narrower than the specified width are ignored
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
#include "SWM341_iofilt.h"


static uint32_t Width[2];


/*******************************************************************************************************************************
* @brief	IOFILT init
* @param	IOFILTn is the IOFILT to init, can be 0 and 1
* @param	signal select which signal to filter, can be IOFILT0_PB5, IOFILT0_PB4, ...
* @param	width: pulses narrower than the this width (in unit of HCLK period) are treated as burrs and filtered out
* @return
*******************************************************************************************************************************/
void IOFILT_Init(uint32_t IOFILTn, uint32_t signal, uint32_t width)
{
	SYS->CLKSEL &= ~SYS_CLKSEL_IOFILT_Msk;
	SYS->CLKSEL |= (0 << SYS_CLKSEL_IOFILT_Pos);	// clock source: HRC
	
	SYS->CLKEN1 |= (1 << SYS_CLKEN1_IOFILT_Pos);
	
	*(&SYS->IOFILT0 + IOFILTn) = (signal << SYS_IOFILT_IOSEL_Pos)  |
								 (0      << SYS_IOFILT_CLKDIV_Pos) |
								 (width  << SYS_IOFILT_TIM_Pos);
	
	Width[IOFILTn] = width;
}

/*******************************************************************************************************************************
* @brief	IOFILT open
* @param	IOFILTn is the IOFILT to open, can be 0 and 1
* @return
*******************************************************************************************************************************/
void IOFILT_Open(uint32_t IOFILTn)
{
	*(&SYS->IOFILT0 + IOFILTn) |= (Width[IOFILTn] << SYS_IOFILT_TIM_Pos);
}

/*******************************************************************************************************************************
* @brief	IOFILT close, and then the signal on the PAD goes directly into the module
* @param	IOFILTn is the IOFILT to close, can be 0 and 1
* @return
*******************************************************************************************************************************/
void IOFILT_Close(uint32_t IOFILTn)
{
	*(&SYS->IOFILT0 + IOFILTn) &= ~SYS_IOFILT_TIM_Msk;
}
