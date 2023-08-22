#include "SWM341.h"

uint32_t Buffer[512/4] = {
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
	0x0F00FF00, 0x0FFF0000, 0xFFFF0000, 0x0FFFFF00, 0x0000FFFF, 0xFF000000, 0x0FFFFFFF, 0x00000000,
};

void SerialInit(void);

int main(void)
{
 	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTM, PIN2, PORTM_PIN2_SD_CLK, 0);
	PORT_Init(PORTM, PIN4, PORTM_PIN4_SD_CMD, 1);
	PORT_Init(PORTM, PIN5, PORTM_PIN5_SD_D0,  1);
	PORT_Init(PORTM, PIN6, PORTM_PIN6_SD_D1,  1);
	PORT_Init(PORTN, PIN0, PORTN_PIN0_SD_D2,  1);
	PORT_Init(PORTN, PIN1, PORTN_PIN1_SD_D3,  1);
	
	SDIO_IO_Init(1000000, SDIO_4bit);
	
	/* ������ӻ����� GO_IDLE ����ӻ��ɽ���������Ϊ�����λָ�� */
	SDIO_SendCmd(SD_CMD_GO_IDLE_STATE, 0x00, SD_RESP_NO, 0);
	
	SDIO_IO_ByteWrite(0, 0x02, 0xFE);		// Enable all Function
	
	SDIO_IO_ByteWrite(0, 0x07, 0x02);		// Data bus with, 0 for 1-bit, 2 for 4-bit
	
	SDIO_IO_ByteWrite(0, 0x111, 512 >> 8);	// I/O block size for function 1
	SDIO_IO_ByteWrite(0, 0x110, 512 & 0xFF);
	
	/* ������ӻ��������� */
	SDIO_IO_BlockWrite(1, 0x00, 0, Buffer, 512);
	
	SDIO_IO_MultiBlockWrite(1, 0x00, 0, Buffer, 2);
	
	/* �����Ӵӻ���ȡ���� */
	SDIO_IO_BlockRead(1, 0x00, 0, Buffer, 512);
	
	SDIO_IO_MultiBlockRead(1, 0x00, 0, Buffer, 2);
	
 	while(1==1)
 	{
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

