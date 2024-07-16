#include "SWM341.h"
#include <string.h>


#define SPI0_CS_Low()	GPIO_ClrBit(GPIOM, PIN3)
#define SPI0_CS_High()	GPIO_SetBit(GPIOM, PIN3)

uint16_t SPI1RXBuffer[32] = {0};
uint32_t SPI1RXIndex = 0;


void SerialInit(void);
void SPIMstInit(void);
void SPISlvInit(void);

int main(void)
{	
	uint32_t n, i;
	uint32_t rxdata, txdata = 0x23;
	
	SystemInit();
	
	SerialInit();
	
	SPIMstInit();
	
	SPISlvInit();
	
	while(1==1)
	{
		SPI0_CS_Low();
		for(i = 0; i < 1200; i++);
		for(i = 0; i < 16; i++)
		{
			SPI0->DATA = i;
			while((SPI0->STAT & SPI_STAT_TFNF_Msk) == 0) __NOP();
		}
		while(SPI_IsTXEmpty(SPI0) == 0) __NOP();
		for(i = 0; i < 12000; i++) __NOP();		// Although TX FIFO is empty, the last data is still in the TX shift register and needs to wait for it to be sent out
		SPI0_CS_High();
		
		for(i = 0; i < SystemCoreClock/4; i++) __NOP();
	}
}


void SPIMstInit(void)
{
	SPI_InitStructure SPI_initStruct;
	
	GPIO_Init(GPIOM, PIN3, 1, 0, 0, 0);
	SPI0_CS_High();
	
	PORT_Init(PORTM, PIN2, PORTM_PIN2_SPI0_SCLK, 0);
	PORT_Init(PORTM, PIN4, PORTM_PIN4_SPI0_MISO, 1);
	PORT_Init(PORTM, PIN5, PORTM_PIN5_SPI0_MOSI, 0);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_32;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThreshold = 0;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold = 0;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	SPI_Open(SPI0);
}


void SPISlvInit(void)
{
	SPI_InitStructure SPI_initStruct;
	
	PORT_Init(PORTB, PIN2, PORTB_PIN2_SPI1_SCLK, 1);
	PORT_Init(PORTB, PIN3, PORTB_PIN3_SPI1_MISO, 0);
	PORT_Init(PORTB, PIN4, PORTB_PIN4_SPI1_MOSI, 1);
	PORT_Init(PORTB, PIN5, PORTB_PIN5_SPI1_SSEL, 1);
	
	SPI_initStruct.clkDiv = 1;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_SECOND_EDGE;
	SPI_initStruct.IdleLevel = SPI_HIGH_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 0;
	SPI_initStruct.RXThreshold = 4;
	SPI_initStruct.RXThresholdIEn = 1;
	SPI_initStruct.TXThreshold = 0;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn = 0;
	SPI_Init(SPI1, &SPI_initStruct);
	
	SPI_INTEn(SPI1, SPI_IT_RX_OVF);			// The SPI no longer receives data after overflow, so the RX FIFO needs to be cleared in the overflow ISR
	
	SPI1->IE |= (1 << SPI_IE_SSRISE_Pos);	// enable SPI_SSEL rising edge interrupt
	
	SPI_Open(SPI1);
}

void SPI1_Handler(void)
{	
	if(SPI_INTStat(SPI1, SPI_IT_RX_OVF))
	{
		SPI1->CTRL |= SPI_CTRL_RFCLR_Msk;
		__NOP();__NOP();__NOP();__NOP();
		SPI1->CTRL &= ~SPI_CTRL_RFCLR_Msk;

		SPI_INTClr(SPI1, SPI_IT_RX_OVF);
	}
	
	if(SPI1->IF & SPI_IF_RFTHR_Msk)
	{
		while(SPI1->STAT & SPI_STAT_RFNE_Msk)
		{
			SPI1RXBuffer[SPI1RXIndex++] = SPI1->DATA;
		}
		
		SPI1->IF = (1 << SPI_IF_RFTHR_Pos);
	}
	
	if(SPI1->IF & SPI_IF_SSRISE_Msk)
	{
		while(SPI1->STAT & SPI_STAT_RFNE_Msk)
		{
			SPI1RXBuffer[SPI1RXIndex++] = SPI1->DATA;
		}
		
		SPI1->IF = (1 << SPI_IF_SSRISE_Pos);
		
		/* detect rising edge on SPI_SSEL pin, so one frame is received */
		for(uint32_t i = 0; i < SPI1RXIndex; i++)
			printf("%d, ", SPI1RXBuffer[i]);
		printf("\r\n\r\n");
		
		SPI1RXIndex = 0;
		memset(SPI1RXBuffer, 0, sizeof(SPI1RXBuffer));
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
