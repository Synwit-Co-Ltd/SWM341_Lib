#include "SWM341.h"


/* ע�⣺
 *	оƬ�� ISP��SWD ����Ĭ�Ͽ������������裬���������߹��ģ�������������߹��ģ�����ǰ��ر��������ŵ���������������
 */
 
void TurnOffUSBPower(void);

int main(void)
{	
	uint32_t i;
	
	for(i = 0; i < SystemCoreClock; i++) __NOP();	//��ֹ�޷����³���
	
	SystemInit();
	
	TurnOffUSBPower();
	
	SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);			//����32K��Ƶ����
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//����� ��LED
	
	GPIO_Init(GPIOA, PIN0, 0, 1, 0, 0);				//���룬����ʹ�ܣ���KEY
	SYS->PAWKEN |= (1 << PIN0);						//����PA0���ŵ͵�ƽ����
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//����LED
		for(i = 0; i < SystemCoreClock/4; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN5);					//Ϩ��LED
		
		__disable_irq();
		switchTo20MHz();	//����ǰ���л��� 20MHz
		
		SYS->PAWKSR = (1 << PIN0);					//������ѱ�־
		SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);	//����˯��ģʽ
		while((SYS->PAWKSR & (1 << PIN0)) == 0);	//�ȴ���������
		
		switchToPLL(0);		//���Ѻ��л��� PLL
		__enable_irq();
	}
}


void TurnOffUSBPower(void)
{
	SYS->USBCR |= 0x07;
	
	SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_USB_Pos);
	USBD->DEVCR = (USBD_SPEED_FS << USBD_DEVCR_SPEED_Pos);
	
	SYS->USBPHYCR &= ~SYS_USBPHYCR_PLLEN_Msk;
	SYS->USBPHYCR &= ~SYS_USBPHYCR_OPMODE_Msk;
	SYS->USBPHYCR |= ( 2 << SYS_USBPHYCR_OPMODE_Pos);
}

