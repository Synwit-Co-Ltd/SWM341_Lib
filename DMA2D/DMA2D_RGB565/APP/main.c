#include "SWM341.h"


#define LCD_HDOT	480		// 水平点数
#define LCD_VDOT	272		// 垂直点数
#define LCD_DIRH	1		// 水平显示？


uint16_t *LCD_Buffer = (uint16_t *)SDRAMM_BASE;
uint32_t *Img_Buffer = (uint32_t *)(SDRAMM_BASE + 0x100000);

DMA2D_LayerSetting fgLayer, bgLayer, outLayer;

extern const unsigned char gImage_Synwit128[32768];


void SerialInit(void);
void MemoryInit(void);
void RGBLCDInit(void);
void test_PixelFill(void);
void test_PixelMove(void);
void test_PixelBlend(void);

int main(void)
{
	DMA2D_InitStructure DMA2D_initStruct;
	
	SystemInit();
	
	SerialInit();
	
	MemoryInit();
	
	RGBLCDInit();
	
	LCD_Start(LCD);
	
	DMA2D_initStruct.Interval = CyclesPerUs;
	DMA2D_initStruct.IntEOTEn = 1;
	DMA2D_Init(&DMA2D_initStruct);
	
	test_PixelFill();
	
	test_PixelMove();
	
	test_PixelBlend();
	
	while(1==1)
	{
	}
}


void DMA2D_Handler(void)
{	
	DMA2D_INTClr();
}


void test_PixelFill(void)
{
	uint32_t i;
	
	/* 全屏填充蓝色 */
	outLayer.Address = (uint32_t)LCD_Buffer;
	outLayer.LineCount = LCD_VDOT;
	outLayer.LinePixel = LCD_HDOT;
	outLayer.LineOffset = 0;
	outLayer.ColorMode = DMA2D_FMT_RGB565;
	DMA2D_PixelFill(&outLayer, 0x001F);
	
	while(DMA2D_IsBusy()) __NOP();
	
	for(i = 0; i < SystemCoreClock/8; i++) __NOP();
	
	/* 左上角绘制150*150绿色方块 */
	outLayer.Address = (uint32_t)LCD_Buffer;
	outLayer.LineCount = 150;
	outLayer.LinePixel = 150;
	outLayer.LineOffset = LCD_HDOT-150;
	outLayer.ColorMode = DMA2D_FMT_RGB565;
	DMA2D_PixelFill(&outLayer, 0x07E0);
	
	while(DMA2D_IsBusy()) __NOP();
	
	for(i = 0; i < SystemCoreClock/8; i++) __NOP();
	
	/* 右下角绘制150*150红色方块 */
	outLayer.Address = (uint32_t)LCD_Buffer + (LCD_HDOT * (LCD_VDOT - 150) + (LCD_HDOT - 150)) * 2;
	outLayer.LineCount = 150;
	outLayer.LinePixel = 150;
	outLayer.LineOffset = LCD_HDOT-150;
	outLayer.ColorMode = DMA2D_FMT_RGB565;
	DMA2D_PixelFill(&outLayer, 0xF800);
	
	while(DMA2D_IsBusy()) __NOP();
	
	for(i = 0; i < SystemCoreClock/8; i++) __NOP();
}


void test_PixelMove(void)
{
	uint32_t i;
	
	fgLayer.Address = (uint32_t)gImage_Synwit128;
	fgLayer.LineOffset = 0;
	fgLayer.ColorMode = DMA2D_FMT_RGB565;
	
	/* 左上角绘制128*128图片 */
	outLayer.Address = (uint32_t)LCD_Buffer;
	outLayer.LineCount = 128;
	outLayer.LinePixel = 128;
	outLayer.LineOffset = LCD_HDOT-128;
	DMA2D_PixelMove(&fgLayer, &outLayer);
	
	while(DMA2D_IsBusy()) __NOP();
	
	for(i = 0; i < SystemCoreClock/8; i++) __NOP();
	
	/* 右下角绘制128*128图片 */
	outLayer.Address = (uint32_t)LCD_Buffer + (LCD_HDOT * (LCD_VDOT - 128) + (LCD_HDOT - 128)) * 2;
	outLayer.LineCount = 128;
	outLayer.LinePixel = 128;
	outLayer.LineOffset = LCD_HDOT-128;
	DMA2D_PixelMove(&fgLayer, &outLayer);
	
	while(DMA2D_IsBusy()) __NOP();
	
	for(i = 0; i < SystemCoreClock/8; i++) __NOP();
}


