#include "SWM341.h"

volatile int32_t Speed;

void SerialInit(void);
void testSignal(void);

int main(void)
{	
	uint32_t i;
	QEI_InitStructure  QEI_initStruct;
	
 	SystemInit();
	
	SerialInit();
	
	testSignal();
	
	PORT_Init(PORTD, PIN4, PORTD_PIN4_QEI_A,   1);
	PORT_Init(PORTD, PIN3, PORTD_PIN3_QEI_B,   1);
	PORT_Init(PORTD, PIN2, PORTD_PIN2_QEI_Z,   1);
	PORT_Init(PORTD, PIN7, PORTD_PIN7_QEI_DIR, 0);
	
	QEI_initStruct.mode   = QEI_MODE_X4;
	QEI_initStruct.maxcnt = 10000;
	QEI_initStruct.swapAB = 0;
	QEI_initStruct.intINDEXEn = 0;
	QEI_initStruct.intMATCHEn = 0;
	QEI_initStruct.intCNTOVEn = 0;
	QEI_initStruct.intERROREn = 0;
	QEI_Init(QEI, &QEI_initStruct);
	
	QEI_Start(QEI);
	
	TIMR_Init(BTIMR1, TIMR_MODE_TIMER, CyclesPerUs, 50000, 1);	// The measured signal period is: 1/10KHz * 1000 = 0.1s, so the measurement interval should be less than 0.05s
	TIMR_Start(BTIMR1);
	
	while(1==1)
	{
		printf("speed = %+d\r\n", Speed);
		
		for(i = 0; i < CyclesPerUs*100; i++) __NOP();
	}
}


void BTIMR1_Handler(void)
{
	static int16_t Cnt[2] = {0};

	TIMR_INTClr(BTIMR1, TIMR_IT_TO);
	
	Cnt[1] = Cnt[0];
	Cnt[0] = (int16_t)QEI->POSCNT;
	
	Speed = Cnt[1] - Cnt[0];
}


void testSignal(void)
{
	GPIO_Init(GPIOA, PIN0, 1, 0, 0, 0);		// connect QEI_A
	GPIO_Init(GPIOA, PIN1, 1, 0, 0, 0);		// connect QEI_B
	GPIO_Init(GPIOA, PIN2, 1, 0, 0, 0);		// connect QEI_Z
	GPIO_ClrBit(GPIOA, PIN0);
	GPIO_ClrBit(GPIOA, PIN1);
	GPIO_ClrBit(GPIOA, PIN2);
	
	TIMR_Init(BTIMR0, TIMR_MODE_TIMER, CyclesPerUs, 100, 1);	// 10KHz
	TIMR_Start(BTIMR0);
}

void BTIMR0_Handler(void)
{	
	static uint32_t i = 0;
	
	TIMR_INTClr(BTIMR0, TIMR_IT_TO);
	
	if(i%20000 < 10000)	// forward rotation
	{
		switch(i%4)
		{
		case 0:
			GPIOA->DATAPIN0 = 0;
			GPIOA->DATAPIN1 = 1;
			break;
		
		case 1:
			GPIOA->DATAPIN0 = 0;
			GPIOA->DATAPIN1 = 0;
			break;
		
		case 2:
			GPIOA->DATAPIN0 = 1;
			GPIOA->DATAPIN1 = 0;
			break;
		
		case 3:
			GPIOA->DATAPIN0 = 1;
			GPIOA->DATAPIN1 = 1;
			break;
		}
	}
	else				// reverse rotation
	{
		switch(i%4)
		{
		case 0:
			GPIOA->DATAPIN0 = 1;
			GPIOA->DATAPIN1 = 1;
			break;
		
		case 1:
			GPIOA->DATAPIN0 = 1;
			GPIOA->DATAPIN1 = 0;
			break;
		
		case 2:
			GPIOA->DATAPIN0 = 0;
			GPIOA->DATAPIN1 = 0;
			break;
		
		case 3:
			GPIOA->DATAPIN0 = 0;
			GPIOA->DATAPIN1 = 1;
			break;
		}
	}
	
	switch(i%1000)
	{
	case 0: GPIOA->DATAPIN2 = 1; break;
	case 1: GPIOA->DATAPIN2 = 0; break;
	default: break;
	}
	
	i++;
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
