#include <string.h>
#include "SWM341.h"


void SerialInit(void);
void UART_SendChars(char data[], uint32_t len);

int main(void)
{
	char str_Hi[] = "Hello from Synwit UART!\r\n";
	
	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN2, 1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);
   	
	while(1==1)
	{
		UART_SendChars(str_Hi, strlen(str_Hi));
		
		for(int i = 0; i < SystemCoreClock/8; i++) __NOP();
	}
}


char * UART_TXBuffer = 0;
uint32_t UART_TXCount = 0,
         UART_TXIndex = 0;

void UART_SendChars(char data[], uint32_t len)
{
	UART_TXBuffer = data;
	UART_TXCount = len;
	UART_TXIndex = 0;
	
	UART_INTEn(UART0, UART_IT_TX_THR | UART_IT_TX_DONE);
}


void UART0_Handler(void)
{
	if(UART_INTStat(UART0, UART_IT_TX_THR))
	{
		while(UART_IsTXFIFOFull(UART0) == 0)
		{
			if(UART_TXIndex < UART_TXCount)
			{
				UART_WriteByte(UART0, UART_TXBuffer[UART_TXIndex++]);
			}
			else
			{
				UART_INTDis(UART0, UART_IT_TX_THR);
				
				break;
			}
		}
		
		GPIO_InvBit(GPIOA, PIN2);
	}
	else if(UART_INTStat(UART0, UART_IT_TX_DONE))
	{
		GPIO_InvBit(GPIOA, PIN5);
		
		UART_INTDis(UART0, UART_IT_TX_DONE);
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
	
	NVIC_EnableIRQ(UART0_IRQn);
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
