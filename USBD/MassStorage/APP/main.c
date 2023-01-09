#include "SWM341.h"
#include "FlashDisk.h"
#include "MassStorage.h"


/*  ͨ���޸� FlashDisk.h �е� MSC_MEDIUM ��ֵ��ѡ���дƬ��Flash��Ƭ��SPI Flash��SD�� 

	ע�⣺�� SWM341 USB ���ӵ���ʱ��Windows ���ȡ FAT ����ʶ�� FAT16/FAT32 �ļ�ϵͳ��
	      SD ���� FAT ��ܴ󣬶� USB FS ���ٶ��ֽϵͣ�������ӵ���ʱ��Ҫ�ϳ�ʱ����ܳɹ�ʶ�� U �̡�
		  ��˱����̶�д SD ��������ֻ����С������ SDNAND����Ҫ������ͨ SD ��
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
