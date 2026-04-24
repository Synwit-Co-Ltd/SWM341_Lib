#include "SWM341.h"


/* By default, the pull-up resistance of ISP and SWD pins is enabled, which increases the sleep power consumption.
 * To obtain the lowest sleep power consumption, disable the pull-up and pull-down resistance of all pins before sleep
 */


void TurnOffUSBPower(void);
void GPIO_CLR(void);

uint32_t SysTick_Delayus(uint32_t us)
{
	uint32_t ticks = us * CyclesPerUs;
	
	if((ticks - 1) > SysTick_LOAD_RELOAD_Msk)
		return 1;				//Reload value impossible
	
	SysTick->LOAD = ticks - 1;
	SysTick->VAL  = 0;			//清零计数器、清除 COUNTFLAG 标志
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_ENABLE_Msk;
	
	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)) __NOP();
	SysTick->CTRL = 0;
	
	return 0;
}

int main(void)
{	
	uint32_t  i;

	for(i = 0; i < SystemCoreClock; i++) __NOP();	//防止无法更新程序

	SystemInit();

	//打开外部12MHz，外部晶振为USB的工作时钟源，关闭USB  PHY 之前需要打开

	PORT_Init(PORTA, PIN3, PORTA_PIN3_XTAL_IN,  0);
	PORT_Init(PORTA, PIN4, PORTA_PIN4_XTAL_OUT, 0);
	SYS->XTALCR |= (1 << SYS_XTALCR_ON_Pos) | (15 << SYS_XTALCR_DRV_Pos) | (0 << SYS_XTALCR_DET_Pos);

	//等待外振起振
	SysTick_Delayus(6000);
	
	for(i = 0; i < SystemCoreClock; i++) __NOP();	//防止无法更新程序

	TurnOffUSBPower();

	//等待USB关闭，需要等下6.5us以上，设置10us
	SysTick_Delayus(10000);	

	GPIO_CLR();//设置所有GPIO为浮空状态,用户根据外设情况设置GPIO状态以防止GPIO漏电

	SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);			//开启32K低频振荡器

	//开启PN10引脚低电平唤	;除B12 B13 B14 B15不能用做唤醒脚，其它GPIO均可以唤醒SLEEP模式
	GPIO_Init(GPIOA, PIN10, 0, 1, 0, 0);				
	SYS->PAWKEN |= (1 << PIN10);	
					   
	while(1==1)
	{
		
		__disable_irq();       

		switchTo20MHz();	  //休眠前，切换到 20MHz

		SYS->PLLCR|=(1<< SYS_PLLCR_OFF_Pos);//关闭PLL
		SYS->XTALCR=0;     //关闭外部时钟

		SYS->PAWKSR = (1 << PIN10);					//清除唤醒标志
		SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);	//进入睡眠模式
		while((SYS->PAWKSR & (1 << PIN10)) == 0){};	//等待唤醒条件
		

		switchToPLL(1, 3, 60, PLL_OUT_DIV8, 0);		//唤醒后，切换到 PLL

		__enable_irq();    

		//切换到 PLL等待一下，看功耗变化
		for(i = 0; i < SystemCoreClock/20; i++) __NOP();
	}
}



void TurnOffUSBPower(void)
{

	SYS->USBCR |= 0x07;
	SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_USB_Pos);
	USBD->DEVCR = (USBD_SPEED_FS << USBD_DEVCR_SPEED_Pos);
	USBH->PORTSR |=(1<<2);
	SYS->USBPHYCR &= ~SYS_USBPHYCR_PLLEN_Msk;
	SYS->USBPHYCR &= ~SYS_USBPHYCR_OPMODE_Msk;
	SYS->USBPHYCR |= ( 1 << SYS_USBPHYCR_OPMODE_Pos);
}


