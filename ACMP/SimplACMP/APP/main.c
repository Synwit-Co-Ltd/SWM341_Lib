#include "SWM341.h"


void SerialInit(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTA, PIN2,  PORTA_PIN2_ACMP0_INP,  0);
// 	PORT_Init(PORTB, PIN12, PORTB_PIN12_ACMP0_INN, 0);	// SWDCK pin
	PORT_Init(PORTA, PIN1,  PORTA_PIN1_ACMP1_INP,  0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_ACMP1_INN,  0);
	PORT_Init(PORTA, PIN0,  PORTA_PIN0_ACMP2_INP,  0);
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_ACMP2_INN,  0);
	
	/* hysteresis enable, negative input connects to internal VREF, interrupt enable or disable */
	SYS->ACMPCR |= (1 << SYS_ACMPCR_CMP0ON_Pos) | (1 << SYS_ACMPCR_CMP0HYS_Pos) | (1 << SYS_ACMPCR_0NVREF_Pos) | (1 << SYS_ACMPCR_CMP0IE_Pos);
	SYS->ACMPCR |= (1 << SYS_ACMPCR_CMP1ON_Pos) | (1 << SYS_ACMPCR_CMP1HYS_Pos) | (1 << SYS_ACMPCR_1NVREF_Pos) | (0 << SYS_ACMPCR_CMP1IE_Pos);
	SYS->ACMPCR |= (1 << SYS_ACMPCR_CMP2ON_Pos) | (1 << SYS_ACMPCR_CMP2HYS_Pos) | (1 << SYS_ACMPCR_2NVREF_Pos) | (0 << SYS_ACMPCR_CMP2IE_Pos);
	
	SYS->ACMPCR2 &= ~SYS_ACMPCR2_VREF_Msk;
	SYS->ACMPCR2 |= (40 << SYS_ACMPCR2_VREF_Pos);
	
	NVIC_EnableIRQ(ACMP_IRQn);
	
	while(1==1)
	{
		printf("ACMP0 OUT = %d\r\n", (SYS->ACMPSR & SYS_ACMPSR_CMP0OUT_Msk) ? 1 : 0);
//		printf("ACMP1 OUT = %d\r\n", (SYS->ACMPSR & SYS_ACMPSR_CMP1OUT_Msk) ? 1 : 0);
//		printf("ACMP2 OUT = %d\r\n", (SYS->ACMPSR & SYS_ACMPSR_CMP2OUT_Msk) ? 1 : 0);
		
		for(int i = 0; i < SystemCoreClock/100; i++) __NOP();
	}
}

void ACMP_Handler(void)
{
	if(SYS->ACMPSR & SYS_ACMPSR_CMP0IF_Msk)
	{
		SYS->ACMPSR = (1 << SYS_ACMPSR_CMP0IF_Pos);
		
		printf("ACMP0 Interrupt Happened\r\n");
	}
	else if(SYS->ACMPSR & SYS_ACMPSR_CMP1IF_Msk)
	{
		SYS->ACMPSR = (1 << SYS_ACMPSR_CMP1IF_Pos);
		
		printf("ACMP1 Interrupt Happened\r\n");
	}
	else if(SYS->ACMPSR & SYS_ACMPSR_CMP2IF_Msk)
	{
		SYS->ACMPSR = (1 << SYS_ACMPSR_CMP2IF_Pos);
		
		printf("ACMP2 Interrupt Happened\r\n");
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
