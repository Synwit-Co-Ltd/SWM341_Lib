#include "SWM341.h"

void SerialInit(void);

int main(void)
{
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	SYS->HRCCR |= (1 << SYS_HRCCR_ON_Pos);
	SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);
	SYS->PRNGCR = (0 << SYS_PRNGCR_CLR_Pos) |
				  (3 << SYS_PRNGCR_MODE_Pos);
	while(1==1)
	{
		while((SYS->PRNGCR & SYS_PRNGCR_RDY_Msk) == 0) __NOP();
		printf("PRNGDL: %08X, PRNGDH: %08X\n", SYS->PRNGDL, SYS->PRNGDH);
		
		for(i = 0; i < SystemCoreClock/8; i++) __NOP();
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
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThresholdIEn = 0;
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
