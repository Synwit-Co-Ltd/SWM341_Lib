#include "SWM341.h"

void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	for(int i = 0; i < SystemCoreClock/4; i++) __NOP();	//等待晶振稳定，防止上电时误识别晶振停振
   	
	SYS->XTALSR = SYS_XTALSR_STOP_Msk;					//清除标志
	NVIC_ClearPendingIRQ(XTALSTOP_IRQn);
	NVIC_EnableIRQ(XTALSTOP_IRQn);
   	
	while(1==1)
	{
		printf("SystemCoreClock = %d\r\n", SystemCoreClock);
		
		for(int i = 0; i < SystemCoreClock/4; i++) __NOP();
	}
}


void XTALSTOP_Handler(void)
{
	/* 若不执行 switchTo20MHz()，晶振恢复振荡时系统时钟会自动切换回外部晶振，
	   若外部晶振不稳定，在振荡和不振荡间来回切换，则系统时钟也会在内部时钟和外部时钟之间来回切换 */
	switchTo20MHz();
	SystemCoreClockUpdate();
	
	SYS->XTALCR = 0;							//外部晶振工作不正常，关闭
	NVIC_DisableIRQ(XTALSTOP_IRQn);
	
	UART_SetBaudrate(UART0, 57600);				//系统主频变化，需重新设定波特率
	printf("XTAL Stop Detected!\r\n");
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
