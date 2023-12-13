#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN10, 0, 1, 0, 0);			//输入，上拉使能，接KEY
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);			//输出， 接LED
	
	while(1==1)
	{
		if(GPIO_GetBit(GPIOA, PIN10) == 0)		//按键被按下
		{
			GPIO_SetBit(GPIOA, PIN9);
		}
		else
		{
			GPIO_ClrBit(GPIOA, PIN9);
		}
	}
}