void test_PixelBlend(void)
{
	/* 将图片由 RGB565 转换成 ARGB888 格式 */
	fgLayer.Address = (uint32_t)gImage_Synwit128;
	fgLayer.LineOffset = 0;
	fgLayer.ColorMode = DMA2D_FMT_RGB565;
	
	outLayer.Address = (uint32_t)Img_Buffer;
	outLayer.LineCount = 128;
	outLayer.LinePixel = 128;
	outLayer.LineOffset = 0;
	outLayer.ColorMode = DMA2D_FMT_ARGB888;
	DMA2D_PixelConvert(&fgLayer, &outLayer);
	
	while(DMA2D_IsBusy()) __NOP();
	
	/* 将图片中的白色像素点的 Alpha 值设置为 0，这样后面混合时就不显示图片的白色，而是显示背景色了
	   注意：这步可在电脑上通过图像处理软件完成，然后将处理后的图片按照 ARGB888 格式转换成数组存入程序中使用
	*/
	for(int i = 0; i < 128; i++)
		for(int j = 0; j < 128; j++)
			if(Img_Buffer[i*128+j] == 0xFFFFFFFF)
				Img_Buffer[i*128+j] = 0x00FFFFFF;
	
	/* 执行图片混合 */
	fgLayer.Address = (uint32_t)Img_Buffer;
	fgLayer.LineOffset = 0;
	fgLayer.ColorMode = DMA2D_FMT_ARGB888;
	fgLayer.AlphaMode = DMA2D_AMODE_PIXEL;
	
	bgLayer.Address = (uint32_t)LCD_Buffer + (LCD_HDOT * (LCD_VDOT - 128)/2 + (LCD_HDOT - 128)/2) * 2;;
	bgLayer.LineOffset = LCD_HDOT-128;
	bgLayer.ColorMode = DMA2D_FMT_RGB565;
	bgLayer.AlphaMode = DMA2D_AMODE_PIXEL;
	
	outLayer.Address = (uint32_t)LCD_Buffer + (LCD_HDOT * (LCD_VDOT - 128)/2 + (LCD_HDOT - 128)/2) * 2;
	outLayer.LineCount = 128;
	outLayer.LinePixel = 128;
	outLayer.LineOffset = LCD_HDOT-128;
	outLayer.ColorMode = DMA2D_FMT_RGB565;
	DMA2D_PixelBlend(&fgLayer, &bgLayer, &outLayer);
	
	while(DMA2D_IsBusy()) __NOP();
}


