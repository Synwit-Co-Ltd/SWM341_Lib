#include <string.h>
#include "SWM341.h"

#include "ff.h"

FATFS fatfs;

char str[] __attribute__((aligned(4))) = 
			"Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n"
			"Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n"
			"Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n"
			"Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n"
			"Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n"
			"Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n"
			"Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n";
char buf[1024] __attribute__((aligned(4))) = {0};

void SerialInit(void);

int main(void)
{
	uint32_t len;
	FRESULT  res;
	FIL filw, filr;
	
	SystemInit();
	
	SerialInit();
	
	for(int i = 0; i < SystemCoreClock/10; i++) __NOP();	// Wait for the SD card to complete initialization
	
	res = f_mount(&fatfs, "sd:", 1);
	if(res != FR_OK)
	{
		printf("sdcard init fail!\r\n");
		goto loop_forever;
	}
	
    res = f_open(&filw, "sd:test.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(res != FR_OK)
	{
		printf("create file fail!\r\n");
		goto loop_forever;
	}
	
	res = f_write(&filw, str, strlen(str), &len);
	if(res != FR_OK)
	{
		printf("write file fail!\r\n");
		goto loop_forever;
	}
	
	f_close(&filw);
	
	res = f_open(&filr, "sd:test.txt", FA_OPEN_EXISTING | FA_READ);
	if(res != FR_OK)
	{
		printf("open file fail!\r\n");
		goto loop_forever;
	}
	
	res = f_read(&filr, buf, sizeof(buf)-1, &len);
	if(res != FR_OK)
	{
		printf("read file fial!\r\n");
		goto loop_forever;
	}
	
	f_close(&filr);
	
	if(strcmp(buf, str) == 0)
	{
		printf("read string == write string\r\n");
	}
	else
	{
		printf("read string != write string\r\n");
	}
		
loop_forever:
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
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
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
