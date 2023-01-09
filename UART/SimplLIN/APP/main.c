#include "SWM341.h"
#include <string.h>

#define UART_RX_LEN	 128
char UART_RXBuffer[UART_RX_LEN] = {0};
volatile uint32_t UART_RXIndex = 0;

char TX_Str1[] = "Hi from Synwit\r\n";
char TX_Str2[] = "SWM320 LIN Demo\r\n";


void SerialInit(void);
void Serial1Init(void);
void LIN_FrameSend(char data[], uint32_t count);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	Serial1Init();
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);
   	
	while(1==1)
	{
		LIN_FrameSend(TX_Str1, sizeof(TX_Str1));
		for(i = 0; i < SystemCoreClock/16; i++) __NOP();
		
		LIN_FrameSend(TX_Str2, sizeof(TX_Str2));
		for(i = 0; i < SystemCoreClock/16; i++) __NOP();
	}
}


void Serial1Init(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTD, PIN4, PORTD_PIN4_UART1_RX, 1);	//GPIOD.4����ΪUART1��������
	PORT_Init(PORTD, PIN3, PORTD_PIN3_UART1_TX, 0);	//GPIOD.3����ΪUART1�������
 	
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
 	UART_Init(UART1, &UART_initStruct);
	
	UART_LINConfig(UART1, 13, 0, 14, 0);
	
	UART_Open(UART1);
}

void LIN_FrameSend(char data[], uint32_t count)
{
	uint32_t i;
	
	UART_LINGenerate(UART1);
	while(UART1->LINCR & UART_LINCR_GENBRK_Msk) __NOP();	//����Զ�����
	
	for(i = 0; i < count; i++)
	{
		while(UART_IsTXFIFOFull(UART1)) __NOP();
		
		UART_WriteByte(UART1, data[i]);
	}
}


void UART0_Handler(void)
{
	uint32_t chr;
	
	if(UART_INTStat(UART0, UART_IT_RX_THR | UART_IT_RX_TOUT))
	{
		while(UART_IsRXFIFOEmpty(UART0) == 0)
		{
			if(UART_ReadByte(UART0, &chr) == 0)
			{
				if(UART_RXIndex < UART_RX_LEN)
				{
					UART_RXBuffer[UART_RXIndex] = chr;
					
					UART_RXIndex++;
				}
			}
		}
		
		if(UART_INTStat(UART0, UART_IT_RX_TOUT))
		{
			UART_INTClr(UART0, UART_IT_RX_TOUT);
		}
	}
	else if(UART_LINIsDetected(UART0))
	{
		UART0->LINCR |= UART_LINCR_BRKDETIF_Msk;	//����жϱ�־
		
		GPIO_InvBit(GPIOA, PIN5);
		
		if(UART_RXIndex)
		{
			printf("LIN frame data:\r\n   %s\r\n", UART_RXBuffer);
			memset(UART_RXBuffer, 0, sizeof(UART_RXBuffer));
			UART_RXIndex = 0;
		}
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
	UART_initStruct.RXThresholdIEn = 1;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 1;
 	UART_Init(UART0, &UART_initStruct);
	
	UART_LINConfig(UART0, 13, 1, 14, 0);
	
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
