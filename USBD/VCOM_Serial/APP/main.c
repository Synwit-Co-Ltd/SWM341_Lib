#include "SWM341.h"
#include "vcom_serial.h"


int main(void)
{
	uint32_t i=0;
	
	SystemInit();
	
	VCOM_Init();
	
	USBD_Open();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);
   	
	while(1==1)
	{
		VCOM_TransferData();

        if(++i%1000000 == 0)
            GPIO_InvBit(GPIOA, PIN5);
	}
}