void RGBLCDInit(void)
{
	uint32_t i;
	LCD_InitStructure LCD_initStruct;
	
	GPIO_Init(GPIOA, PIN6, 1, 0, 0, 0);		//屏幕背光
	GPIO_SetBit(GPIOA, PIN6);
	GPIO_Init(GPIOC, PIN6, 1, 0, 0, 0);		//屏幕复位
	GPIO_ClrBit(GPIOC, PIN6);
	for(i = 0; i < CyclesPerUs*1000; i++) __NOP();
	GPIO_SetBit(GPIOC, PIN6);
	
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_LCD_B0,  0);
	PORT_Init(PORTB, PIN11, PORTB_PIN11_LCD_B1, 0);
	PORT_Init(PORTB, PIN13, PORTB_PIN13_LCD_B2, 0);
	PORT_Init(PORTB, PIN15, PORTB_PIN15_LCD_B3, 0);
	PORT_Init(PORTA, PIN2,  PORTA_PIN2_LCD_B4,  0);
	PORT_Init(PORTA, PIN9,  PORTA_PIN9_LCD_B5,  0);
	PORT_Init(PORTA, PIN10, PORTA_PIN10_LCD_B6, 0);
	PORT_Init(PORTA, PIN11, PORTA_PIN11_LCD_B7, 0);
	PORT_Init(PORTA, PIN12, PORTA_PIN12_LCD_G0, 0);
	PORT_Init(PORTA, PIN13, PORTA_PIN13_LCD_G1, 0);
	PORT_Init(PORTA, PIN14, PORTA_PIN14_LCD_G2, 0);
	PORT_Init(PORTA, PIN15, PORTA_PIN15_LCD_G3, 0);
	PORT_Init(PORTC, PIN0,  PORTC_PIN0_LCD_G4,  0);
	PORT_Init(PORTC, PIN1,  PORTC_PIN1_LCD_G5,  0);
	PORT_Init(PORTC, PIN2,  PORTC_PIN2_LCD_G6,  0);
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_LCD_G7,  0);
	PORT_Init(PORTC, PIN4,  PORTC_PIN4_LCD_R0,  0);
	PORT_Init(PORTC, PIN5,  PORTC_PIN5_LCD_R1,  0);
	PORT_Init(PORTC, PIN8,  PORTC_PIN8_LCD_R2,  0);
	PORT_Init(PORTC, PIN9,  PORTC_PIN9_LCD_R3,  0);
	PORT_Init(PORTC, PIN10, PORTC_PIN10_LCD_R4, 0);
	PORT_Init(PORTC, PIN11, PORTC_PIN11_LCD_R5, 0);
	PORT_Init(PORTC, PIN12, PORTC_PIN12_LCD_R6, 0);
	PORT_Init(PORTC, PIN13, PORTC_PIN13_LCD_R7, 0);
	PORT_Init(PORTB, PIN2,  PORTB_PIN2_LCD_VSYNC, 0);
	PORT_Init(PORTB, PIN3,  PORTB_PIN3_LCD_HSYNC, 0);
	PORT_Init(PORTB, PIN4,  PORTB_PIN4_LCD_DEN,   0);
	PORT_Init(PORTB, PIN5,  PORTB_PIN5_LCD_DCLK,  0);
	
	LCD_initStruct.ClkDiv = 8;
	LCD_initStruct.Format = LCD_FMT_RGB565;
	LCD_initStruct.HnPixel = LCD_HDOT;
	LCD_initStruct.VnPixel = LCD_VDOT;
	LCD_initStruct.Hfp = 5;
	LCD_initStruct.Hbp = 40;
	LCD_initStruct.Vfp = 8;
	LCD_initStruct.Vbp = 8;
	LCD_initStruct.HsyncWidth = 5;
	LCD_initStruct.VsyncWidth = 5;
	LCD_initStruct.DataSource = (uint32_t)LCD_Buffer;
	LCD_initStruct.Background = 0xFFFF;
	LCD_initStruct.SampleEdge = LCD_SAMPLE_FALL;	// ATK-4342 RGBLCD 下降沿采样
	LCD_initStruct.IntEOTEn = 1;
	LCD_Init(LCD, &LCD_initStruct);
}

void LCD_Handler(void)
{
	LCD_INTClr(LCD);
	
	LCD_Start(LCD);
}


void MemoryInit(void)
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
	SDRAM_InitStruct.ClkDiv = SDRAM_CLKDIV_2;
	SDRAM_InitStruct.CASLatency = SDRAM_CASLATENCY_2;
	SDRAM_InitStruct.TimeTRP  = SDRAM_TRP_3;
	SDRAM_InitStruct.TimeTRCD = SDRAM_TRCD_3;
	SDRAM_InitStruct.TimeTRFC  = SDRAM_TRFC_9;
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
