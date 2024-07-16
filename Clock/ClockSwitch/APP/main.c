#include "SWM341.h"


/* system clock switching demo program */


void SerialInit(void);
void SystemInit_80MHz(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
   	
	printf("SystemCoreClock = %d\r\n", SystemCoreClock);
	
	while(1==1)
	{
		SystemInit_80MHz();
		
		UART_SetBaudrate(UART0, 57600);		// after the system frequency is changed, the UART baudrate needs to be reconfigured
		
		for(int i = 0; i < 4; i++)
		{
			printf("SystemCoreClock = %d\r\n", SystemCoreClock);
			
			for(int j = 0; j < SystemCoreClock / 8; j++) __NOP();
		}
		
		SystemInit();
		
		UART_SetBaudrate(UART0, 57600);
		
		for(int i = 0; i < 4; i++)
		{
			printf("SystemCoreClock = %d\r\n", SystemCoreClock);
			
			for(int j = 0; j < SystemCoreClock / 8; j++) __NOP();
		}
	}
}


void SystemInit_80MHz(void)
{
	Flash_Param_at_xMHz(150);
	
	switchToPLL(0, 5, 40, PLL_OUT_DIV8, 0);
	
	SystemCoreClockUpdate();
	
	Flash_Param_at_xMHz(CyclesPerUs);
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
