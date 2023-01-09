#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//输出，接LED
	
	SysTick_Config(0x1000000);				//注意：SysTick 是 24 位的，超过最大计数值时 SysTick_Config 不配置，直接返回
	
	while(1==1)
	{
	}
}

void SysTick_Handler(void)
{	
	GPIO_InvBit(GPIOA, PIN5);	//反转LED亮灭状态
}

