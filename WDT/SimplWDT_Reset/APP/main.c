#include "SWM341.h"

void SerialInit(void);

int main(void)
{
	uint32_t i;

	SystemInit();
	
	SerialInit();
	
	if(SYS->RSTSR & SYS_RSTSR_WDT_Msk)
	{
		SYS->RSTSR = (1 << SYS_RSTSR_WDT_Pos);
		
		printf("WDT Reset Happened\r\n");
	}
	
	WDT_Init(WDT, 0, 1024);
	WDT_Start(WDT);
	
	while(1==1)
	{
		WDT_Feed(WDT);	// ע�͵�ι����оƬ�᲻ͣ��λ��ӡ��WDT Reset Happened��
		
		/* ע�⣺
		 *	1������ι��֮�����ټ�� 5 �� WDT ʱ�����ڣ��� 1000000 / 32768 * 5 = 150us���ֿ��ǵ� WDT ʱ�����ܴ󣬽�������С�� 300us
		 *	2��WDT ֹͣ״̬�£���Ҫִ�� WDT_Feed()
		 *	3��ִ�� WDT_Feed() �󣬲�������ִ�� WDT_Stop()�������� 5 �� WDT ʱ��������ִ�� WDT_Stop()
		*/

		for(i = 0; i < CyclesPerUs * 300 / 4; i++) __NOP();
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
