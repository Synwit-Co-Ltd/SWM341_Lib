#include "SWM341.h"


void led_init(void)
{
	GPIO_INIT(GPIOA, PIN0, GPIO_OUTPUT);
}

void led_switch(int on)
{
	if(on)
		GPIO_SetBit(GPIOA, PIN0);
	else
		GPIO_ClrBit(GPIOA, PIN0);
}
