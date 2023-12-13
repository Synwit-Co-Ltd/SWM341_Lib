#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);			//输出，接LED
	
	GPIO_Init(GPIOA, PIN2, 0, 1, 0, 0);			//输入，上拉使能，接KEY
	
	EXTI_Init(GPIOA, PIN2, EXTI_FALL_EDGE);		//下降沿触发中断
	
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
		EXTI_Clear(GPIOA, PIN2);	// 当GPIOA中断使能时，GPIOA2下降沿同时触发NMI中断和GPIOA中断
	}
}
