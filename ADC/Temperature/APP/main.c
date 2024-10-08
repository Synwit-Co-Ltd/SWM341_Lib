#include "SWM341.h"

void SerialInit(void);

int main(void)
{
	uint32_t chn;
	ADC_InitStructure ADC_initStruct;
	ADC_SEQ_InitStructure ADC_SEQ_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	/* Enable temperature sensor, the temperature sensor is connected to CH7 of ADC 1.
	 * The output voltage of the temperature sensor varies with the temperature change at a ratio of 7mV/degree.
	 * The specific ADC reading and temperature conversion relationship can be based on the formula: y = kx + b.
	 * Take two known temperatures and their corresponding ADC readings, and calculate k and b
	 */
	SYS->TEMPCR |= SYS_TEMPCR_EN_Msk;
	
	ADC_initStruct.clk_src = ADC_CLKSRC_HRC_DIV8;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_initStruct.EOC_IEn = 0;
	ADC_initStruct.HalfIEn = 0;
	ADC_Init(ADC1, &ADC_initStruct);
	
	ADC_SEQ_initStruct.channels = ADC_CH7;
	ADC_SEQ_initStruct.trig_src = ADC_TRIGGER_SW;
	ADC_SEQ_initStruct.conv_cnt = 1;
	ADC_SEQ_initStruct.samp_tim = ADC_SAMPLE_1CLOCK;
	ADC_SEQ_Init(ADC1, ADC_SEQ0, &ADC_SEQ_initStruct);
	
	ADC_Open(ADC1);
	ADC_Calibrate(ADC1);
	
	while(1==1)
	{
		ADC_Start(ADC1, ADC_SEQ0);
		while((ADC1->SEQ[0].SR & ADC_SR_EOC_Msk) == 0);
		printf("%4d,", ADC_Read(ADC1, ADC_SEQ0, &chn));
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
