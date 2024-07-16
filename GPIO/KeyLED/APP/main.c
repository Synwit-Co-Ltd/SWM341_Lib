#include "SWM341.h"


/* 	Press the button connected to PA4, and the LED connected to PA5 will light up;
	Release the button and the LED goes off!
*/


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN10, 0, 1, 0, 0);			// input, pull-up enable, connect key
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);				// output, connect LED
	
	while(1==1)
	{
		if(GPIO_GetBit(GPIOA, PIN10) == 0)			// key pressed
		{
			GPIO_SetBit(GPIOA, PIN9);
		}
		else
		{
			GPIO_ClrBit(GPIOA, PIN9);
		}
	}
}

