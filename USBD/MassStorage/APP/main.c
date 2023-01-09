#include "SWM341.h"
#include "FlashDisk.h"
#include "MassStorage.h"


/*  通过修改 FlashDisk.h 中的 MSC_MEDIUM 的值，选择读写片内Flash、片外SPI Flash、SD卡 

	注意：将 SWM341 USB 连接电脑时，Windows 会读取 FAT 表以识别 FAT16/FAT32 文件系统，
	      SD 卡的 FAT 表很大，而 USB FS 的速度又较低，因此连接电脑时需要较长时间才能成功识别 U 盘。
		  因此本例程读写 SD 卡，建议只用于小容量的 SDNAND，不要用于普通 SD 卡
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
