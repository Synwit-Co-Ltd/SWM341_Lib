#include "SWM341.h"


/* 注意：
 *	芯片的 ISP、SWD 引脚默认开启了上拉电阻，会增加休眠功耗，若想获得最低休眠功耗，休眠前请关闭所有引脚的上拉和下拉电阻
 */
 
void TurnOffUSBPower(void);

int main(void)
{	
	uint32_t i;
	
	for(i = 0; i < SystemCoreClock; i++) __NOP();	//防止无法更新程序
	
	SystemInit();
	
	TurnOffUSBPower();
	
	SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);			//开启32K低频振荡器
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//输出， 接LED
	
	GPIO_Init(GPIOA, PIN0, 0, 1, 0, 0);				//输入，上拉使能，接KEY
	SYS->PAWKEN |= (1 << PIN0);						//开启PA0引脚低电平唤醒
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//点亮LED
		for(i = 0; i < SystemCoreClock/4; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN5);					//熄灭LED
		
		__disable_irq();
		switchTo20MHz();	//休眠前，切换到 20MHz
		
		SYS->PAWKSR = (1 << PIN0);					//清除唤醒标志
		SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);	//进入睡眠模式
		while((SYS->PAWKSR & (1 << PIN0)) == 0);	//等待唤醒条件
		
		switchToPLL(0);		//唤醒后，切换到 PLL
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

