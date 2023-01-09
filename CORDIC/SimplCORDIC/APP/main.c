#include "SWM341.h"


void SerialInit(void);

int main(void)
{
	uint32_t input;
	
	SystemInit();
	
	SerialInit();
   	
	CORDIC_Init(CORDIC);
	
	input = CORDIC_PI/6*16384;
	if((input > 163) && (input < 25559))		//只能计算0.01~1.56之间的弧度
	{
		CORDIC_Sin(input);
		while(CORDIC_Sin_IsDone() == 0);
		printf("sin(30) = %f\r\n", CORDIC_Sin_Result()/16384.0);
	}
	
	input = CORDIC_PI/3*16384;
	if((input > 163) && (input < 25559))		//只能计算0.01~1.56之间的弧度
	{
		CORDIC_Cos(input);
		while(CORDIC_Cos_IsDone() == 0);
		printf("cos(60) = %f\r\n", CORDIC_Cos_Result()/16384.0);
	}
	
	input = 1*16384;
	if((input > 819) && (input < 10000*16384))	//只能计算0.05~10000之间的值
	{
		CORDIC_Arctan(input);
		while(CORDIC_Arctan_IsDone() == 0);
		printf("arctan(1) = pi/%d\r\n", (int)(CORDIC_PI/(CORDIC_Arctan_Result()/16384.0)));
	}
	
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
