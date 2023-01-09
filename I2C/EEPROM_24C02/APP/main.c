#include "SWM341.h"


#define SLV_ADDR  0x50

#define MEM_ADDR  0x10

char txbuff[4] = {0x37, 0x55, 0xAA, 0x78};
char rxbuff[4] = {0};

void SerialInit(void);
void I2CMstInit(void);

int main(void)
{
	uint32_t i;
	uint8_t ack;
	
	SystemInit();
	
	SerialInit();
	
	I2CMstInit();
	
	while(1==1)
	{
		/*************** EEPROM Write ***************/		
		ack = I2C_Start(I2C0, (SLV_ADDR << 1) | 0, 1);
		if(ack == 0)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		ack = I2C_Write(I2C0, MEM_ADDR, 1);
		if(ack == 0)
		{
			printf("Slave send NACK for memory address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < 4; i++)
		{
			ack = I2C_Write(I2C0, txbuff[i], 1);
			if(ack == 0)
			{
				printf("Slave send NACK for data\r\n");
				goto nextloop;
			}
		}
		
		I2C_Stop(I2C0, 1);
		
		printf("Master Write %X %X %X %X @ %X\r\n", txbuff[0], txbuff[1], txbuff[2], txbuff[3], MEM_ADDR);


		for(i = 0; i < 1000000; i++) __NOP();	// 延时等待内部写入操作完成
		
		
		/*************** EEPROM Read ***************/
		ack = I2C_Start(I2C0, (SLV_ADDR << 1) | 0, 1);
		if(ack == 0)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		ack = I2C_Write(I2C0, MEM_ADDR, 1);
		if(ack == 0)
		{
			printf("Slave send NACK for memory address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < CyclesPerUs; i++) __NOP();	// 不加此延时，系统主频高于 100MHz 时，re-start 发不出来
		
		ack = I2C_Start(I2C0, (SLV_ADDR << 1) | 1, 1);
		if(ack == 0)
		{
			printf("Slave send NACK for address\r\n");
			goto nextloop;
		}
		
		for(i = 0; i < 3; i++)
		{
			rxbuff[i] = I2C_Read(I2C0, 1, 1);
		}
		rxbuff[i] = I2C_Read(I2C0, 0, 1);
		
		printf("Master Read %X %X %X %X @ %X\r\n", rxbuff[0], rxbuff[1], rxbuff[2], rxbuff[3], MEM_ADDR);
		
		if((txbuff[0] == rxbuff[0]) && (txbuff[1] == rxbuff[1]) && (txbuff[2] == rxbuff[2]) && (txbuff[3] == rxbuff[3]))
			printf("Success\r\n");
		else
			printf("Fail\r\n");

nextloop:
		I2C_Stop(I2C0, 1);
		for(i = 0; i < SystemCoreClock/3; i++) __NOP();
	}
}


void I2CMstInit(void)
{
	I2C_InitStructure I2C_initStruct;
	
	PORT_Init(PORTN, PIN5, PORTN_PIN5_I2C0_SCL, 1);
	PORTN->OPEND |= (1 << PIN5);	// 开漏
	PORTN->PULLU |= (1 << PIN5);	// 上拉
	PORT_Init(PORTN, PIN4, PORTN_PIN4_I2C0_SDA, 1);
	PORTN->OPEND |= (1 << PIN4);
	PORTN->PULLU |= (1 << PIN4);
	
	I2C_initStruct.Master = 1;
	I2C_initStruct.MstClk = 100000;
	I2C_initStruct.Addr10b = 0;
	I2C_initStruct.TXEmptyIEn = 0;
	I2C_initStruct.RXNotEmptyIEn = 0;
	I2C_Init(I2C0, &I2C_initStruct);
	
	I2C_Open(I2C0);
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
