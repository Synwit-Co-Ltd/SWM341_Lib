#include "SWM341.h"


/* Error Handling demo:
 *	0) SWM341 CAN <==CAN_RX/TX==> CAN transceiver (e.g. VP230) <==CAN_H/L==> CAN analyzer
 *	1) Disconnect the CAN transceiver from the CAN analyzer, trigger ACK error, and CAN->TXERR increases from 0 to 128, then remains unchanged.
 *	2) Disconnect SWM341 CAN from the CAN transceiver, trigger bit error, and CAN->TXERR increases to 255, then triggers Bus Off.
 *		And then CAN->CR.RST automatically set to 1, SWM341 CAN enters reset state.
 *	3) In ISR, after detecting Bus Off, run CAN_Open(CAN0) to make the SWM341 CAN exit from reset state. 
 *		At this time, restore the above disconnected connection. CAN->TXERR will gradually decrease to 0, and SWM341 CAN will resume normal work.
 */


void SerialInit(void);

int main(void)
{
	CAN_InitStructure CAN_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN5, PORTB_PIN5_CAN0_RX, 1);
	PORT_Init(PORTB, PIN4, PORTB_PIN4_CAN0_TX, 0);
	
	CAN_initStruct.Mode = CAN_MODE_NORMAL;
	CAN_initStruct.CAN_bs1 = CAN_BS1_5tq;
	CAN_initStruct.CAN_bs2 = CAN_BS2_4tq;
	CAN_initStruct.CAN_sjw = CAN_SJW_2tq;
	CAN_initStruct.Baudrate = 50000;
	CAN_initStruct.RXNotEmptyIEn = 1;
	CAN_initStruct.ArbitrLostIEn = 0;
	CAN_initStruct.ErrPassiveIEn = 1;
	CAN_Init(CAN0, &CAN_initStruct);
	
	CAN_INTEn(CAN0, CAN_IT_ERR_WARN | CAN_IT_RX_OVERFLOW);
	
	CAN_SetFilter32b(CAN0, CAN_FILTER_1, 0x00122122, 0x1FFFFFFE);		// accept extended frame with ID 0x00122122 and 0x00122123
	CAN_SetFilter16b(CAN0, CAN_FILTER_2, 0x122, 0x7FE, 0x101, 0x7FF);	// accept standard frame with ID 0x122, 0x123 and 0x101
	
	CAN_Open(CAN0);
	
	while(1==1)
	{
		uint8_t tx_data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
		
		CAN_Transmit(CAN0, CAN_FRAME_STD, 0x133, tx_data, 8, 1);
		while(CAN_TXComplete(CAN0) == 0) __NOP();
		
		printf("\r\nCAN->TXERR: %d\r\n", CAN0->TXERR);
		
		for(int i = 0; i < SystemCoreClock / 16; i++) __NOP();
	}
}

void CAN0_Handler(void)
{
	uint32_t int_sr = CAN_INTStat(CAN0);
	
	if(int_sr & CAN_IF_RXDA_Msk)
	{
		CAN_RXMessage msg;
		
		CAN_Receive(CAN0, &msg);
		
		if(msg.size > 0)
		{
			printf("\r\nReceive %s: %08X, ", msg.format == CAN_FRAME_STD ? "STD" : "EXT", msg.id);
			for(int i = 0; i < msg.size; i++) printf("%02X, ", msg.data[i]);
		}
		else if(msg.remote == 1)	// remote frame
		{
			printf("\r\nReceive %s Remote Request", msg.format == CAN_FRAME_STD ? "STD" : "EXT");
		}
	}
	
	if(int_sr & CAN_IF_RXOV_Msk)
	{
		printf("\r\nCAN RX Overflow\r\n");
		
		CAN_Close(CAN0);
		for(int i = 0; i < CyclesPerUs; i++) __NOP();
		CAN_Open(CAN0);
	}
	
	if(int_sr & CAN_IF_ERRWARN_Msk)
	{
		if(CAN0->SR & CAN_SR_BUSOFF_Msk)
		{
			printf("\r\nCAN Bus Off\r\n");
			printf("\r\nCAN->CR.RST = %d\r\n", CAN0->CR & CAN_CR_RST_Msk ? 1 : 0);
			
			CAN_Open(CAN0);			// Bus Off recovery
		}
		else if(CAN0->SR & CAN_SR_ERRWARN_Msk)
		{
			printf("\r\nCAN Error Warning\r\n");
		}
	}
	
	if(int_sr & CAN_IF_ERRPASS_Msk)
	{
		printf("\r\nCAN Error Passive\r\n");
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
