#include "SWM341.h"

#include <string.h>

#define SLV_ADDR  0x6C

char mst_txbuff[4] = {0x37, 0x55, 0xAA, 0x78};
char mst_rxbuff[4] = {0};
char slv_txbuff[4] = {0};
char slv_rxbuff[5] = {0};	// first item is address

volatile uint32_t mst_txindx = 0;
volatile uint32_t mst_rxindx = 0;
volatile uint32_t slv_rxindx = 0;
volatile uint32_t slv_txindx = 0;

#define MST_OP_TX	  1
#define MST_OP_RX	  2
volatile uint32_t mst_oper = 0;

#define MST_RES_SUCC  1
#define MST_RES_FAIL  2
volatile uint32_t mst_result = 0;

void SerialInit(void);
void I2CMstInit(void);
void I2CSlvInit(void);

int main(void)
{
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	I2CMstInit();
	I2CSlvInit();
	
	NVIC_SetPriority(I2C0_IRQn, 3);
	NVIC_SetPriority(I2C1_IRQn, 1);	// Set the priority of the slave to be higher than that of the host
	
	while(1==1)
	{
		/*************************** Master Write ************************************/
		mst_oper = MST_OP_TX;
		mst_result = 0;
		mst_txindx = 0;
		slv_rxindx = 0;
		
		I2C_Start(I2C0, (SLV_ADDR << 1) | 0, 0);
		
		while(mst_result != MST_RES_SUCC)
		{
			if(mst_result == MST_RES_FAIL)
				goto nextloop;
		}
		
		printf("Master Send %X %X %X %X\r\n", mst_txbuff[0], mst_txbuff[1], mst_txbuff[2], mst_txbuff[3]);
		
		/********************************** Master Read *******************************/
		mst_oper = MST_OP_RX;
		mst_result = 0;
		mst_rxindx = 0;
		slv_txindx = 0;
		memcpy(slv_txbuff, &slv_rxbuff[1], 4);
		
		I2C_Start(I2C0, (SLV_ADDR << 1) | 1, 0);
		
		while(mst_result != MST_RES_SUCC)
		{
			if(mst_result == MST_RES_FAIL)
				goto nextloop;
		}
		
		printf("Master Read %X %X %X %X\r\n", mst_rxbuff[0], mst_rxbuff[1], mst_rxbuff[2], mst_rxbuff[3]);
		
		if((mst_txbuff[0] == mst_rxbuff[0]) && (mst_txbuff[1] == mst_rxbuff[1]) && (mst_txbuff[2] == mst_rxbuff[2]) && (mst_txbuff[3] == mst_rxbuff[3]))
			printf("Success\r\n");
		else
			printf("Fail\r\n");

nextloop:
		I2C_Stop(I2C0, 1);
		for(i = 0; i < SystemCoreClock/3; i++) __NOP();
	}
}


void I2CMstInit(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTN, PIN5, PORTN_PIN5_I2C0_SCL, 1);
	PORTN->OPEND |= (1 << PIN5);	// open-drain
	PORTN->PULLU |= (1 << PIN5);	// pull-up
	PORT_Init(PORTN, PIN4, PORTN_PIN4_I2C0_SDA, 1);
	PORTN->OPEND |= (1 << PIN4);
	PORTN->PULLU |= (1 << PIN4);
	
	I2C_initStruct.Master = 1;
	I2C_initStruct.MstClk = 100000;
	I2C_initStruct.Addr10b = 0;
	I2C_initStruct.TXEmptyIEn = 0;
	I2C_initStruct.RXNotEmptyIEn = 0;
	I2C_Init(I2C0, &I2C_initStruct);
	
	I2C_INTEn(I2C0, I2C_IT_TX_DONE);
	/* note: The RX_NOT_EMPTY interrupt cannot be used to replace the RX_DONE interrupt in this routine, 
	 *		because when RX_NOT_EMPTY occurs, the receive is not complete, I2C0->MCR.RD is not cleared.
	 *		In this case, enter interrupt and run I2C_Read() to set the I2C0->MCR.RD bit to be invalid. 
	 *		As a result, the next data cannot be received.
	 */
	I2C_INTEn(I2C0, I2C_IT_RX_DONE);
	NVIC_EnableIRQ(I2C0_IRQn);
	
	I2C_Open(I2C0);
}

