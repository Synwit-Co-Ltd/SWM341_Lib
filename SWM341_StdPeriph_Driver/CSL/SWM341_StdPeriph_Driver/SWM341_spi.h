#ifndef __SWM341_SPI_H__
#define __SWM341_SPI_H__


typedef struct {
	uint8_t  FrameFormat;		// SPI_FORMAT_SPI, SPI_FORMAT_TI_SSI
	uint8_t  SampleEdge;		// SPI_FIRST_EDGE, SPI_SECOND_EDGE
	uint8_t  IdleLevel;			// SPI_LOW_LEVEL, SPI_HIGH_LEVEL
	uint8_t  WordSize;			// 4-16
	uint8_t  Master;			// 1 master mode, 0 slave mode
	uint8_t  clkDiv;			// SPI_CLK = SYS_CLK / clkDiv, can be SPI_CLKDIV_4, SPI_CLKDIV_8, ..., SPI_CLKDIV_512
	
	uint8_t  RXThreshold;		// 1--8
	uint8_t  RXThresholdIEn;	// when count of data in RX FIFO >= RXThreshold, generate RXThreshold interrupt
	
	uint8_t  TXThreshold;		// 0--7
	uint8_t  TXThresholdIEn;	// when count of data in TX FIFO <= TXThreshold, generate TXThreshold interrupt
	
	uint8_t  TXCompleteIEn;		// TX FIFO and TX shift register empty interrupt enable
} SPI_InitStructure;


#define SPI_FORMAT_SPI			0		// Motorola SPI fromat
#define SPI_FORMAT_TI_SSI		1		// TI SSI format
#define SPI_FORMAT_I2S			2

#define SPI_FIRST_EDGE			0		// sample at first clock edge
#define SPI_SECOND_EDGE			1		// sample at second clock edge

#define SPI_LOW_LEVEL			0		// when idle, clock line remain low level
#define SPI_HIGH_LEVEL			1		// when idle, clock line remain high level

#define SPI_CLKDIV_2			8
#define SPI_CLKDIV_4			0
#define SPI_CLKDIV_8			1
#define SPI_CLKDIV_16			2
#define SPI_CLKDIV_32			3
#define SPI_CLKDIV_64			4
#define SPI_CLKDIV_128			5
#define SPI_CLKDIV_256			6
#define SPI_CLKDIV_512			7


/* Interrupt Type */
#define SPI_IT_RX_OVF		(1 << 0)	// RX FIFO Overflow
#define SPI_IT_RX_FULL		(1 << 1)	// RX FIFO Full
#define SPI_IT_RX_HFULL		(1 << 2)	// RX FIFO Half Full
#define SPI_IT_TX_EMPTY		(1 << 3)	// TX FIFO Empty
#define SPI_IT_TX_HFULL		(1 << 4)	// TX FIFO Half Full
#define SPI_IT_RX_THRES		(1 << 5)	// RX FIFO Threshold, count of data in RX FIFO >= RXThreshold
#define SPI_IT_TX_THRES		(1 << 6)	// TX FIFO Threshold, count of data in TX FIFO <= TXThreshold
#define SPI_IT_TX_DONE		(1 << 9)	// TX Done, TX FIFO and TX shift register empty
#define SPI_IT_CS_FALL		(1 << 10)	// only for slave mode, falling edge on CS line
#define SPI_IT_CS_RISE		(1 << 11)	// only for slave mode, rising edge on CS line


void SPI_Init(SPI_TypeDef * SPIx, SPI_InitStructure * initStruct);
void SPI_Open(SPI_TypeDef * SPIx);
void SPI_Close(SPI_TypeDef * SPIx);

uint32_t SPI_Read(SPI_TypeDef * SPIx);
void SPI_Write(SPI_TypeDef * SPIx, uint32_t data);
void SPI_WriteWithWait(SPI_TypeDef * SPIx, uint32_t data);
uint32_t SPI_ReadWrite(SPI_TypeDef * SPIx, uint32_t data);

uint32_t SPI_IsRXEmpty(SPI_TypeDef * SPIx);
uint32_t SPI_IsTXFull(SPI_TypeDef * SPIx);
uint32_t SPI_IsTXEmpty(SPI_TypeDef * SPIx);


void SPI_INTEn(SPI_TypeDef * SPIx, uint32_t it);
void SPI_INTDis(SPI_TypeDef * SPIx, uint32_t it);
void SPI_INTClr(SPI_TypeDef * SPIx, uint32_t it);
uint32_t SPI_INTStat(SPI_TypeDef * SPIx, uint32_t it);




typedef struct {
	uint8_t  Mode;				// I2S_MASTER_TX, I2S_MASTER_RX, I2S_MASTER_TX_RX, I2S_SLAVE_TX, I2S_SLAVE_RX, I2S_SLAVE_TX_RX
	uint8_t  FrameFormat;		// I2S_I2S_PHILIPS, I2S_MSB_JUSTIFIED, I2S_PCM_SHORT, I2S_PCM_LONG0, I2S_PCM_LONG1
	uint8_t  ChannelLen;		// I2S_CHNNLEN_16, I2S_CHNNLEN_32
	uint8_t  DataLen;			// I2S_DATALEN_8, I2S_DATALEN_16, I2S_DATALEN_24, I2S_DATALEN_32
	uint32_t ClkFreq;			// I2S_SCLK Frequency
	
	uint8_t  RXThreshold;
	uint8_t  RXThresholdIEn;
	
	uint8_t  TXThreshold;
	uint8_t  TXThresholdIEn;
	
	uint8_t  TXCompleteIEn;
} I2S_InitStructure;


#define I2S_MASTER_RX		5
#define I2S_MASTER_TX		6
#define I2S_SLAVE_RX		1
#define I2S_SLAVE_TX		2

#define I2S_I2S_PHILIPS		0
#define I2S_MSB_JUSTIFIED	1
#define I2S_PCM_SHORT		2
#define I2S_PCM_LONG0		3	// PCM Long Mode Sync Width 1 SCLK period
#define I2S_PCM_LONG1		4	// PCM Long Mode Sync Width 1 Data Length

#define I2S_CHNNLEN_16		0
#define I2S_CHNNLEN_32		1

#define I2S_DATALEN_8		0
#define I2S_DATALEN_16		1
#define I2S_DATALEN_24		2
#define I2S_DATALEN_32		3


void I2S_Init(SPI_TypeDef * SPIx, I2S_InitStructure * initStruct);
void I2S_Open(SPI_TypeDef * SPIx);
void I2S_Close(SPI_TypeDef * SPIx);


#endif
