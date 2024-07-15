#include "SWM341.h"

#define ADC_SIZE  16	// ADC can convert a maximum of 256 times consecutively
uint16_t ADC_Result[ADC_SIZE] = {0};
volatile int ADC_ValueFound = 0;

#define ADC_LIMIT_MIN	400
#define ADC_LIMIT_MAX	3600


void SerialInit(void);

int main(void)
{
	DMA_InitStructure DMA_initStruct;
	ADC_InitStructure ADC_initStruct;
	ADC_SEQ_InitStructure ADC_SEQ_initStruct;
	ADC_CMP_InitStructure ADC_CMP_initStruct;
	
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
	ADC_initStruct.EOC_IEn = 0;
	ADC_initStruct.HalfIEn = 0;
	ADC_Init(ADC0, &ADC_initStruct);
	
	ADC_SEQ_initStruct.channels = ADC_CH7;
	ADC_SEQ_initStruct.trig_src = ADC_TRIGGER_TIMER2;
	ADC_SEQ_initStruct.conv_cnt = ADC_SIZE;
	ADC_SEQ_initStruct.samp_tim = ADC_SAMPLE_1CLOCK;
	ADC_SEQ_Init(ADC0, ADC_SEQ0, &ADC_SEQ_initStruct);
	
	ADC_CMP_initStruct.LowerLimit = ADC_LIMIT_MIN;
	ADC_CMP_initStruct.LowerLimitIEn = 0;
	ADC_CMP_initStruct.UpperLimit = ADC_LIMIT_MAX;
	ADC_CMP_initStruct.UpperLimitIEn = 1;
	ADC_CMP_Init(ADC0, ADC_SEQ0, &ADC_CMP_initStruct);
	
	ADC_Open(ADC0);
	ADC_Calibrate(ADC0);
	
	ADC0->CR |= (ADC_SEQ0 << ADC_CR_DMAEN_Pos);
	
	
	DMA_initStruct.Mode = DMA_MODE_CIRCLE;
	DMA_initStruct.Unit = DMA_UNIT_HALFWORD;
	DMA_initStruct.Count = ADC_SIZE;
	DMA_initStruct.SrcAddr = (uint32_t)&ADC0->SEQ[0].DR;
	DMA_initStruct.SrcAddrInc = 0;
	DMA_initStruct.DstAddr = (uint32_t)ADC_Result;
	DMA_initStruct.DstAddrInc = 1;
	DMA_initStruct.Handshake = DMA_CH0_ADC0;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	
	DMA_CH_Open(DMA_CH0);
	
	TIMR_Init(TIMR2, TIMR_MODE_TIMER, CyclesPerUs, 1000, 0);
	TIMR_Start(TIMR2);
	
	while(1==1)
	{
		if(ADC_ValueFound)
		{
			ADC_ValueFound = 0;
			
			for(int i = 0; i < ADC_SIZE; i++)
			{
				int adcv = ADC_Result[i] & ADC_DR_VALUE_Msk;
				if(adcv > ADC_LIMIT_MAX)
				{
					printf("ADC Value: %d\n", adcv);
					
					break;
				}
			}
		}
	}
}


void ADC0_Handler(void)
{
	if(ADC_INTStat(ADC0, ADC_SEQ0, ADC_IT_CMP_MAX))
	{
		ADC_INTClr(ADC0, ADC_SEQ0, ADC_IT_CMP_MAX);
		
		ADC_ValueFound = 1;
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
