#include "SWM341.h"

#include <string.h>

void SerialInit(void);

int main(void)
{
	uint32_t i, res;
	char data[] = "Hi from SynWit";
	CRC_InitStructure CRC_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	CRC_initStruct.Poly = CRC_POLY_104C11DB7;
	CRC_initStruct.init_crc = 0;
	CRC_initStruct.in_width = CRC_WIDTH_8;
	CRC_initStruct.in_rev = CRC_REV_NOT;
	CRC_initStruct.in_not = false;
	CRC_initStruct.out_rev = CRC_REV_NOT;
	CRC_initStruct.out_not = false;
	
	CRC_Init(CRC, &CRC_initStruct);
	
	for(i = 0; i < strlen("Hi from SynWit"); i++)
		CRC_Write(data[i]);
	
	res = CRC_Result();
	
	if(res == 0xC4D9CB63)
		printf("CRC Result = 0x%08X, Pass!\r\n", res);
	else
		printf("CRC Result = 0x%08X, Fail!\r\n", res);
   	
	/* 重算一遍 */
	CRC_SetInitVal(CRC, 0);
	
	for(i = 0; i < strlen("Hi from SynWit"); i++)
		CRC_Write(data[i]);
	
	res = CRC_Result();
	
	if(res == 0xC4D9CB63)
		printf("CRC Result = 0x%08X, Pass!\r\n", res);
	else
		printf("CRC Result = 0x%08X, Fail!\r\n", res);
	
	while(1==1)
	{
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
