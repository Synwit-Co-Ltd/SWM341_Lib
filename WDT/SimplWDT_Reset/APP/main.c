#include "SWM341.h"


void SerialInit(void);

int main(void)
{
	uint32_t i;

	SystemInit();
	
	SerialInit();
	
	if(SYS->RSTSR & SYS_RSTSR_WDT_Msk)
	{
		SYS->RSTSR = (1 << SYS_RSTSR_WDT_Pos);
		
		printf("WDT Reset Happened\r\n");
	}
	
	WDT_Init(WDT, 0, 1024);
	WDT_Start(WDT);
	
	while(1==1)
	{
		WDT_Feed(WDT);	// Comment out this code, the chip will keep resetting print "WDT Reset Happened"
		
		/* Note:
		 *	1) At least 5 WDT clock cycles between feeding the dog, i.e. 1000000/32768 * 5 = 150us; Considering that the WDT clock error is large, the recommended interval is not less than 300us
		 *	2) Do not execute WDT_Feed() when WDT is stopped
		 *	3) After WDT_Feed() is executed, WDT_Stop() cannot be executed immediately. WDT_Stop() must be executed after 5 WDT clock cycles.
		 */

		for(i = 0; i < CyclesPerUs * 300 / 4; i++) __NOP();
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
