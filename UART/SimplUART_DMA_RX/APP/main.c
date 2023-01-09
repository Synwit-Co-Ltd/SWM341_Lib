#include "SWM341.h"
#include "CircleBuffer.h"

CircleBuffer_t CirBuf;

#define RX_LEN	32
uint8_t RX_Buffer[2][RX_LEN] = {0};		// RX_Buffer[0]��DMA ǰ��ΰ���ʹ�õ� Buffer
										// RX_Buffer[1]��DMA ���ΰ���ʹ�õ� Buffer

void SerialInit(void);

int main(void)
{
	DMA_InitStructure DMA_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	DMA_initStruct.Mode = DMA_MODE_CIRCLE;
	DMA_initStruct.Unit = DMA_UNIT_BYTE;
	DMA_initStruct.Count = RX_LEN * 2;	// DMA ���˷����Σ�ǰ��κͺ��θ��� RX_LEN
	DMA_initStruct.SrcAddr = (uint32_t)&UART0->DATA;
	DMA_initStruct.SrcAddrInc = 0;
	DMA_initStruct.DstAddr = (uint32_t)RX_Buffer;
	DMA_initStruct.DstAddrInc = 2;		// Scatter-Gather ģʽ
	DMA_initStruct.Handshake = DMA_CH1_UART0RX;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = DMA_IT_DSTSG_HALF | DMA_IT_DSTSG_DONE;
	DMA_CH_Init(DMA_CH1, &DMA_initStruct);
	DMA_CH_Open(DMA_CH1);
   	
	while(1==1)
	{
		uint8_t chr;
		if(CirBuf_Read(&CirBuf, &chr, 1))
			printf("%c", chr);
	}
}


void DMA_Handler(void)
{	
	if(DMA_CH_INTStat(DMA_CH1, DMA_IT_DSTSG_HALF))
	{
		DMA_CH_INTClr(DMA_CH1, DMA_IT_DSTSG_HALF);
		
		CirBuf_Write(&CirBuf, RX_Buffer[0], RX_LEN);
	}
	else if(DMA_CH_INTStat(DMA_CH1, DMA_IT_DSTSG_DONE))
	{
		DMA_CH_INTClr(DMA_CH1, DMA_IT_DSTSG_DONE);
		
		CirBuf_Write(&CirBuf, RX_Buffer[1], RX_LEN);
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);	//GPIOM.0����ΪUART0��������
	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);	//GPIOM.1����ΪUART0�������
 	
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
