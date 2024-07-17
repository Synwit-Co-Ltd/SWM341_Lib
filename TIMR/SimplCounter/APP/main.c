#include "SWM341.h"


int main(void)
{
	SystemInit();
	
	GPIO_INIT(GPIOA, PIN0, GPIO_OUTPUT);
	GPIO_INIT(GPIOA, PIN5, GPIO_OUTPUT);
	
	PORT_Init(PORTB, PIN9, PORTB_PIN9_TIMR2_IN, 1);		// connect PA0 pin and count the rising edge on PA0 pin
	
	TIMR_Init(TIMR2, TIMR_MODE_COUNTER, 1, 5, 1);		// every 5 rising edges trigger an interrupt
	TIMR_Start(TIMR2);
	
	while(1==1)
	{
		GPIO_InvBit(GPIOA, PIN0);
		for(int i = 0; i < SystemCoreClock/64; i++) __NOP();
	}
}

void TIMR2_Handler(void)
{
	TIMR_INTClr(TIMR2, TIMR_IT_TO);
	
	GPIO_InvBit(GPIOA, PIN5);
}
