/*******************************************************************************************************************************
* @brief	system clock setting
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
#include <stdint.h>
#include "SWM341.h"


/*******************************************************************************************************************************
 * system clock setting
 ******************************************************************************************************************************/
#define SYS_CLK_20MHz		0	 	// 0 internal 20MHz
#define SYS_CLK_2M5Hz		1		// 1 internal 2.5MHz
#define SYS_CLK_40MHz		2		// 2 internal 40MHz
#define SYS_CLK_5MHz		3		// 3 internal  5MHz
#define SYS_CLK_XTAL		4		// 4 external XTAL (4-32MHz)
#define SYS_CLK_XTAL_DIV8	5		// 5 external XTAL (4-32MHz) divide by 8
#define SYS_CLK_PLL			6		// 6 PLL output
#define SYS_CLK_PLL_DIV8	7		// 7 PLL output divide by 8
#define SYS_CLK_32KHz		8		// 8 internal 32KHz
#define SYS_CLK_XTAL_32K	9		// 9 exteranl 32KHz XTAL

#define SYS_CLK   SYS_CLK_PLL


#define __HSI		(20000000UL)	// high speed internal clock
#define __LSI		(   32000UL)	// low speed internal clock
#define __HSE		(12000000UL)	// high speed external clock
#define __LSE		(   32768UL)	// low speed external clock


/********************************** PLL setting ********************************************************************************
 * VCO output frequency = PLL input frequency / INDIV * 4 * FBDIV
 * PLL output frequency = PLL input frequency / INDIV * 4 * FBDIV / OUTDIV = VCO output frequency / OUTDIV
 * note: VCO output frequency shoud be in range [600MHz, 1400MHz]
 ******************************************************************************************************************************/ 
#define SYS_PLL_SRC   	SYS_CLK_XTAL	// SYS_CLK_20MHz or SYS_CLK_XTAL

#define PLL_IN_DIV		3

#define PLL_FB_DIV		60

#define PLL_OUT_DIV		PLL_OUT_DIV8



uint32_t SystemCoreClock  = __HSI;   				// System Clock Frequency (Core Clock)
uint32_t CyclesPerUs      = (__HSI / 1000000); 		// Cycles per micro second


/*******************************************************************************************************************************
* @brief	This function is used to update the variable SystemCoreClock and must be called whenever the core clock is changed
* @param
* @param
* @return
*******************************************************************************************************************************/
void SystemCoreClockUpdate(void)    
{
	if(SYS->CLKSEL & SYS_CLKSEL_SYS_Msk)			//SYS  <= HRC
	{
		if(SYS->HRCCR & SYS_HRCCR_DBL_Msk)				//HRC = 40MHz
		{
			SystemCoreClock = __HSI*2;
		}
		else											//HRC = 20MHz
		{
			SystemCoreClock = __HSI;
		}
	}
	else											//SYS  <= CLK
	{
		switch((SYS->CLKSEL & SYS_CLKSEL_CLK_Msk) >> SYS_CLKSEL_CLK_Pos)
		{
		case 0:
			SystemCoreClock = __LSI;
			break;
		
		case 1:
			if(SYS->PLLCR & SYS_PLLCR_INSEL_Msk)			//PLL_IN <= HRC
			{
				SystemCoreClock = __HSI;
			}
			else											//PLL_IN <= XTAL
			{
				SystemCoreClock = __HSE;
			}
			
			uint32_t indiv  = (SYS->PLLDIV & SYS_PLLDIV_INDIV_Msk)  >> SYS_PLLDIV_INDIV_Pos;
			uint32_t fbdiv  = (SYS->PLLDIV & SYS_PLLDIV_FBDIV_Msk)  >> SYS_PLLDIV_FBDIV_Pos;
			uint32_t outdiv = (SYS->PLLDIV & SYS_PLLDIV_OUTDIV_Msk) >> SYS_PLLDIV_OUTDIV_Pos;
			
			SystemCoreClock = SystemCoreClock / indiv * fbdiv * 4 / (2 << (2 - outdiv));
			break;
		
		case 2:
			SystemCoreClock = __LSE;
			break;
		
		case 3:
			SystemCoreClock = __HSE;
			break;
		
		case 4:
			SystemCoreClock = __HSI;
			if(SYS->HRCCR & SYS_HRCCR_DBL_Msk)  SystemCoreClock *= 2;
			break;
		}
		
		if(SYS->CLKSEL & SYS_CLKSEL_CLK_DIVx_Msk)  SystemCoreClock /= 8;
	}
	
	CyclesPerUs = SystemCoreClock / 1000000;
}

