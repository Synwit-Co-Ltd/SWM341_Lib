#include "SWM341.h"


/* By default, the pull-up resistance of ISP and SWD pins is enabled, which increases the sleep power consumption.
 * To obtain the lowest sleep power consumption, disable the pull-up and pull-down resistance of all pins before sleep
 */


void TurnOffUSBPower(void);

int main(void)
{
	for(int i = 0; i < SystemCoreClock; i++) __NOP();	// Prevents unable to download programs
	
	SystemInit();
	
	TurnOffUSBPower();
	
	SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);			// Turn on 32KHz LRC oscillator
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);				// output, connect a LED
	
	GPIO_Init(GPIOA, PIN10, 0, 1, 0, 0);			// intput, pull-up enable, connect a key
	SYS->PAWKEN |= (1 << PIN10);					// enable PA10 pin low wake up
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN9);					// turn on the LED
		for(int i = 0; i < SystemCoreClock/4; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN9);					// turn off the LED
		
		__disable_irq();
		switchTo20MHz();							// Before sleep, switch to 20MHz
		
		SYS->PAWKSR = (1 << PIN10);					// clear wakeup flag
		SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);	// enter sleep mode
		while((SYS->PAWKSR & (1 << PIN10)) == 0);	// wait wake-up
		
		switchToPLL(1, 3, 60, PLL_OUT_DIV8, 0);		// After waking up, switch to PLL
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

