#include "SWM341.h"

/* ע�⣺
	1������STOP��ֻ��ͨ��Wake-up���Ż��ѣ����ܰ���λ������λоƬ
	2����STOPģʽWake-up������ͷ��ʼ����ִ�У���Backup�������ݿ��Ա���
	3��Wake-up������PD6��PD15���½��ػ��ѣ������ڼ���Ҫ���ָߵ�ƽ
*/

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);			//����32K��Ƶʱ��
	
	printf("BACKUP[0]: %08X\r\n", SYS->BACKUP[0]);	//ע�⣺���Է���û�б��ֹ���
	SYS->BACKUP[0] += 1;
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//����� ��LED
	
	PORT_Init(PORTD, PIN6, PORTD_PIN6_WAKEUP, 1);
	PORTD->PULLU |= (1 << PIN6);
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//����LED
		for(i = 0; i < SystemCoreClock/4; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN5);					//Ϩ��LED
		
		SYS->SLEEP |= (1 << SYS_SLEEP_STOP_Pos);	//����STOPģʽ
	
		for(i = 0; i < 5000; i++);
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
