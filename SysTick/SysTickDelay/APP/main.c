#include "SWM341.h"


uint32_t SysTick_Delayus(uint32_t us);

int main(void)
{
	SystemInit();
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);		// OUTPUT, connect to a LED
	
	while(1==1)
	{
		GPIO_InvBit(GPIOA, PIN9);
		SysTick_Delayus(100000);
		SysTick_Delayus(100000);
		SysTick_Delayus(100000);
		SysTick_Delayus(100000);
		SysTick_Delayus(100000);
	}
}


/* When CPU frequency is 160MHz, the maximum delay is pow(2, 24) / 160 = 104857 us */
uint32_t SysTick_Delayus(uint32_t us)
{
	uint32_t ticks = us * CyclesPerUs;
	
	if((ticks - 1) > SysTick_LOAD_RELOAD_Msk)
		return 1;				// Reload value impossible
	
	SysTick->LOAD = ticks - 1;
	SysTick->VAL  = 0;			// clear the counter and COUNTFLAG flag
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_ENABLE_Msk;
	
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) __NOP();
	SysTick->CTRL = 0;
	
	return 0;
}
