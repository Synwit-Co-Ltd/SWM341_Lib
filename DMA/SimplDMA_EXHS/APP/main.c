#include "SWM341.h"
#include <string.h>


/* note 1. when use DMA_EXHS_ handshake signal, DMA's other channels can not work
 * note 2. when use DMA_EXHS_ handshake signal, DMA's mode can only be DMA_MODE_SINGLE.
 * note 3. when use DMA_EXHS_ handshake signal, DMA's transfer can only be terminated by clear EXTHSEN bit.
*/

char str_hi[] = "Hi from Synwit\n";


void SerialInit(void);

int main(void)
{	
	DMA_InitStructure DMA_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = strlen(str_hi);
	DMA_initStruct.SrcAddr = (uint32_t)str_hi;
	DMA_initStruct.SrcAddrInc = 1;
	DMA_initStruct.DstAddr = (uint32_t)&UART0->DATA;
	DMA_initStruct.DstAddrInc = 0;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = 0;
	
#if 1
	DMA_initStruct.Handshake = DMA_EXHS_TIMR0;
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 500000, 0);	// each time TIMR's counter overflows, DMA transfer a data to UART0->DATA register
	TIMR_Start(TIMR0);
#else
	DMA_initStruct.Handshake = DMA_EXHS_TRIG0;
	
	PORT_Init(PORTN, PIN5, PORTN_PIN5_DMA_TRIG0, 1);	// each time a rising edge appears on PN5 pin, DMA transfer a data to UART0->DATA register
	PORTN->PULLU |= (1 << PIN5);
	
//	PORT_Init(PORTB, PIN0, PORTB_PIN0_DMA_TRIG1, 1);	// each time a rising edge appears on PB0 pin, DMA transfer a data to UART0->DATA register
//	PORTB->PULLU |= (1 << PIN0);
#endif

	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	DMA_CH_Open(DMA_CH0);
	
	while(1==1)
	{
	}
}


void DMA_Handler(void)
{
	if(DMA_CH_INTStat(DMA_CH0, DMA_IT_DONE))
	{
		DMA_CH_INTClr(DMA_CH0, DMA_IT_DONE);
		
		DMA->CH[0].MUX &= ~DMA_MUX_EXTHSEN_Msk;
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
