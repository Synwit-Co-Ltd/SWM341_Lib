#include "SWM341.h"


int main(void)
{	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN10, 0, 1, 0, 0);			//���룬����ʹ�ܣ���KEY
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);			//����� ��LED
	
	while(1==1)
	{
		if(GPIO_GetBit(GPIOA, PIN10) == 0)		//����������
		{
			GPIO_SetBit(GPIOA, PIN9);
		}
		else
		{
			GPIO_ClrBit(GPIOA, PIN9);
		}
	}
}

