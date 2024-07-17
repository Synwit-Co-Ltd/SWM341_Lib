#include <string.h>
#include "SWM341.h"
#include "usbh_core.h"
#include "usbh_cdc_core.h"

extern USBH_Class_cb_t USBH_CDC_cb;
extern USBH_User_cb_t  USBH_USR_cb;


char *strHi = "Hi from Synwit by USB Host CDC_ACM example!\r\n";
char rxBuff[128];


void SerialInit(void);

int main(void)
{	
	uint32_t n;
	
	SystemInit();
	
	SerialInit();
	
	USBH_Init(&USBH_CDC_cb, &USBH_USR_cb);
	
	SysTick_Config(CyclesPerUs * 100);
	
	while(1==1)
	{
#if 0
		do
		{
			n = USBH_CDC_Send((uint8_t *)strHi, strlen(strHi));
		} while(n == 0);
		
		for(uint32_t i = 0; i < SystemCoreClock/2; i++) __NOP();
#else
		memset(rxBuff, 0x00, 128);
		n = USBH_CDC_Read((uint8_t *)rxBuff, 128-1);
		if(n)
			printf("RX: %s\r\n", rxBuff);
#endif
	}
}


void SysTick_Handler(void)
{	
	USBH_Process();
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
	
	while(UART_IsTXBusy(UART0)) __NOP();
 	
	return ch;
}
