#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);		// output, connect a LED
	
	SysTick_Config(0x1000000);				// Note: SysTick counter is 24-bit width and SysTick will not be configured if argment bigger than 0x1000000.
	
	while(1==1)
	{
	}
}

void SysTick_Handler(void)
{	
	GPIO_InvBit(GPIOA, PIN9);
}

