#include "SWM341.h"


void SerialInit(void);
void TestSignal(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	TestSignal();	// generate test waveform, use HALL to measure it's pulse width
	
#if 1
	PORT_Init(PORTA, PIN0, PORTA_PIN0_HALL0_IN0, 1);
	PORT_Init(PORTA, PIN1, PORTA_PIN1_HALL0_IN1, 1);
	PORT_Init(PORTA, PIN2, PORTA_PIN2_HALL0_IN2, 1);
	PORTA->PULLU |= ((1 << PIN0) | (1 << PIN1) | (1 << PIN2));
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 0xFFFFFF, 0);		// The maximum measurable pulse width is 0xFFFFFF uS, i.e. 16.7s
	
	TIMRG->HALLEN |= TIMRG_HALLEN_HALL0_Msk;
	TIMRG->HALLIF =  TIMRG_HALLIF_H0IN0_Msk | TIMRG_HALLIF_H0IN1_Msk | TIMRG_HALLIF_H0IN2_Msk;
	TIMRG->HALLIE |= TIMRG_HALLIE_HALL0_Msk;
	NVIC_EnableIRQ(HALL0_IRQn);
	
	TIMR_Start(TIMR0);
#else
	PORT_Init(PORTA, PIN12, PORTA_PIN12_HALL3_IN0, 1);
	PORT_Init(PORTA, PIN13, PORTA_PIN13_HALL3_IN1, 1);
	PORT_Init(PORTA, PIN14, PORTA_PIN14_HALL3_IN2, 1);
	PORTA->PULLU |= ((1 << PIN12) | (1 << PIN13) | (1 << PIN14));
	
	TIMR_Init(TIMR3, TIMR_MODE_TIMER, CyclesPerUs, 0xFFFFFF, 0);
	
	TIMRG->HALLEN |= TIMRG_HALLEN_HALL3_Msk;
	TIMRG->HALLIF =  TIMRG_HALLIF_H3IN0_Msk | TIMRG_HALLIF_H3IN1_Msk | TIMRG_HALLIF_H3IN2_Msk;
	TIMRG->HALLIE |= TIMRG_HALLIE_HALL3_Msk;
	NVIC_EnableIRQ(HALL3_IRQn);
	
	TIMR_Start(TIMR3);
#endif

	while(1==1)
	{
	}
}


void HALL0_Handler(void)
{
	TIMRG->HALLIF =  TIMRG_HALLIF_H0IN0_Msk | TIMRG_HALLIF_H0IN1_Msk | TIMRG_HALLIF_H0IN2_Msk;
	
	printf("%dus\r\n", TIMRG->HALL0V);
}


void HALL3_Handler(void)
{
	TIMRG->HALLIF =  TIMRG_HALLIF_H3IN0_Msk | TIMRG_HALLIF_H3IN1_Msk | TIMRG_HALLIF_H3IN2_Msk;
	
	printf("%dus\r\n", TIMRG->HALL3V);
}


void TestSignal(void)
{
	GPIO_Init(GPIOA, PIN9,  1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN10, 1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN11, 1, 0, 0, 0);
	
	TIMR_Init(TIMR1, TIMR_MODE_TIMER, CyclesPerUs, 40000, 1);
	TIMR_Start(TIMR1);
}


void TIMR1_Handler(void)
{
	static uint32_t step = 0;
	
	TIMR1->IF = (1 << TIMR_IF_TO_Pos);
	
	switch(step++)
	{
	case 0: GPIO_SetBit(GPIOA, PIN9);  break;
	case 1: GPIO_SetBit(GPIOA, PIN10); break;
	case 2: GPIO_SetBit(GPIOA, PIN11); break;
	case 3: GPIO_ClrBit(GPIOA, PIN9);  break;
	case 4: GPIO_ClrBit(GPIOA, PIN10); break;
	case 5: GPIO_ClrBit(GPIOA, PIN11); step = 0; break;
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
