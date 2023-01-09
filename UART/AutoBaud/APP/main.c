#include "SWM341.h"

void SerialInit(void);

int main(void)
{
	uint32_t res, i;
	UART_InitStructure UART_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	PORT_Init(PORTD, PIN3, PORTD_PIN3_UART1_TX, 0);	//GPIOD.3配置为UART1输出引脚
	PORT_Init(PORTC, PIN1, PORTC_PIN1_UART2_RX, 1);	//GPIOC.1配置为UART2输入引脚
	
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
	
	UART_initStruct.Baudrate = 57600;
 	UART_Init(UART1, &UART_initStruct);
	UART_Open(UART1);
	
	UART_initStruct.Baudrate = 9600;
 	UART_Init(UART2, &UART_initStruct);
	UART_Open(UART2);
   	
	/* 测试说明
	*  将UART1_TX脚连接到UART2_RX，UART1波特率57600，UART2波特率9600
	*  让UART1发送0xF8，UART2通过测量0xF8自动计算出波特率将自己的波特率设为57600
	****************************************************************************/
	while(1==1)
	{		
		UART_ABRStart(UART2, 0xF8);
		
		while((res = UART_ABRIsDone(UART2)) == 0)
		{
			UART_WriteByte(UART1, 0xF8);
			while(UART_IsTXBusy(UART1));
		}
		
		if(res == UART_ABR_RES_OK) printf("AutoBaud OK\r\n");
		else                       printf("AutoBaud Error\r\n");
		
		printf("UART1 Baud Reg: %d\r\n", UART_GetBaudrate(UART1));
		printf("UART2 Baud Reg: %d\r\n", UART_GetBaudrate(UART2));
		
		for(i = 0; i < SystemCoreClock/5; i++) __NOP();
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);	//GPIOM.0配置为UART0输入引脚
	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);	//GPIOM.1配置为UART0输出引脚
 	
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
* 函数名称: fputc()
* 功能说明: printf()使用此函数完成实际的串口打印动作
* 输    入: int ch		要打印的字符
*			FILE *f		文件句柄
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
