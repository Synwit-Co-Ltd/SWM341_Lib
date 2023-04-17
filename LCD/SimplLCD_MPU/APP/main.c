#include "SWM341.h"

#include "ugui.h"
#include "NT35510.h"


uint16_t Buffer[NT35510_HPIX * 10] = {0};


UG_GUI gui;

void SerialInit(void);
void MPULCDInit(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	MPULCDInit();
	
	NT35510_Init();
	
	NT35510_Clear(C_RED);
	
	UG_Init(&gui,(void(*)(UG_S16,UG_S16,UG_COLOR))NT35510_DrawPoint, 480, 800);
	
	UG_DrawLine(0, 68, 480, 68, C_GREEN);
	UG_DrawLine(0, 136, 480, 136, C_GREEN);
	UG_DrawLine(0, 204, 480, 204, C_GREEN);
	
 	UG_FillFrame(120, 69, 360, 135, C_BLACK);
 	
  	UG_FontSelect(&FONT_12X20);
	UG_PutString(120, 80, "Hi from Synwit");
	
	/* MPU DMA 写入与中断演示 */
	for(int i = 0; i < sizeof(Buffer) / 2; i++)
		Buffer[i] = 0xFF00;
	
	GPIO_INIT(GPION, PIN5, GPIO_OUTPUT);
	GPIO_ClrBit(GPION, PIN5);
	
	NVIC_EnableIRQ(LCD_IRQn);
	
	while(1==1)
	{
		GPIO_SetBit(GPION, PIN5);
		NT35510_DMAWrite((uint32_t *)Buffer, 0, sizeof(Buffer) / 2 / NT35510_HPIX);
		
		/* 注意：由于写 LCD->MPUIR、LCD->MPUDR 启动的传输也会产生 LCD 中断，
				 因此只在启动 DMA 传输后使能中断，并在中断响应后关闭中断使能 */
		LCD_INTClr(LCD);
		LCD_INTEn(LCD);
		
		for(int i = 0; i < SystemCoreClock / 100; i++) __NOP();
	}
}


void LCD_Handler(void)
{
	LCD_INTDis(LCD);
	
	GPIO_InvBit(GPION, PIN5);
}


void MPULCDInit(void)
{
	uint32_t i;
	MPULCD_InitStructure MPULCD_initStruct;
	
	GPIO_Init(GPIOA, PIN6, 1, 0, 0, 0);		//屏幕背光
	GPIO_SetBit(GPIOA, PIN6);
	GPIO_Init(GPIOC, PIN6, 1, 0, 0, 0);		//屏幕复位
	GPIO_ClrBit(GPIOC, PIN6);
	for(i = 0; i < 1000000; i++) __NOP();
	GPIO_SetBit(GPIOC, PIN6);
	
//	PORT_Init(PORTB, PIN1,  PORTB_PIN1_LCD_B0,  0);
//	PORT_Init(PORTB, PIN11, PORTB_PIN11_LCD_B1, 0);
//	PORT_Init(PORTB, PIN13, PORTB_PIN13_LCD_B2, 0);
	PORT_Init(PORTB, PIN15, PORTB_PIN15_LCD_B3, 1);		//MPU_D0
	PORT_Init(PORTA, PIN2,  PORTA_PIN2_LCD_B4,  1);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_LCD_B5,  1);
	PORT_Init(PORTA, PIN10, PORTA_PIN10_LCD_B6, 1);
	PORT_Init(PORTA, PIN11, PORTA_PIN11_LCD_B7, 1);
//	PORT_Init(PORTA, PIN12, PORTA_PIN12_LCD_G0, 0);
//	PORT_Init(PORTA, PIN13, PORTA_PIN13_LCD_G1, 0);
	PORT_Init(PORTA, PIN14, PORTA_PIN14_LCD_G2, 1);
	PORT_Init(PORTA, PIN15, PORTA_PIN15_LCD_G3, 1);
	PORT_Init(PORTC, PIN0,  PORTC_PIN0_LCD_G4,  1);
	PORT_Init(PORTC, PIN1,  PORTC_PIN1_LCD_G5,  1);
	PORT_Init(PORTC, PIN2,  PORTC_PIN2_LCD_G6,  1);
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_LCD_G7,  1);
//	PORT_Init(PORTC, PIN4,  PORTC_PIN4_LCD_R0,  0);
//	PORT_Init(PORTC, PIN5,  PORTC_PIN5_LCD_R1,  0);
//	PORT_Init(PORTC, PIN8,  PORTC_PIN8_LCD_R2,  0);
	PORT_Init(PORTC, PIN9,  PORTC_PIN9_LCD_R3,  1);
	PORT_Init(PORTC, PIN10, PORTC_PIN10_LCD_R4, 1);
	PORT_Init(PORTC, PIN11, PORTC_PIN11_LCD_R5, 1);
	PORT_Init(PORTC, PIN12, PORTC_PIN12_LCD_R6, 1);
	PORT_Init(PORTC, PIN13, PORTC_PIN13_LCD_R7, 1);		//MPU_D15
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_LCD_CS,  0);
	PORT_Init(PORTB, PIN3,  PORTB_PIN3_LCD_WR,  0);
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_LCD_RS,  0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_LCD_RD,  0);
	
	MPULCD_initStruct.RDHoldTime = 32;
	MPULCD_initStruct.WRHoldTime = 16;
	MPULCD_initStruct.CSFall_WRFall = 4;
	MPULCD_initStruct.WRRise_CSRise = 4;
	MPULCD_initStruct.RDCSRise_Fall = 32;
	MPULCD_initStruct.WRCSRise_Fall = 16;
	MPULCD_Init(LCD, &MPULCD_initStruct);
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
