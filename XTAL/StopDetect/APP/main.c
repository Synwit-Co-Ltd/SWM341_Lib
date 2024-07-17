#include "SWM341.h"


/* When the external crystal oscillator stops, the chip automatically switches to the on-chip RC clock and triggers an interrupt */


void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	for(int i = 0; i < SystemCoreClock/4; i++) __NOP();	// Wait for the crystal to stabilize, to prevent misidentification of crystal vibration stop during power-on
   	
	SYS->XTALSR = SYS_XTALSR_STOP_Msk;					// clear STOP flag
	NVIC_ClearPendingIRQ(XTALSTOP_IRQn);
	NVIC_EnableIRQ(XTALSTOP_IRQn);
   	
	while(1==1)
	{
		printf("SystemCoreClock = %d\r\n", SystemCoreClock);
		
		for(int i = 0; i < SystemCoreClock/4; i++) __NOP();
	}
}


void XTALSTOP_Handler(void)
{
	/*	If switchTo20MHz() is not executed, the system clock will automatically switch back to the external crystal when the crystal oscillator recovers.
	 *	If the external crystal oscillator is unstable, the system clock will switch back and forth between the internal clock and the external clock.
	 */
	switchTo20MHz();
	SystemCoreClockUpdate();
	
	SYS->XTALCR = 0;							// External crystal oscillator is not working properly, shut down
	NVIC_DisableIRQ(XTALSTOP_IRQn);
	
	UART_SetBaudrate(UART0, 57600);				// System frequency changes, the UART baudrate needs to be reset
	printf("XTAL Stop Detected!\r\n");
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
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
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
