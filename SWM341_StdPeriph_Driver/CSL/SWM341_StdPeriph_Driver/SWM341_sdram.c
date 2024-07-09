/*******************************************************************************************************************************
* @brief	SDRAM driver
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
#include "SWM341_sdram.h"


/*******************************************************************************************************************************
* @brief	SDRAM init
* @param	initStruct is data used to init the SDRAM
* @return
*******************************************************************************************************************************/
void SDRAM_Init(SDRAM_InitStructure * initStruct)
{
	uint32_t cyclesPerUs = CyclesPerUs / (initStruct->ClkDiv + 1);	// How many SDRAM_CLK cycles are there in 1us
	uint32_t row_n;
	
	SYS->CLKEN1 |= (1 << SYS_CLKEN1_SDRAM_Pos);
	
	SDRAMC->TIM = (initStruct->TimeTRP  << SDRAMC_TIM_TRP_Pos)  |
				  (initStruct->TimeTRCD << SDRAMC_TIM_TRCD_Pos) |
				  (initStruct->TimeTRC  << SDRAMC_TIM_TRC_Pos)  |
				  ((cyclesPerUs * 200)  << SDRAMC_TIM_T100US_Pos);	// must be greater than 100us
	
	SDRAMC->CFG = (initStruct->Size       << SDRAMC_CFG_SIZE_Pos)     |
				  (initStruct->ClkDiv     << SDRAMC_CFG_CLKDIV_Pos)   |
				  (initStruct->CASLatency << SDRAMC_CFG_CASDELAY_Pos) |
				  (((SystemCoreClock / (initStruct->ClkDiv + 1)) > 66000000) << SDRAMC_CFG_HIGHFREQ_Pos);
	
	switch(initStruct->Size)
	{
	case SDRAM_SIZE_2MB:  row_n = 2048; break;
	case SDRAM_SIZE_8MB:  row_n = 4096; break;
	case SDRAM_SIZE_16MB: row_n = 4096; break;
	case SDRAM_SIZE_32MB: row_n = 8192; break;
	default:              row_n = 4096; break;
	}
	
	SDRAMC->T64 = (initStruct->RefreshTime * 1000 / row_n + 1) * cyclesPerUs;
	
	SDRAMC->CR = (1 << SDRAMC_CR_PWRON_Pos);
	
    while(SDRAMC->CR & SDRAMC_CR_PWRON_Msk) __NOP();	// wait for initial completion
}

/*******************************************************************************************************************************
* @brief	SDRAM enable, then you can read/write SDRAM
* @param
* @return
*******************************************************************************************************************************/
void SDRAM_Enable(void)
{
	SDRAMC->CR &= ~SDRAMC_CR_ENTERSRF_Msk;
	
	for(int i = 0; i < 100; i++) __NOP();
}

/*******************************************************************************************************************************
* @brief	SDRAM disable, then SDRAM enter self-refresh low power mode, cannot read/write
* @param
* @return
*******************************************************************************************************************************/
void SDRAM_Disable(void)
{
	SDRAMC->CR |= SDRAMC_CR_ENTERSRF_Msk;
	
	for(int i = 0; i < 100; i++) __NOP();
}
