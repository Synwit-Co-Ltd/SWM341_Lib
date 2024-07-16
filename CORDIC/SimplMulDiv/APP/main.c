#include "SWM341.h"


/* CORDIC fractional multiplication and division test */


void SerialInit(void);

int main(void)
{
	uint16_t mul1, mul2;
	uint16_t dividend, divisor;
	
	SystemInit();
	
	SerialInit();
   	
	CORDIC_Init(CORDIC);
	
	mul1 = 1.25 * 16384;
	mul2 = 1.25 * 16384;
	CORDIC_Mul(mul1, mul2);
	while(CORDIC_Mul_IsDone() == 0) __NOP();
	printf("1.25 * 1.25 = %f\r\n", CORDIC_Mul_Result()/16384.0);
	
	dividend = 1.25 * 16384;
	divisor  = 0.6  * 16384;
	CORDIC_Div(dividend, divisor);
	while(CORDIC_Div_IsDone() == 0) __NOP();
	printf("1.25 / 0.6 = %f\r\n", CORDIC_Div_Result()/16384.0);
	
	while(1==1)
	{
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
