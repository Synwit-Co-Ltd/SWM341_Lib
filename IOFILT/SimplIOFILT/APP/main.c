#include "SWM341.h"


/*	IOFILT function test method
 *	Connect PA5 to PB4, and use an oscilloscope to observe the PA5 and PB5 pins.
 *	It can be observed that the wider pulse on PA5 is copied to the PB5, but the narrower pulse is not.
 */


void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
   	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);
	
	GPIO_Init(GPIOB, PIN4, 0, 1, 0, 0);
	EXTI_Init(GPIOB, PIN4, EXTI_BOTH_EDGE);
	NVIC_EnableIRQ(GPIOB_IRQn);
	EXTI_Open(GPIOB, PIN4);
	
	GPIO_Init(GPIOB, PIN5, 1, 0, 0, 0);
	
	IOFILT_Init(0, IOFILT0_PB4, IOFILT_WIDTH_32);
   	
	while(1==1)
	{
		int i;
		
		GPIO_InvBit(GPIOA, PIN5);
		for(i = 0; i < 2; i++) __NOP();
		
		GPIO_InvBit(GPIOA, PIN5);
		for(i = 0; i < 20; i++) __NOP();
		
		GPIO_InvBit(GPIOA, PIN5);
		for(i = 0; i < 200; i++) __NOP();
		
		GPIO_InvBit(GPIOA, PIN5);
		for(i = 0; i < 2000; i++) __NOP();
	}
}


void GPIOB_Handler(void)
{
	EXTI_Clear(GPIOB, PIN4);
	
	GPIOB->DATAPIN5 = GPIOB->DATAPIN4;
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
