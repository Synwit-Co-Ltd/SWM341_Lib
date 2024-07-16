#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);		// output, connect a LED
	
	SYS->BODCR = (1 << SYS_BODCR_IE_Pos) | (3 << SYS_BODCR_INTLVL_Pos);		// enable interrupt, interrupt trigger level is 2.5v
	SYS->BODSR = (1 << SYS_BODSR_IF_Pos);	// clear interrupt flag
	NVIC_EnableIRQ(BOD_IRQn);
	
	while(1==1)
	{
		GPIO_ClrBit(GPIOA, PIN9);
	}
}

void BOD_Handler(void)
{
	SYS->BODSR = (1 << SYS_BODSR_IF_Pos);
	
	GPIO_SetBit(GPIOA, PIN9);
}
