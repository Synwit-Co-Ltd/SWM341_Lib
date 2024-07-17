#include "SWM341.h"

#define LIN_ID_Sensor	0x11	// This slave is a sensor from which data can only be read
#define LIN_ID_Switch	0x21	// This slave is a switch to which data can only be written

#define LIN_NB_Sensor	6		// Number of Byte, the data length of the sensor is 6 bytes
#define LIN_NB_Switch	4

uint8_t Buffer_Sensor[LIN_NB_Sensor] = { 0x12, 0x34, 0x56, 0x78, 0x55, 0xAA };
uint8_t Buffer_Switch[LIN_NB_Switch] = { 0 };


volatile uint8_t  LIN_Now_ID = 0;

volatile uint32_t LIN_Rcv_Index = 0;
volatile bool     LIN_Rcv_Complete = false;

volatile uint32_t LIN_Trx_Index = 0;


void SerialInit(void);
void LIN_Slave_Init(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	LIN_Slave_Init();
	
	while(1==1)
	{
		if(LIN_Rcv_Complete)
		{
			LIN_Rcv_Complete = false;
			
			printf("Data from Master: ");
			for(int i = 0; i < LIN_NB_Switch; i++)
			{
				printf("%02X, ", Buffer_Switch[i]);
			}
			printf("\n\n");
		}
	}
}


void LIN_Slave_Init(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTD, PIN4, PORTD_PIN4_UART1_RX, 1);
	PORT_Init(PORTD, PIN3, PORTD_PIN3_UART1_TX, 0);
 	
 	UART_initStruct.Baudrate = 9600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 0;
	UART_initStruct.RXThresholdIEn = 1;
	UART_initStruct.TXThreshold = 0;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART1, &UART_initStruct);
	
	UART_LINConfig(UART1, 13, 1, 14, 0);
	
	UART_Open(UART1);
}


void UART1_Handler(void)
{
	if(UART_LININTStat(UART1, UART_IT_LIN_DET))
	{
		UART_LININTClr(UART1, UART_IT_LIN_DET);
		
		LIN_Rcv_Index = 0;
		LIN_Trx_Index = 0;
	}
	else if(UART_INTStat(UART1, UART_IT_RX_THR))
	{
		uint32_t chr;
		
		UART_ReadByte(UART1, &chr);
		
		if(LIN_Rcv_Index == 0)
		{
			if(chr != 0x55)
			{
				/* TODO: baudrate error */
			}
		}
		else if(LIN_Rcv_Index == 1)
		{
			uint8_t id_parity = UART_LIN_IDParity(chr);
			if(chr != id_parity)
			{
				/* TODO: ID parity error */
			}
			
			LIN_Now_ID = chr;
			if((LIN_Now_ID & 0x3F) == LIN_ID_Sensor)
			{
				UART_INTEn(UART1, UART_IT_TX_THR);
			}
		}
		else if((LIN_Now_ID & 0x3F) == LIN_ID_Switch)
		{
			if(LIN_Rcv_Index < 2 + LIN_NB_Switch)
			{
				Buffer_Switch[LIN_Rcv_Index - 2] = chr;
			}
			else if(LIN_Rcv_Index == 2 + LIN_NB_Switch)
			{
				uint8_t checksum = UART_LIN_Checksum(LIN_Now_ID, Buffer_Switch, LIN_NB_Switch, false);
				if(chr != checksum)
				{
					/* TODO: Checksum error */
				}
				
				LIN_Rcv_Complete = true;
			}
		}
		LIN_Rcv_Index++;
	}
	else if(UART_INTStat(UART1, UART_IT_TX_THR))
	{
		if(LIN_Trx_Index < LIN_NB_Sensor)
		{
			UART_WriteByte(UART1, Buffer_Sensor[LIN_Trx_Index++]);
		}
		else if(LIN_Trx_Index == LIN_NB_Sensor)
		{
			uint8_t checksum = UART_LIN_Checksum(LIN_Now_ID, Buffer_Sensor, LIN_NB_Sensor, false);
			
			UART_WriteByte(UART1, checksum);
			
			UART_INTDis(UART1, UART_IT_TX_THR);
		}
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
