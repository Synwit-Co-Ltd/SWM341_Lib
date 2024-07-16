#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);			// output, connect LED
	
	GPIO_Init(GPIOA, PIN2, 0, 1, 0, 0);			// input, enable pull-up, connect key
	
	EXTI_Init(GPIOA, PIN2, EXTI_FALL_EDGE);		// falling edge on PA2 generates an interrupt
	
//	NVIC_EnableIRQ(GPIOA_IRQn);
	
	EXTI_Open(GPIOA, PIN2);
	
	while(1==1)
	{
	}
}

void NMI_Handler(void)
{
	EXTI_Clear(GPIOA, PIN2);
	
	GPIO_InvBit(GPIOA, PIN9);
}

void GPIOA_Handler(void)
{
	if(EXTI_State(GPIOA, PIN2))
	{
		EXTI_Clear(GPIOA, PIN2);	// When GPIOA interrupt is enabled, the falling edge of PA2 triggers both NMI interrupt and GPIOA interrupt
	}
}
