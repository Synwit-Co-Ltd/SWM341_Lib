#include "SWM341.h"

void SerialInit(void);
void TestSignal(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	TestSignal();	// Generate test signals for Input Capture functional
	
	PORT_Init(PORTB, PIN15, PORTB_PIN15_TIMR0_IN, 1);
	PORTB->PULLD |= (1 << PIN15);
	
	TIMR_Init(TIMR0, TIMR_MODE_IC, CyclesPerUs, 0xFFFFFF, 1);
	
	TIMR_IC_Init(TIMR0, 1, 1);
	
	TIMR_Start(TIMR0);
	
	while(1==1)
	{
	}
}


void TIMR0_Handler(void)
{
	if(TIMR_INTStat(TIMR0, TIMR_IT_IC_HIGH))
	{
		TIMR_INTClr(TIMR0, TIMR_IT_IC_HIGH);
		
		printf("H: %d\r\n", TIMR_IC_GetCaptureH(TIMR0));
	}
	else if(TIMR_INTStat(TIMR0, TIMR_IT_IC_LOW))
	{
		TIMR_INTClr(TIMR0, TIMR_IT_IC_LOW);
		
		printf("L: %d\r\n", TIMR_IC_GetCaptureL(TIMR0));
	}
	else if(TIMR_INTStat(TIMR0, TIMR_IT_TO))
	{
		TIMR_INTClr(TIMR0, TIMR_IT_TO);
		
		printf("Overflow: The pulse is too wide to capture\n");
	}
}


void TestSignal(void)
{
	PORT_Init(PORTB, PIN13, PORTB_PIN13_TIMR2_OUT, 0);
	
	TIMR_Init(TIMR2, TIMR_MODE_OC, CyclesPerUs, 100000, 0);		// period is 100ms
	
	TIMR_OC_Init(TIMR2, 75000, 0, 1);							// duty ratio is 75%
	
	TIMR_Start(TIMR2);
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
