#include <string.h>
#include "SWM341.h"

#include "GD25Q21.h"

#define EEPROM_ADDR	  0x008000

uint8_t WrBuff[20] = {0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D,
				      0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};
uint8_t RdBuff[21] = {0};

void SerialInit(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	GD25Q21_Init();
	
	GD25Q21_EraseSector(EEPROM_ADDR);
 	while(GD25Q21_IsBusy()) __NOP();	// Wait for the erase operation to complete
	
	GD25Q21_ReadData(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nAfter Erase: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);
	
	GD25Q21_WritePage(EEPROM_ADDR, WrBuff, 20);
 	while(GD25Q21_IsBusy()) __NOP();	// Wait for the write operation to complete
	
	GD25Q21_ReadData(EEPROM_ADDR, RdBuff, 20);
	
	printf("\r\nAfter Write: \r\n");
	for(i = 0; i < 20; i++) printf("0x%02X, ", RdBuff[i]);
	
	while(1==1)
	{
	}
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
