#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);			//�������LED
	
	GPIO_Init(GPIOA, PIN2, 0, 1, 0, 0);			//���룬����ʹ�ܣ���KEY
	
	EXTI_Init(GPIOA, PIN2, EXTI_FALL_EDGE);		//�½��ش����ж�
	
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
		EXTI_Clear(GPIOA, PIN2);	// ��GPIOA�ж�ʹ��ʱ��GPIOA2�½���ͬʱ����NMI�жϺ�GPIOA�ж�
	}
}
