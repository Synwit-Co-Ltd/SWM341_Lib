#include "SWM341.h"
#include "FlashDisk.h"
#include "MassStorage.h"


int main(void)
{
	uint32_t n=0;
	
	SystemInit();
	
	MSC_Init();
	
	USBD_Open();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);
   	
	while(1==1)
	{
		MSC_ProcessOUT();
		
		if(++n % 1000000 == 0)
		{
			FlashDiskFlush();

			GPIO_InvBit(GPIOA, PIN5);
		}
	}
}


void int_to_unicode(uint32_t value , uint8_t *pbuf , uint8_t len)
{
	for(int i = 0 ; i < len ; i++)
	{
		if((value >> 28) < 0xA)
		{
			pbuf[2 * i] = (value >> 28) + '0';
		}
		else
		{
			pbuf[2 * i] = (value >> 28) + 'A' - 10;
		}

		pbuf[2 * i + 1] = 0;

		value = value << 4;
	}
}