void GPIO_CLR(void)
{
	//按需配置GPIO
	
	*((volatile uint32_t *)0x40000190) = 0; //注意：关闭SWD,在初始化前加上这一句就可以了，
//如要恢复 SWD 端口功能：
//    *((volatile uint32_t *)0x40000190) = 1;//切换 SWD 端口必须加上，
	PORT_Init(PORTA,  0,  0,  0);
	PORT_Init(PORTA,  1,  0,  0);
	PORT_Init(PORTA,  2,  0,  0);
//	PORT_Init(PORTA,  3,  15,  0);
//	PORT_Init(PORTA,  4,  15,  0);
	PORT_Init(PORTA,  5,  15,  0);
	PORT_Init(PORTA,  6,  0,  0);
	PORT_Init(PORTA,  7,  15,  0);
	PORT_Init(PORTA,  8,  0,  0);
	PORT_Init(PORTA,  9,  15,  0);
//	PORT_Init(PORTA,  10, 15 ,  0);
	PORT_Init(PORTA,  11, 15 ,  0);
	PORT_Init(PORTA,  12, 15 ,  0);
	PORT_Init(PORTA,  13, 15,  0);
	PORT_Init(PORTA,  14, 0 ,  0);
	PORT_Init(PORTA,  15, 15 ,  0);
	
	
	
	PORT_Init(PORTB,  0,  0,  0);
	PORT_Init(PORTB,  1,  0,  0);
	PORT_Init(PORTB,  2,  0,  0);
	PORT_Init(PORTB,  3,  0,  0);
	PORT_Init(PORTB,  4,  0,  0);
	PORT_Init(PORTB,  5,  0,  0);
	PORT_Init(PORTB,  6,  0,  0);
	PORT_Init(PORTB,  7,  0,  0);
	PORT_Init(PORTB,  8,  0,  0);
	PORT_Init(PORTB,  9,  0,  0);
	PORT_Init(PORTB,  10, 0 ,  0);
	PORT_Init(PORTB,  11, 0 ,  0);
	PORT_Init(PORTB,  12, 0 ,  0);
	PORT_Init(PORTB,  13, 0 ,  0);
	PORT_Init(PORTB,  14, 0 ,  0);
	PORT_Init(PORTB,  15, 0 ,  0);

	PORT_Init(PORTC, 0,  15,  0);
	PORT_Init(PORTC, 1,  15,  0);
	PORT_Init(PORTC, 2, 15 ,  0);
	PORT_Init(PORTC, 3,  15,  0);
	PORT_Init(PORTC, 4,  15,  0);
	PORT_Init(PORTC, 5,  15,  0);
	PORT_Init(PORTC,6 ,  15,  0);
	PORT_Init(PORTC, 7,  15,  0);
	PORT_Init(PORTC,8 ,  0,  0);
	PORT_Init(PORTC, 9,  15,  0);
	PORT_Init(PORTC, 10,  15,  0);
	PORT_Init(PORTC, 11,  15,  0);
	PORT_Init(PORTC, 12,  15,  0);
	PORT_Init(PORTC, 13,  15,  0);
	PORT_Init(PORTC, 14,  15,  0);
	PORT_Init(PORTC, 15,  15,  0);
	
	

	PORT_Init(PORTD,  0,  15,  0);
	PORT_Init(PORTD,  1,  15,  0);
	PORT_Init(PORTD,  2,  15,  0);
	PORT_Init(PORTD,  3,  15,  0);
	PORT_Init(PORTD,  4,  15,  0);
	PORT_Init(PORTD, 5 ,  15,  0);
	PORT_Init(PORTD,  6,  15,  0);
	PORT_Init(PORTD,  7,  15,  0);
	PORT_Init(PORTD, 8 ,  15,  0);
	PORT_Init(PORTD, 9 ,  0,  0);
	PORT_Init(PORTD,  10,  0,  0);
	PORT_Init(PORTD,  11,  0,  0);
	PORT_Init(PORTD,  12,  0,  0);
	PORT_Init(PORTD,  13,  0,  0);
	PORT_Init(PORTD,  14,  0,  0);
	PORT_Init(PORTD,  15,  0,  0);

	PORT_Init(PORTE,  1,  0,  0);
	PORT_Init(PORTE,  2,  0,  0);
	PORT_Init(PORTE,  3,  0,  0);
	PORT_Init(PORTE,  4,  0,  0);
	PORT_Init(PORTE,  5,  0,  0);
	PORT_Init(PORTE,  6,  0,  0);
	PORT_Init(PORTE,  7,  0,  0);
	PORT_Init(PORTE,  8,  0,  0);
	PORT_Init(PORTE,  9,  0,  0);
	PORT_Init(PORTE,  10,  0,  0);
	PORT_Init(PORTE,  11,  0,  0);
	PORT_Init(PORTE,  12,  0,  0);
	PORT_Init(PORTE,  13,  0,  0);
	PORT_Init(PORTE,  14,  0,  0);
	PORT_Init(PORTE,  15,  0,  0);
	
	PORT_Init(PORTM,   1,  0,  0);
	PORT_Init(PORTM,   2,  0,  0);
	PORT_Init(PORTM,   3,  0,  0);
	PORT_Init(PORTM,   4,  0,  0);
	PORT_Init(PORTM,   6,  0,  0);
	PORT_Init(PORTM,   7,  0,  0);
	PORT_Init(PORTM,   8,  0,  0);
	PORT_Init(PORTM,   9,  0,  0);
	PORT_Init(PORTM,   10,  0,  0);
	PORT_Init(PORTM,   11,  0,  0);
	PORT_Init(PORTM,   12,  0,  0);
	PORT_Init(PORTM,   13,  0,  0);
	PORT_Init(PORTM,   14,  0,  0);
	PORT_Init(PORTM,   15,  0,  0);
	
	PORT_Init(PORTN,  0 ,  0,  0);
	PORT_Init(PORTN,   1,  0,  0);
	PORT_Init(PORTN,  2 ,  0,  0);
	PORT_Init(PORTN,  3 ,  0,  0);
	PORT_Init(PORTN,   4,  0,  0);
	PORT_Init(PORTN,   5,  0,  0);
	PORT_Init(PORTN,   6,  0,  0);
	PORT_Init(PORTN,  7 ,  0,  0);
	PORT_Init(PORTN,8,  0,  0);
	PORT_Init(PORTN,  9 ,  0,  0);
	PORT_Init(PORTN,  10 ,  0,  0);
	PORT_Init(PORTN,   11,  0,  0);
	PORT_Init(PORTN,  12 ,  0,  0);
	PORT_Init(PORTN,   13,  0,  0);
	PORT_Init(PORTN,   14,  0,  0);
	PORT_Init(PORTN,  15 ,  0,  0);

}