/*******************************************************************************************************************************
* @brief	The necessary initializaiton of systerm
* @param
* @param
* @return
*******************************************************************************************************************************/
void SystemInit(void)
{
	SYS->CLKEN0 |= (1 << SYS_CLKEN0_ANAC_Pos);
	
	Flash_Param_at_xMHz(150);
	
	switch(SYS_CLK)
	{
		case SYS_CLK_20MHz:
			switchTo20MHz();
			break;
		
		case SYS_CLK_2M5Hz:
			switchTo2M5Hz();
			break;
		
		case SYS_CLK_40MHz:
			switchTo40MHz();
			break;
		
		case SYS_CLK_5MHz:
			switchTo5MHz();
			break;
		
		case SYS_CLK_XTAL:
			switchToXTAL(0);
			break;
		
		case SYS_CLK_XTAL_DIV8:
			switchToXTAL(1);
			break;
		
		case SYS_CLK_PLL:
			switchToPLL(SYS_PLL_SRC == SYS_CLK_XTAL, PLL_IN_DIV, PLL_FB_DIV, PLL_OUT_DIV, 0);
			break;
		
		case SYS_CLK_PLL_DIV8:
			switchToPLL(SYS_PLL_SRC == SYS_CLK_XTAL, PLL_IN_DIV, PLL_FB_DIV, PLL_OUT_DIV, 1);
			break;
		
		case SYS_CLK_32KHz:
			switchTo32KHz();
			break;
		
		case SYS_CLK_XTAL_32K:
			switchToXTAL_32K();
			break;
	}
	
	SystemCoreClockUpdate();
	
	Flash_Param_at_xMHz(CyclesPerUs);

	PORTM->PULLD &= ~(1 << PIN1);
	PORTM->PULLU &= ~((1 << PIN2) | (1 << PIN3));
	
	SYS->USBCR |= (1 << SYS_USBCR_RSTPLL_Pos);
	SYS->USBPHYCR &= ~SYS_USBPHYCR_OPMODE_Msk;
	SYS->USBPHYCR |= (1 << SYS_USBPHYCR_OPMODE_Pos);  	//Non-Driving, DP Pull-Up disable
}


void FPU_Enable(void)
{
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= (0xF << 20);
#endif	
}


static void delay_3ms(void)
{
	uint32_t i;
	
	if(((SYS->CLKSEL & SYS_CLKSEL_SYS_Msk) == 0) &&
	   ((((SYS->CLKSEL & SYS_CLKSEL_CLK_Msk) >> SYS_CLKSEL_CLK_Pos) == 0) ||   //LSI 32KHz
	    (((SYS->CLKSEL & SYS_CLKSEL_CLK_Msk) >> SYS_CLKSEL_CLK_Pos) == 2)))	   //LSE 32KHz
	{
		for(i = 0; i < 20; i++) __NOP();
	}
	else
	{
		for(i = 0; i < 20000; i++) __NOP();
	}
}


void switchTo20MHz(void)
{
	SYS->HRCCR = (1 << SYS_HRCCR_ON_Pos) |
				 (0 << SYS_HRCCR_DBL_Pos);			//HRC = 20Hz
	
	delay_3ms();
	
	SYS->CLKSEL |= (1 << SYS_CLKSEL_SYS_Pos);		//SYS <= HRC
}

void switchTo2M5Hz(void)
{
	switchTo20MHz();
	
	SYS->CLKDIVx_ON = 1;
	
	SYS->CLKSEL &= ~SYS_CLKSEL_CLK_Msk;
	SYS->CLKSEL |= (4 << SYS_CLKSEL_CLK_Pos);		//CLK <= HRC

	SYS->CLKSEL |= (1 << SYS_CLKSEL_CLK_DIVx_Pos);
	
	delay_3ms();
	
	SYS->CLKSEL &=~(1 << SYS_CLKSEL_SYS_Pos);		//SYS <= HRC/8
}

void switchTo40MHz(void)
{
	SYS->HRCCR = (1 << SYS_HRCCR_ON_Pos) |
				 (1 << SYS_HRCCR_DBL_Pos);			//HRC = 40MHz
	
	delay_3ms();
	
	SYS->CLKSEL |= (1 << SYS_CLKSEL_SYS_Pos);		//SYS <= HRC
}

void switchTo5MHz(void)
{
	switchTo40MHz();
	
	SYS->CLKDIVx_ON = 1;
	
	SYS->CLKSEL &= ~SYS_CLKSEL_CLK_Msk;
	SYS->CLKSEL |= (4 << SYS_CLKSEL_CLK_Pos);		//CLK <= HRC

	SYS->CLKSEL |= (1 << SYS_CLKSEL_CLK_DIVx_Pos);
	
	delay_3ms();
	
	SYS->CLKSEL &=~(1 << SYS_CLKSEL_SYS_Pos);		//SYS <= HRC/8
}

