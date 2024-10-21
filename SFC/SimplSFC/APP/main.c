#include "SWM341.h"


/* Use Serial Flash Controller (SFC) to read and write SPI Flash */


#define EEPROM_ADDR	  0x10000

uint32_t WrBuff[20] = {0x14141414, 0x15151515, 0x16161616, 0x17171717, 0x18181818,
				       0x19191919, 0x1A1A1A1A, 0x1B1B1B1B, 0x1C1C1C1C, 0x1D1D1D1D,
				       0x1E1E1E1E, 0x1F1F1F1F, 0x20202020, 0x21212121, 0x22222222,
				       0x23232323, 0x24242424, 0x25252525, 0x26262626, 0x27272727};
uint32_t RdBuff[20] = {0};


void SerialInit(void);
void SFC_Config(uint8_t width);

int main(void)
{	
	uint32_t i;
	uint32_t id;
	
	SystemInit();
	
	SerialInit();

	SFC_Config(4);
	
	id = SFC_ReadJEDEC();
	
	printf("\r\nFlash JEDEC: %06X\r\n", id);

#if 1
	GPIO_SetBit(GPIOD, PIN6);
	GPIO_SetBit(GPIOD, PIN5);
	GPIO_INIT(GPIOD, PIN5, GPIO_OUTPUT);
	GPIO_INIT(GPIOD, PIN6, GPIO_OUTPUT);
	GPIO_INIT(GPIOD, PIN8, GPIO_OUTPUT);
	GPIO_INIT(GPIOD, PIN7, GPIO_INPUT);
	
	uint8_t uid[8] = {0};
	SFC_GPIOReadUID(uid, 8);
	printf("\r\nFlash UID: %02X%02X%02X%02X%02X%02X%02X%02X\r\n", uid[0], uid[1], uid[2], uid[3], uid[4], uid[5], uid[6], uid[7]);
	
	PORT_Init(PORTD, PIN5, PORTD_PIN5_FSPI_SCLK,  0);
	PORT_Init(PORTD, PIN6, PORTD_PIN6_FSPI_SSEL,  0);
	PORT_Init(PORTD, PIN8, PORTD_PIN8_FSPI_MOSI,  1);
	PORT_Init(PORTD, PIN7, PORTD_PIN7_FSPI_MISO,  1);
#endif
	
	printf("\r\nAfter Erase: \r\n");
	
	SFC_Erase(EEPROM_ADDR, 1);
	
	SFC_Read(EEPROM_ADDR, RdBuff, 20);
	for(i = 0; i < 20; i++) printf("0x%X, ", RdBuff[i]);
	
	printf("\r\nAfter Write: \r\n");

	/* SFC writes are slow, and it is recommended to use GPIO to simulate SPI writes when writing a large number of data */
#if 0
	SFC_Write(EEPROM_ADDR, WrBuff, 20);
#else
	GPIO_SetBit(GPIOD, PIN6);
	GPIO_SetBit(GPIOD, PIN5);
	GPIO_INIT(GPIOD, PIN5, GPIO_OUTPUT);
	GPIO_INIT(GPIOD, PIN6, GPIO_OUTPUT);
	GPIO_INIT(GPIOD, PIN8, GPIO_OUTPUT);
	GPIO_INIT(GPIOD, PIN7, GPIO_INPUT);
	
	SFC_GPIOWrite(EEPROM_ADDR, WrBuff, 20);
	
	PORT_Init(PORTD, PIN5, PORTD_PIN5_FSPI_SCLK,  0);
	PORT_Init(PORTD, PIN6, PORTD_PIN6_FSPI_SSEL,  0);
	PORT_Init(PORTD, PIN8, PORTD_PIN8_FSPI_MOSI,  1);
	PORT_Init(PORTD, PIN7, PORTD_PIN7_FSPI_MISO,  1);
#endif

	SFC_Read(EEPROM_ADDR, RdBuff, 20);
	for(i = 0; i < 20; i++) printf("0x%X, ", RdBuff[i]);
	
	while(1==1)
	{
	}
}


void SFC_Config(uint8_t width)
{
	SFC_InitStructure SFC_initStruct;
	
	/* SFC uses the dedicated Flash SPI (FSPI) interface to connect to SPI Flash */
	PORT_Init(PORTD, PIN5, PORTD_PIN5_FSPI_SCLK,  0);
	PORT_Init(PORTD, PIN6, PORTD_PIN6_FSPI_SSEL,  0);
	PORT_Init(PORTD, PIN8, PORTD_PIN8_FSPI_MOSI,  1);
	PORT_Init(PORTD, PIN7, PORTD_PIN7_FSPI_MISO,  1);
	PORT_Init(PORTD, PIN3, PORTD_PIN3_FSPI_DATA2, 1);
	PORT_Init(PORTD, PIN4, PORTD_PIN4_FSPI_DATA3, 1);
	
	switch(width)
	{
	case 1:
	default:
		SFC_initStruct.ClkDiv = SFC_CLKDIV_4;
		SFC_initStruct.Cmd_Read = 0x03;
		SFC_initStruct.Width_Read = SFC_RDWIDTH_1;
		SFC_initStruct.Cmd_PageProgram = 0x02;
		SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_1;
		break;
	
	case 2:
		SFC_initStruct.ClkDiv = SFC_CLKDIV_4;
		SFC_initStruct.Cmd_Read = 0xBB;
		SFC_initStruct.Width_Read = SFC_RDWIDTH_2;
		SFC_initStruct.Cmd_PageProgram = 0x02;
		SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_1;
		break;
	
	case 4:
		SFC_initStruct.ClkDiv = SFC_CLKDIV_4;
		SFC_initStruct.Cmd_Read = 0xEB;
		SFC_initStruct.Width_Read = SFC_RDWIDTH_4;
		SFC_initStruct.Cmd_PageProgram = 0x32;
		SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_4;
		break;
	}
	
	SFC_Init(&SFC_initStruct);
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);
 	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);
 	
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

int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
