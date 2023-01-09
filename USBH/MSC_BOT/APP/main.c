#include <string.h>
#include "SWM341.h"
#include "usbh_core.h"
#include "usbh_msc_core.h"

#include "ff.h"

extern USBH_Class_cb_t USBH_MSC_cb;
extern USBH_User_cb_t  USBH_USR_cb;

FATFS fatfs;

char str[] = "Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n"
			 "Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n"
			 "Hi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\nHi from Synwit!\n";
char buf[1024] = {0};

void SerialInit(void);


int main(void)
{	
	uint32_t len;
	FRESULT  res;
	FIL filw, filr;
	
	SystemInit();
	
	SerialInit();
	
	USBH_Init(&USBH_MSC_cb, &USBH_USR_cb);
	
	SysTick_Config(CyclesPerUs * 100);
	
	while(USBH_MSC_Info.Ready == 0) __NOP();	// 等待 U 盘插入
	
once:
	res = f_mount(&fatfs, "usb:", 1);
	if(res == FR_OK)
	{
		printf("usbdisk init ok!\r\n");
	}
	else
	{
		printf("usbdisk init fail!\r\n");
		goto fail;
	}
	
    res = f_open(&filw, "usb:test.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(res == FR_OK)
	{
		printf("create file ok!\r\n");
	}
	else
	{
		printf("create file fail!\r\n");
		goto fail;
	}
	
	res = f_write(&filw, str, strlen(str), &len);
	if(res == FR_OK)
	{
		printf("write file ok!\r\n");
	}
	else
	{
		printf("write file fail!\r\n");
		goto fail;
	}
	
	f_close(&filw);
	
	res = f_open(&filr, "usb:test.txt", FA_OPEN_EXISTING | FA_READ);
	if(res == FR_OK)
	{
		printf("open file ok!\r\n");
	}
	else
	{
		printf("open file fail!\r\n");
		goto fail;
	}
	
	res = f_read(&filr, buf, sizeof(buf)-1, &len);
	if(res == FR_OK)
	{
		printf("read file ok!\r\n");
	}
	else
	{
		printf("read file fial!\r\n");
		goto fail;
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
	
	while(USBH_MSC_Info.Ready == 1) __NOP();	// 等待 U 盘拔下
	
fail:
	while(1==1)
	{
		if(USBH_MSC_Info.Ready == 1)	// 检测到 U 盘插入
			goto once;
	}
}


void SysTick_Handler(void)
{
	USBH_Process();
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
	
	while(UART_IsTXBusy(UART0)) __NOP();
 	
	return ch;
}
