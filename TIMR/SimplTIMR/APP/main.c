#include "SWM341.h"


int main(void)
{
	SystemInit();
	
	GPIO_INIT(GPIOA, PIN9, GPIO_OUTPUT);
	
	TIMR_Init(TIMR4, TIMR_MODE_TIMER, CyclesPerUs, 500000, 1);	// An interrupt is triggered every 0.5 seconds
	
	TIMR_Start(TIMR4);
	
	while(1==1)
	{
	}
}

void TIMR4_Handler(void)
{
	TIMR_INTClr(TIMR4, TIMR_IT_TO);
	
	GPIO_InvBit(GPIOA, PIN9);
}
