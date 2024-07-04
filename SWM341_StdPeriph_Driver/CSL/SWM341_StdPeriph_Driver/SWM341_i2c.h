#ifndef __SWM341_I2C_H__
#define __SWM341_I2C_H__


typedef struct {
	uint8_t  Master;		// 1 master mode, 0 slave mode
	uint32_t MstClk;		// master clock frequency in Hz
	
	uint8_t  Addr10b;		// 1 10-bit address mode, 0 7-bit address mode
	uint16_t SlvAddr;		// slave address
	uint16_t SlvAddrMsk;
	
	uint8_t  TXEmptyIEn;	// TX Register Empty interrupt enable
	uint8_t  RXNotEmptyIEn;	// RX Register Not Empty interrupt enable
	uint8_t  SlvSTADetIEn;	// Slave detected start bit interrupt enable
	uint8_t  SlvSTODetIEn;	// Slave detected stop bit interrupt enable
} I2C_InitStructure;


/* Interrupt Type */
#define I2C_IT_TX_EMPTY		(1 <<  0)	// TX Register Empty
#define I2C_IT_RX_NOT_EMPTY	(1 <<  1)	// RX Register Not Empty
#define I2C_IT_RX_OVF		(1 <<  2)	// RX Register Overflow
#define I2C_IT_TX_DONE		(1 <<  3)	// Send complete (ACK received)
#define I2C_IT_RX_DONE		(1 <<  4)	// Receive complete (send out ACK)
#define I2C_IT_SLV_DET_STA	(1 <<  8)	// Slave detected start bit
#define I2C_IT_SLV_DET_STP	(1 <<  9)	// Slave detected stop bit
#define I2C_IT_ARB_LOST		(1 << 16)	// Master Arbitration lost
#define I2C_IT_SCL_LOW_TO	(1 << 17)	// Master SCL low level timeout


void I2C_Init(I2C_TypeDef * I2Cx, I2C_InitStructure * initStruct);

void I2C_Open(I2C_TypeDef * I2Cx);
void I2C_Close(I2C_TypeDef * I2Cx);

uint8_t I2C_Start(I2C_TypeDef * I2Cx, uint8_t addr, uint8_t wait);
void I2C_Stop(I2C_TypeDef * I2Cx, uint8_t wait);
uint8_t I2C_Write(I2C_TypeDef * I2Cx, uint8_t data, uint8_t wait);
uint8_t I2C_Read(I2C_TypeDef * I2Cx, uint8_t ack, uint8_t wait);

uint8_t I2C_StartDone(I2C_TypeDef * I2Cx);
uint8_t I2C_StopDone(I2C_TypeDef * I2Cx);
uint8_t I2C_WriteDone(I2C_TypeDef * I2Cx);
uint8_t I2C_ReadDone(I2C_TypeDef * I2Cx);
uint8_t I2C_IsAck(I2C_TypeDef * I2Cx);

void I2C_INTEn(I2C_TypeDef * I2Cx, uint32_t it);
void I2C_INTDis(I2C_TypeDef * I2Cx, uint32_t it);
void I2C_INTClr(I2C_TypeDef * I2Cx, uint32_t it);
uint32_t I2C_INTStat(I2C_TypeDef * I2Cx, uint32_t it);


#endif
