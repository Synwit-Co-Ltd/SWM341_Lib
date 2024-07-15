#include "SWM341.h"

void SerialInit(void);

int main(void)
{
	ADC_InitStructure ADC_initStruct;
	ADC_SEQ_InitStructure ADC_SEQ_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTC, PIN6,  PORTC_PIN6_ADC0_CH0,   0);	//PC.6  => ADC0.CH0
	PORT_Init(PORTC, PIN5,  PORTC_PIN5_ADC0_CH1,   0);	//PC.5  => ADC0.CH1
	PORT_Init(PORTC, PIN4,  PORTC_PIN4_ADC0_CH2,   0);	//PC.4  => ADC0.CH2
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_ADC0_CH3,   0);	//PC.3  => ADC0.CH3
	PORT_Init(PORTC, PIN2,  PORTC_PIN2_ADC0_CH4,   0);	//PC.2  => ADC0.CH4
	PORT_Init(PORTC, PIN1,  PORTC_PIN1_ADC0_CH5,   0);	//PC.1  => ADC0.CH5
	PORT_Init(PORTC, PIN0,  PORTC_PIN0_ADC0_CH6,   0);	//PC.0  => ADC0.CH6
	PORT_Init(PORTA, PIN15, PORTA_PIN15_ADC0_CH7,  0);	//PA.15 => ADC0.CH7
	PORT_Init(PORTA, PIN14, PORTA_PIN14_ADC0_CH8,  0);	//PA.14 => ADC0.CH8
	PORT_Init(PORTA, PIN13, PORTA_PIN13_ADC0_CH9,  0);	//PA.13 => ADC0.CH9
	PORT_Init(PORTA, PIN12, PORTA_PIN12_ADC0_CH10, 0);	//PA.12 => ADC0.CH10
	PORT_Init(PORTA, PIN10, PORTA_PIN10_ADC0_CH11, 0);	//PA.10 => ADC0.CH11
	
	ADC_initStruct.clk_src = ADC_CLKSRC_HRC_DIV8;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_initStruct.EOC_IEn = ADC_SEQ0;	
	ADC_initStruct.HalfIEn = 0;
	ADC_Init(ADC0, &ADC_initStruct);
	
	ADC_SEQ_initStruct.channels = ADC_CH5;
	ADC_SEQ_initStruct.trig_src = ADC_TRIGGER_SW;
	ADC_SEQ_initStruct.conv_cnt = 1;
	ADC_SEQ_initStruct.samp_tim = ADC_SAMPLE_1CLOCK;
	ADC_SEQ_Init(ADC0, ADC_SEQ0, &ADC_SEQ_initStruct);
	
	ADC_Open(ADC0);
	ADC_Calibrate(ADC0);
	
	ADC_Start(ADC0, ADC_SEQ0);
	
	while(1==1)
	{
	}
}

void ADC0_Handler(void)
{
	uint32_t chn;
	
	ADC0->IF = (1 << ADC_IF_SEQ0EOC_Pos);
	
	printf("%4d,", ADC_Read(ADC0, ADC_SEQ0, &chn));
	
	ADC_Start(ADC0, ADC_SEQ0);
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
