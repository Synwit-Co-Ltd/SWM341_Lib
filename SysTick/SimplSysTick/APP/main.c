#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//�������LED
	
	SysTick_Config(0x1000000);				//ע�⣺SysTick �� 24 λ�ģ�����������ֵʱ SysTick_Config �����ã�ֱ�ӷ���
	
	while(1==1)
	{
	}
}

void SysTick_Handler(void)
{	
	GPIO_InvBit(GPIOA, PIN5);	//��תLED����״̬
}