void switchToXTAL(uint32_t div8)
{
	switchTo20MHz();
	
	PORT_Init(PORTA, PIN3, PORTA_PIN3_XTAL_IN,  0);
	PORT_Init(PORTA, PIN4, PORTA_PIN4_XTAL_OUT, 0);
	SYS->XTALCR |= (1 << SYS_XTALCR_ON_Pos) | (15 << SYS_XTALCR_DRV_Pos) | (1 << SYS_XTALCR_DET_Pos);
	
	SYS->CLKDIVx_ON = 1;
	
	SYS->CLKSEL &= ~SYS_CLKSEL_CLK_Msk;
	SYS->CLKSEL |= (3 << SYS_CLKSEL_CLK_Pos);		//CLK <= XTAL

	if(div8) SYS->CLKSEL |= (1 << SYS_CLKSEL_CLK_DIVx_Pos);
	else     SYS->CLKSEL &=~(1 << SYS_CLKSEL_CLK_DIVx_Pos);
	
	delay_3ms();
	
	SYS->CLKSEL &=~(1 << SYS_CLKSEL_SYS_Pos);		//SYS <= XTAL
}

void switchToPLL(uint32_t clksrc_xtal, uint32_t indiv, uint32_t fbdiv, uint32_t outdiv, uint32_t div8)
{
	switchTo20MHz();
	
	if(clksrc_xtal == 0)
	{
		SYS->HRCCR = (1 << SYS_HRCCR_ON_Pos) |
					 (0 << SYS_HRCCR_DBL_Pos);		//HRC = 20Hz
		
		SYS->PLLCR |= (1 << SYS_PLLCR_INSEL_Pos);	//PLL_SRC <= HRC
	}
	else
	{
		PORT_Init(PORTA, PIN3, PORTA_PIN3_XTAL_IN,  0);
		PORT_Init(PORTA, PIN4, PORTA_PIN4_XTAL_OUT, 0);
		SYS->XTALCR |= (1 << SYS_XTALCR_ON_Pos) | (15 << SYS_XTALCR_DRV_Pos) | (1 << SYS_XTALCR_DET_Pos);
		
		SYS->PLLCR &= ~(1 << SYS_PLLCR_INSEL_Pos);	//PLL_SRC <= XTAL
	}
	
	SYS->PLLDIV &= ~(SYS_PLLDIV_INDIV_Msk |
					 SYS_PLLDIV_FBDIV_Msk |
					 SYS_PLLDIV_OUTDIV_Msk);
	SYS->PLLDIV |= (indiv  << SYS_PLLDIV_INDIV_Pos) |
				   (fbdiv  << SYS_PLLDIV_FBDIV_Pos) |
				   (outdiv << SYS_PLLDIV_OUTDIV_Pos);
	
	SYS->PLLCR &= ~(1 << SYS_PLLCR_OFF_Pos);
	
	while(SYS->PLLLOCK == 0);		// wait for PLL to lock
	
	SYS->PLLCR |= (1 << SYS_PLLCR_OUTEN_Pos);
	
	SYS->CLKDIVx_ON = 1;
	
	SYS->CLKSEL &= ~SYS_CLKSEL_CLK_Msk;
	SYS->CLKSEL |= (1 << SYS_CLKSEL_CLK_Pos);		//CLK <= PLL

	if(div8)  SYS->CLKSEL |= (1 << SYS_CLKSEL_CLK_DIVx_Pos);
	else      SYS->CLKSEL &=~(1 << SYS_CLKSEL_CLK_DIVx_Pos);
	
	SYS->CLKSEL &=~(1 << SYS_CLKSEL_SYS_Pos);		//SYS <= PLL
}

void switchTo32KHz(void)
{
	switchTo20MHz();
	
	SYS->LRCCR = (1 << SYS_LRCCR_ON_Pos);
	
	SYS->CLKDIVx_ON = 1;
	
	SYS->CLKSEL &= ~SYS_CLKSEL_CLK_Msk;
	SYS->CLKSEL |= (0 << SYS_CLKSEL_CLK_Pos);		//CLK <= LRC

	SYS->CLKSEL &=~(1 << SYS_CLKSEL_CLK_DIVx_Pos);
	
	delay_3ms();

	SYS->CLKSEL &=~(1 << SYS_CLKSEL_SYS_Pos);		//SYS <= LRC
}

void switchToXTAL_32K(void)
{
	switchTo20MHz();
	
	SYS->XTALCR |= (1 << SYS_XTALCR_32KON_Pos) | (7 << SYS_XTALCR_32KDRV_Pos) | (1 << SYS_XTALCR_32KDET_Pos);
	
	SYS->CLKDIVx_ON = 1;
	
	SYS->CLKSEL &= ~SYS_CLKSEL_CLK_Msk;
	SYS->CLKSEL |= (2 << SYS_CLKSEL_CLK_Pos);		//CLK <= XTAL_32K

	SYS->CLKSEL &=~(1 << SYS_CLKSEL_CLK_DIVx_Pos);
	
	delay_3ms();

	SYS->CLKSEL &=~(1 << SYS_CLKSEL_SYS_Pos);		//SYS <= XTAL_32K
}
