#include "SWM341.h"
#include "FlashDisk.h"
#include "MassStorage.h"


/*	By changing the value of MSC_MEDIUM in flashdiskd.h, you can read and write the on-chip Flash, off-chip SPI Flash, and SD card
 *	Note: When you connect the SWM341 USB to your computer, Windows reads the FAT table to identify the FAT16/FAT32 file system.
 *	The FAT table of the SD card is large, and the speed of the USB FS is low, so it takes a long time to successfully identify U-Disk when connecting to the computer.
 *	Therefore, you are advised to use this routine to read and write SD cards only for small-capacity SDNAND and not for common SD cards
 */


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
