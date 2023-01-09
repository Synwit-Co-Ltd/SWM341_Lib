#include "SWM341.h"

#include "CircleBuffer.h"

CircleBuffer_t CirBuf;

void SerialInit(void);

int main(void)
{
	uint32_t i, n = 0;
	CAN_InitStructure CAN_initStruct;
	uint8_t tx_data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTB, PIN5, PORTB_PIN5_CAN0_RX, 1);	//GPIOB.4����ΪCAN0��������
	PORT_Init(PORTB, PIN4, PORTB_PIN4_CAN0_TX, 0);	//GPIOB.3����ΪCAN0�������
	
	CAN_initStruct.Mode = CAN_MODE_NORMAL;
	CAN_initStruct.CAN_bs1 = CAN_BS1_5tq;
	CAN_initStruct.CAN_bs2 = CAN_BS2_4tq;
	CAN_initStruct.CAN_sjw = CAN_SJW_2tq;
	CAN_initStruct.Baudrate = 50000;
	CAN_initStruct.RXNotEmptyIEn = 1;
	CAN_initStruct.ArbitrLostIEn = 0;
	CAN_initStruct.ErrPassiveIEn = 0;
	CAN_Init(CAN0, &CAN_initStruct);
	
	CAN_SetFilter32b(CAN0, CAN_FILTER_1, 0x00122122, 0x1FFFFFFE);		//����IDΪ0x00122122��0x00122123����չ��
	CAN_SetFilter16b(CAN0, CAN_FILTER_2, 0x122, 0x7FE, 0x101, 0x7FF);	//����IDΪ0x122��123��0x101�ı�׼��
	
	CAN_Open(CAN0);
	
	while(1==1)
	{
		CAN_RXMessage msg;
		
		if(CirBuf_Read(&CirBuf, &msg, 1))
		{
			if(msg.size > 0)
			{
				printf("\r\nReceive %s: %08X, ", msg.format == CAN_FRAME_STD ? "STD" : "EXT", msg.id);
				for(i = 0; i < msg.size; i++) printf("%02X, ", msg.data[i]);
			}
			else if(msg.remote == 1)	//Զ��֡
			{
				printf("\r\nReceive %s Remote Request", msg.format == CAN_FRAME_STD ? "STD" : "EXT");
			}
		}
		
		if(++n % (SystemCoreClock/5) == 0)
			CAN_Transmit(CAN0, CAN_FRAME_STD, 0x177, tx_data, 8, 1);	//������ԣ�����SWM341 CAN��������
	}
}

void CAN0_Handler(void)
{
	CAN_RXMessage msg;
	
	uint32_t int_sr = CAN_INTStat(CAN0);
	
	if(int_sr & CAN_IF_RXDA_Msk)
	{
		CAN_Receive(CAN0, &msg);
		
		CirBuf_Write(&CirBuf, &msg, 1);
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);		//GPIOM.0����ΪUART0��������
 	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);		//GPIOM.1����ΪUART0�������
 	
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

/****************************************************************************************************************************************** 
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
