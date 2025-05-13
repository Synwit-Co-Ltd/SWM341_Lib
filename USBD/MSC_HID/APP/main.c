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
