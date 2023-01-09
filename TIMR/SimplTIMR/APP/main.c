#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			//输出，接LED
	
	TIMR_Init(TIMR4, TIMR_MODE_TIMER, CyclesPerUs, 500000, 1);	//每0.5秒钟触发一次中断
	
	TIMR_Start(TIMR4);
	
	while(1==1)
	{
		
	}
}

void TIMR4_Handler(void)
{
	TIMR_INTClr(TIMR4);
	
	GPIO_InvBit(GPIOA, PIN5);	//反转LED亮灭状态
}

