#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//输出，接LED
	
	SYS->BODCR = (1 << SYS_BODCR_IE_Pos) | (3 << SYS_BODCR_INTLVL_Pos);		// 2.5v 触发BOD中断
	SYS->BODSR = (1 << SYS_BODSR_IF_Pos);	//清除中断标志
	NVIC_EnableIRQ(BOD_IRQn);
	
	while(1==1)
	{
		GPIO_ClrBit(GPIOA, PIN5);
	}
}

void BOD_Handler(void)
{
	SYS->BODSR = (1 << SYS_BODSR_IF_Pos);
	
	GPIO_SetBit(GPIOA, PIN5);
}
