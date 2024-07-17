#include <string.h>
#include "SWM341.h"
#include "usbh_core.h"
#include "usbh_hid_core.h"
#include "usbh_hid_keybd.h"


/* USB Host driving keyboard demo (Full Speed keyboard only) */


extern USBH_Class_cb_t USBH_HID_cb;
extern USBH_User_cb_t  USBH_USR_cb;


void SerialInit(void);


int main(void)
{	
	SystemInit();
	
	SerialInit();

	USBH_Init(&USBH_HID_cb, &USBH_USR_cb);
	
	SysTick_Config(CyclesPerUs * 100);
	
	while(1==1)
	{
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
