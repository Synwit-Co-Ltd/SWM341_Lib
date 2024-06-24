#include "SWM341.h"


void SerialInit(void);
void SFC_Config(uint8_t width);
void SDRAM_Config(void);
void led_init(void);
void led_switch(int on);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	SFC_Config(4);
	
	SDRAM_Config();
	
	SFC_Read(0, (uint32_t *)SDRAMM_BASE, 0x100000/4);        // 将 SPI Flash 上前 1MB 拷贝到 SDRAM 上
	
	led_init();
	
	while(1==1)
	{
		led_switch(1);
		
		for(int i = 0; i < SystemCoreClock/8; i++) __NOP();
		
		led_switch(0);
		
		for(int i = 0; i < SystemCoreClock/8; i++) __NOP();
	}
}


void SFC_Config(uint8_t width)
{
	SFC_InitStructure SFC_initStruct;
	
	/* SFC使用专用的FSPI（Flash SPI）接口连接SPI Flash */
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


void SDRAM_Config(void)
{
	SDRAM_InitStructure SDRAM_InitStruct;
	
	PORT_Init(PORTM, PIN13, PORTM_PIN13_SDR_CLK, 0);
	PORT_Init(PORTM, PIN14, PORTM_PIN14_SDR_CKE, 0);
	PORT_Init(PORTB, PIN7,  PORTB_PIN7_SDR_WE,   0);
	PORT_Init(PORTB, PIN8,  PORTB_PIN8_SDR_CAS,  0);
	PORT_Init(PORTB, PIN9,  PORTB_PIN9_SDR_RAS,  0);
	PORT_Init(PORTB, PIN10, PORTB_PIN10_SDR_CS,  0);
	PORT_Init(PORTE, PIN15, PORTE_PIN15_SDR_BA0, 0);
	PORT_Init(PORTE, PIN14, PORTE_PIN14_SDR_BA1, 0);
	PORT_Init(PORTN, PIN14, PORTN_PIN14_SDR_A0,  0);
	PORT_Init(PORTN, PIN13, PORTN_PIN13_SDR_A1,  0);
	PORT_Init(PORTN, PIN12, PORTN_PIN12_SDR_A2,  0);
	PORT_Init(PORTN, PIN11, PORTN_PIN11_SDR_A3,  0);
	PORT_Init(PORTN, PIN10, PORTN_PIN10_SDR_A4,  0);
	PORT_Init(PORTN, PIN9,  PORTN_PIN9_SDR_A5,   0);
	PORT_Init(PORTN, PIN8,  PORTN_PIN8_SDR_A6,   0);
	PORT_Init(PORTN, PIN7,  PORTN_PIN7_SDR_A7,   0);
	PORT_Init(PORTN, PIN6,  PORTN_PIN6_SDR_A8,   0);
	PORT_Init(PORTN, PIN3,  PORTN_PIN3_SDR_A9,   0);
	PORT_Init(PORTN, PIN15, PORTN_PIN15_SDR_A10, 0);
	PORT_Init(PORTN, PIN2,  PORTN_PIN2_SDR_A11,  0);
	PORT_Init(PORTM, PIN15, PORTM_PIN15_SDR_A12, 0);
	PORT_Init(PORTE, PIN7,  PORTE_PIN7_SDR_D0,   1);
	PORT_Init(PORTE, PIN6,  PORTE_PIN6_SDR_D1,   1);
	PORT_Init(PORTE, PIN5,  PORTE_PIN5_SDR_D2,   1);
	PORT_Init(PORTE, PIN4,  PORTE_PIN4_SDR_D3,   1);
	PORT_Init(PORTE, PIN3,  PORTE_PIN3_SDR_D4,   1);
	PORT_Init(PORTE, PIN2,  PORTE_PIN2_SDR_D5,   1);
	PORT_Init(PORTE, PIN1,  PORTE_PIN1_SDR_D6,   1);
	PORT_Init(PORTE, PIN0,  PORTE_PIN0_SDR_D7,   1);
	PORT_Init(PORTE, PIN8,  PORTE_PIN8_SDR_D8,   1);
	PORT_Init(PORTE, PIN9,  PORTE_PIN9_SDR_D9,   1);
	PORT_Init(PORTE, PIN10, PORTE_PIN10_SDR_D10, 1);
	PORT_Init(PORTE, PIN11, PORTE_PIN11_SDR_D11, 1);
	PORT_Init(PORTE, PIN12, PORTE_PIN12_SDR_D12, 1);
	PORT_Init(PORTE, PIN13, PORTE_PIN13_SDR_D13, 1);
	PORT_Init(PORTC, PIN14, PORTC_PIN14_SDR_D14, 1);
	PORT_Init(PORTC, PIN15, PORTC_PIN15_SDR_D15, 1);
	PORT_Init(PORTB, PIN6,  PORTB_PIN6_SDR_LDQM, 0);
	PORT_Init(PORTM, PIN12, PORTM_PIN12_SDR_UDQM,0);
	
	SDRAM_InitStruct.Size = SDRAM_SIZE_8MB;
	SDRAM_InitStruct.ClkDiv = SDRAM_CLKDIV_1;
	SDRAM_InitStruct.CASLatency = SDRAM_CASLATENCY_2;
	SDRAM_InitStruct.RefreshTime = 64;
	SDRAM_InitStruct.TimeTRP  = SDRAM_TRP_3;
	SDRAM_InitStruct.TimeTRCD = SDRAM_TRCD_3;
	SDRAM_InitStruct.TimeTRC  = SDRAM_TRC_15;
	SDRAM_Init(&SDRAM_InitStruct);
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);		//GPIOM.0配置为UART0输入引脚
 	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);		//GPIOM.1配置为UART0输出引脚
 	
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
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
