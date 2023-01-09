#include "SWM341.h"

/* 注意：
	1、进入STOP后只能通过Wake-up引脚唤醒，不能按复位按键复位芯片
	2、从STOP模式Wake-up后程序从头开始重新执行，但Backup区的数据可以保持
	3、Wake-up引脚是PD6、PD15，下降沿唤醒，休眠期间需要保持高电平
*/

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	SYS->LRCCR |= (1 << SYS_LRCCR_ON_Pos);			//开启32K低频时钟
	
	printf("BACKUP[0]: %08X\r\n", SYS->BACKUP[0]);	//注意：测试发现没有保持功能
	SYS->BACKUP[0] += 1;
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//输出， 接LED
	
	PORT_Init(PORTD, PIN6, PORTD_PIN6_WAKEUP, 1);
	PORTD->PULLU |= (1 << PIN6);
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//点亮LED
		for(i = 0; i < SystemCoreClock/4; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN5);					//熄灭LED
		
		SYS->SLEEP |= (1 << SYS_SLEEP_STOP_Pos);	//进入STOP模式
	
		for(i = 0; i < 5000; i++);
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
