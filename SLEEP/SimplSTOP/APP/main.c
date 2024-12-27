#include "SWM341.h"


/* note:
 * 1) After entering STOP mode, you can only wake-up through the wake-up pin, and cannot reset the chip by pressing the reset button.
 * 2) The program restarts from the beginning after wake-up form the STOP mode.
 * 3) The wake-up pin is PD6, its falling edge wake-up chip, so need to keep high level during STOP mode.
 */


void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);			// Turn on 32KHz LRC oscillator
	
	printf("BACKUP[0]: %08X\r\n", SYS->BACKUP[0]);	// Note: do not have data retention function
	SYS->BACKUP[0] += 1;
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);				// output, connect a LED
	
	PORT_Init(PORTD, PIN6, PORTD_PIN6_WAKEUP, 1);
	PORTD->PULLU |= (1 << PIN6);
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN9);					// turn on the LED
		for(i = 0; i < SystemCoreClock/4; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN9);					// turn off the LED
		
		SYS->SLEEP |= (1 << SYS_SLEEP_STOP_Pos);	// enter STOP mode
	
		for(i = 0; i < 5000; i++);
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);
	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