void I2C0_Handler(void)
{
	if(I2C0->IF & I2C_IF_TXDONE_Msk)			// transmit complete
	{
		I2C0->IF = I2C_IF_TXDONE_Msk;
		
		if(mst_oper == MST_OP_TX)
		{
			if(I2C_IsAck(I2C0))
			{
				if(mst_txindx < 4)
				{
					I2C_Write(I2C0, mst_txbuff[mst_txindx], 0);
					
					mst_txindx++;
				}
				else
				{
					I2C_Stop(I2C0, 0);
					
					mst_result = MST_RES_SUCC;
				}
			}
			else
			{
				if(mst_txindx == 0)
					printf("Slave send NACK for address\r\n");
				else
					printf("Slave send NACK for data\r\n");
				
				mst_result = MST_RES_FAIL;
			}
		}
		else // mst_oper == MST_OP_RX
		{
			if(I2C_IsAck(I2C0))
			{
				I2C_Read(I2C0, 1, 0);
			}
			else
			{
				printf("Slave send NACK for address\r\n");
				
				mst_result = MST_RES_FAIL;
			}
		}
	}
	else if(I2C0->IF & I2C_IF_RXDONE_Msk)		// receive complete
	{
		I2C0->IF = I2C_IF_RXDONE_Msk;
		
		if(mst_rxindx < 4)
		{
			mst_rxbuff[mst_rxindx] = I2C0->RXDATA;
			
			mst_rxindx++;
			
			if(mst_rxindx < 3)
			{
				I2C_Read(I2C0, 1, 0);
			}
			else if(mst_rxindx < 4)
			{
				I2C_Read(I2C0, 0, 0);
			}
			else
			{
				I2C_Stop(I2C0, 0);
				
				mst_result = MST_RES_SUCC;
			}
		}
	}
}


void I2CSlvInit(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTC, PIN5, PORTC_PIN5_I2C1_SCL, 1);
	PORTC->OPEND |= (1 << PIN5);	// open-drain
	PORTC->PULLU |= (1 << PIN5);	// pull-up
	PORT_Init(PORTC, PIN4, PORTC_PIN4_I2C1_SDA, 1);
	PORTC->OPEND |= (1 << PIN4);
	PORTC->PULLU |= (1 << PIN4);
	
	I2C_initStruct.Master = 0;
	I2C_initStruct.MstClk = 0;
	I2C_initStruct.Addr10b = 0;
	I2C_initStruct.SlvAddr = SLV_ADDR;
	I2C_initStruct.TXEmptyIEn = 1;
	I2C_initStruct.RXNotEmptyIEn = 1;
	I2C_initStruct.SlvSTADetIEn  = 1;
	I2C_initStruct.SlvSTODetIEn  = 1;
	I2C_Init(I2C1, &I2C_initStruct);
	
	I2C_Open(I2C1);
}

void I2C1_Handler(void)
{
	uint32_t i;
	
	if(I2C1->IF & I2C_IF_RXSTA_Msk)					// start bit received
	{
		I2C1->IF = (1 << I2C_IF_RXSTA_Pos);
		
		slv_rxindx = 0;
		
		I2C1->TR = (1 << I2C_TR_TXCLR_Pos);			// Cannot write when there is data
		I2C1->TXDATA = slv_txbuff[0];
		slv_txindx = 1;
	}
	else if(I2C1->IF & I2C_IF_RXNE_Msk)				// RX register is not empty
	{
		slv_rxbuff[slv_rxindx] = I2C1->RXDATA;
		if(slv_rxindx < 4) slv_rxindx++;
	}
	else if(I2C1->IF & I2C_IF_TXE_Msk)				// TX register is empty
	{
		I2C1->TXDATA = slv_txbuff[slv_txindx];
		if(slv_txindx < 3) slv_txindx++;
	}
	else if(I2C1->IF & I2C_IF_RXSTO_Msk)			// stop bit received
	{
		I2C1->IF = (1 << I2C_IF_RXSTO_Pos);
		
		for(i = 0; i < 4; i++) slv_txbuff[i] = slv_rxbuff[1+i];
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
