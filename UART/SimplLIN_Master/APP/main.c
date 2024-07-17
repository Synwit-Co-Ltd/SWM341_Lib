#include "SWM341.h"


#define LIN_ID_Sensor	0x11	// This slave is a sensor from which data can only be read
#define LIN_ID_Switch	0x21	// This slave is a switch to which data can only be written

#define LIN_NB_Sensor	6		// Number of Byte, the data length of the sensor is 6 bytes
#define LIN_NB_Switch	4

uint8_t Buffer_Sensor[LIN_NB_Sensor] = { 0 };
uint8_t Buffer_Switch[LIN_NB_Switch] = { 0x12, 0x34, 0x56, 0x78 };


volatile uint8_t  LIN_Now_ID = 0;
volatile bool	  LIN_Now_enhanced_checksum = false;

volatile uint32_t LIN_Rcv_Index = 0;
volatile bool     LIN_Rcv_Complete = false;

volatile uint32_t LIN_Trx_Index = 0;


void SerialInit(void);
void LIN_Master_Init(void);
void LIN_Master_Start(uint8_t lin_id, bool enhanced_checksum);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	LIN_Master_Init();
	
	TIMR_Init(BTIMR0, TIMR_MODE_TIMER, CyclesPerUs, 1000000/9600, 1);	// After break is sent, the subsequent content is sent after 1 bit time delay
	
	TIMR_Init(BTIMR1, TIMR_MODE_TIMER, CyclesPerUs, 20000, 0);
	
	while(1==1)
	{
		LIN_Master_Start(LIN_ID_Sensor, false);		// read data from the sensor
		
		TIMR_Stop(BTIMR1);
		TIMR_Start(BTIMR1);
		TIMR_INTClr(BTIMR1, TIMR_IT_TO);
		while(!LIN_Rcv_Complete)
		{
			if(TIMR_INTStat(BTIMR1, TIMR_IT_TO))	// no data is received within 20ms
			{
				goto retry;
			}
		}
		LIN_Rcv_Complete = false;
		
		printf("Data from Sensor: ");
		for(int i = 0; i < LIN_NB_Sensor; i++)
		{
			printf("%02X, ", Buffer_Sensor[i]);
		}
		printf("\n\n");
		
		LIN_Master_Start(LIN_ID_Switch, false);		// write data to the switch
		
retry:
		for(int i = 0; i < SystemCoreClock/4; i++) __NOP();
	}
}


void LIN_Master_Init(void)
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
	
	UART_LINConfig(UART1, 13, 0, 14, 1);
	
	UART_Open(UART1);
}


void LIN_Master_Start(uint8_t lin_id, bool enhanced_checksum)
{
	LIN_Now_ID = lin_id;
	LIN_Now_enhanced_checksum = enhanced_checksum;
	
	UART_LINGenerate(UART1);
}


void UART1_Handler(void)
{
	if(UART_LININTStat(UART1, UART_IT_LIN_GEN))
	{
		UART_LININTClr(UART1, UART_IT_LIN_GEN);
		
		LIN_Rcv_Index = 0;
		LIN_Trx_Index = 0;
		
		TIMR_Start(BTIMR0);
	}
	else if(UART_INTStat(UART1, UART_IT_RX_THR))
	{
		uint32_t chr;
		
		UART_ReadByte(UART1, &chr);
		
		if(LIN_Rcv_Index < 2)
		{
			// 0x55 and ID issued by the master pass through the transceiver and are also received by the master itself
		}
		else if(LIN_Rcv_Index < 2 + LIN_NB_Sensor)
		{
			Buffer_Sensor[LIN_Rcv_Index - 2] = chr;
		}
		else if(LIN_Rcv_Index == 2 + LIN_NB_Sensor)
		{
			/* TODO: calculate the checksum and compare */
			
			LIN_Rcv_Complete = true;
		}
		LIN_Rcv_Index++;
	}
	else if(UART_INTStat(UART1, UART_IT_TX_THR))
	{
		if(LIN_Trx_Index < LIN_NB_Switch)
		{
			UART_WriteByte(UART1, Buffer_Switch[LIN_Trx_Index++]);
		}
		else if(LIN_Trx_Index == LIN_NB_Switch)
		{
			uint8_t checksum = UART_LIN_Checksum(LIN_Now_ID, Buffer_Switch, LIN_NB_Switch, LIN_Now_enhanced_checksum);
			
			UART_WriteByte(UART1, checksum);
			
			UART_INTDis(UART1, UART_IT_TX_THR);
		}
	}
}


void BTIMR0_Handler(void)
{
	TIMR_Stop(BTIMR0);
	TIMR_INTClr(BTIMR0, TIMR_IT_TO);
	
	UART_WriteByte(UART1, 0x55);
	UART_WriteByte(UART1, UART_LIN_IDParity(LIN_Now_ID));
	
	if(LIN_Now_ID == LIN_ID_Switch)
	{
		UART_INTEn(UART1, UART_IT_TX_THR);
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
