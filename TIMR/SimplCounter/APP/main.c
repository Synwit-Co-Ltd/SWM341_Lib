#include "SWM341.h"


int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN0, 1, 0, 0, 0);					//产生方波，方便测试
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);
	
	PORT_Init(PORTB, PIN9, PORTB_PIN9_TIMR2_IN, 1);		//连接 PA0 引脚，计数 PA0 引脚上的上升沿
	
	TIMR_Init(TIMR2, TIMR_MODE_COUNTER, 1, 5, 1);		//每计5个上升沿进入中断
	TIMR_Start(TIMR2);
	
	while(1==1)
	{
		GPIO_InvBit(GPIOA, PIN0);
		for(i = 0; i < SystemCoreClock/64; i++) __NOP();
	}
}

void TIMR2_Handler(void)
{
	TIMR_INTClr(TIMR2);
	
	GPIO_InvBit(GPIOA, PIN5);
}
