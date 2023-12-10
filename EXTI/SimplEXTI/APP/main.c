#include "SWM341.h"

#define FAST_IRQ	0

int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);			//�������LED
	
	GPIO_Init(GPIOA, PIN10, 0, 1, 0, 0);			//���룬����ʹ�ܣ���KEY
	
	EXTI_Init(GPIOA, PIN10, EXTI_FALL_EDGE);		//�½��ش����ж�

#if FAST_IRQ
	NVIC_EnableIRQ(GPIOA10_IRQn);
#else
	NVIC_EnableIRQ(GPIOA_IRQn);
#endif
	
	EXTI_Open(GPIOA, PIN10);

	while(1==1)
	{
	}
}

#if FAST_IRQ
void GPIOA10_Handler(void)
{
	EXTI_Clear(GPIOA, PIN10);
	
	GPIO_InvBit(GPIOA, PIN9);
}
#else
void GPIOA_Handler(void)
{
	if(EXTI_State(GPIOA, PIN10))
	{
		EXTI_Clear(GPIOA, PIN10);
		
		GPIO_InvBit(GPIOA, PIN9);
	}
}
#endif
