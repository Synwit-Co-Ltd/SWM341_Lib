#include "SWM341.h"


uint32_t WordBuffer[20] = {0x14141414, 0x15151515, 0x16161616, 0x17171717, 0x18181818, 0x19191919, 0x1A1A1A1A, 0x1B1B1B1B, 0x1C1C1C1C, 0x1D1D1D1D,
				           0x1E1E1E1E, 0x1F1F1F1F, 0x20202020, 0x21212121, 0x22222222, 0x23232323, 0x24242424, 0x25252525, 0x26262626, 0x27272727};

uint16_t HalfBuffer[20] = {0x1414,     0x1515,     0x1616,     0x1717,     0x1818,     0x1919,     0x1A1A,     0x1B1B,     0x1C1C,     0x1D1D,    
				           0x1E1E,     0x1F1F,     0x2020,     0x2121,     0x2222,     0x2323,     0x2424,     0x2525,     0x2626,     0x2727};

uint8_t  ByteBuffer[20] = {0x14,       0x15,       0x16,       0x17,       0x18,       0x19,       0x1A,       0x1B,       0x1C,       0x1D,      
				           0x1E,       0x1F,       0x20,       0x21,       0x22,       0x23,       0x24,       0x25,       0x26,       0x27};

void WordTest(uint32_t addr, uint32_t *buff, uint32_t size);
void HalfTest(uint32_t addr, uint16_t *buff, uint32_t size);
void ByteTest(uint32_t addr, uint8_t  *buff, uint32_t size);
void SerialInit(void);

int main(void)
{
	uint32_t i, val;
	SDRAM_InitStructure SDRAM_InitStruct;
	
	SystemInit();
	
	SerialInit();
	
#if 1
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
#else
	PORTB->FUNC0 &= ~0xFF000000;
	PORTB->FUNC0 |=  0x22000000;
	PORTB->FUNC1 &= ~0x00000FFF;
	PORTB->FUNC1 |=  0x00000444;
	
	PORTC->FUNC1 &= ~0xFF000000;
	PORTC->FUNC1 |=  0x11000000;
	PORTC->INEN  |= (1 << 15) | (1 << 14);
	
	PORTE->FUNC0 = 0x11111111;
	PORTE->FUNC1 = 0x11111111;
	PORTE->INEN  |= 0x3FFF;
	
	PORTM->FUNC1 &= ~0xFFFF0000;
	PORTM->FUNC1 |=  0x33220000;
	
	PORTN->FUNC0 &= ~0xFF00FF00;
	PORTN->FUNC0 |=  0x11002100;
	PORTN->FUNC1 = 0x11111111;
#endif
	
	SDRAM_InitStruct.Size = SDRAM_SIZE_8MB;
	SDRAM_InitStruct.ClkDiv = SDRAM_CLKDIV_1;
	SDRAM_InitStruct.CASLatency = SDRAM_CASLATENCY_2;
	SDRAM_InitStruct.TimeTRP  = SDRAM_TRP_2;
	SDRAM_InitStruct.TimeTRCD = SDRAM_TRCD_2;
	SDRAM_InitStruct.TimeTRFC  = SDRAM_TRFC_7;
	SDRAM_Init(&SDRAM_InitStruct);
	

	WordTest(SDRAMM_BASE, WordBuffer, sizeof(WordBuffer)/4);
	WordTest(SDRAMM_BASE+0x100000, WordBuffer, sizeof(WordBuffer)/4);
	WordTest(SDRAMM_BASE+0x110001, WordBuffer, sizeof(WordBuffer)/4);
	
	HalfTest(SDRAMM_BASE, HalfBuffer, sizeof(HalfBuffer)/2);
	HalfTest(SDRAMM_BASE+0x200000, HalfBuffer, sizeof(HalfBuffer)/2);
	HalfTest(SDRAMM_BASE+0x210000+1, HalfBuffer, sizeof(HalfBuffer)/2);
	
	ByteTest(SDRAMM_BASE, ByteBuffer, sizeof(ByteBuffer)/1);
	ByteTest(SDRAMM_BASE+0x3210, ByteBuffer, sizeof(ByteBuffer)/1);
	ByteTest(SDRAMM_BASE+0x1235, ByteBuffer, sizeof(ByteBuffer)/1);
	

#if 1  // SDRAM Chip Test
#define SDRAM_SIZE  (0x100000 * 2)

	/* Word Test */
	for(i = 0; i < SDRAM_SIZE; i += 4)
	{
		*((volatile uint32_t *)(SDRAMM_BASE + i)) = i;
	}
	
	for(i = 0; i < SDRAM_SIZE; i += 4)
	{
		val = *((volatile uint32_t *)(SDRAMM_BASE + i));
		if(val != i)
		{
			printf("Word Test Error: 0x%08X expected, 0x%08X get\r\n", i, val);
			while(1);
		}
	}
	printf("Word Test Pass!\r\n");
	
	/* Half Test */
	for(i = 0; i < SDRAM_SIZE; i += 2)
	{
		*((volatile uint16_t *)(SDRAMM_BASE + i)) = i&0xFFFF;
	}
	
	for(i = 0; i < SDRAM_SIZE; i += 2)
	{
		val = *((volatile uint16_t *)(SDRAMM_BASE + i));
		if(val != (i&0xFFFF))
		{
			printf("Half Test Error: 0x%04X expected, 0x%04X get\r\n", (i&0xFFFF), val);
			while(1);
		}
	}
	printf("Half Test Pass!\r\n");
	
	/* Byte Test */
	for(i = 0; i < SDRAM_SIZE; i += 1)
	{
		*((volatile uint8_t *)(SDRAMM_BASE + i)) = i&0xFF;
	}
	
	for(i = 0; i < SDRAM_SIZE; i += 1)
	{
		val = *((volatile uint8_t *)(SDRAMM_BASE + i));
		if(val != (i&0xFF))
		{
			printf("Byte Test Error: 0x%02X expected, 0x%02X get\r\n", (i&0xFF), val);
			while(1);
		}
	}
	printf("Byte Test Pass!\r\n");
#endif	
	
	while(1==1)
	{
	}
}

void WordTest(uint32_t addr, uint32_t *buff, uint32_t size)
{
	uint32_t i;
#define SDRW  ((volatile uint32_t *)addr)
	
	printf("RW @ 0x%08X\r\n", addr);

	for(i = 0; i < size; i++)  SDRW[i] = 0x00000000;
	
	printf("\r\nAfter Erase: \r\n");
	for(i = 0; i < size; i++)  printf("0x%08X, ",  SDRW[i]);
	
	for(i = 0; i < size; i++)  SDRW[i] = buff[i];
	
	printf("\r\nAfter Write: \r\n");
	for(i = 0; i < size; i++)  printf("0x%08X, ",  SDRW[i]);
	
	printf("\r\n\r\n\r\n");
}

void HalfTest(uint32_t addr, uint16_t *buff, uint32_t size)
{
	uint32_t i;
#define SDRH  ((volatile uint16_t *)addr)
	
	printf("RW @ 0x%08X\r\n", addr);

	for(i = 0; i < size; i++)  SDRH[i] = 0x0000;
	
	printf("\r\nAfter Erase: \r\n");
	for(i = 0; i < size; i++)  printf("0x%04X, ",  SDRH[i]);
	
	for(i = 0; i < size; i++)  SDRH[i] = buff[i];
	
	printf("\r\nAfter Write: \r\n");
	for(i = 0; i < size; i++)  printf("0x%04X, ",  SDRH[i]);
	
	printf("\r\n\r\n\r\n");
}

void ByteTest(uint32_t addr, uint8_t  *buff, uint32_t size)
{
	uint32_t i;
#define SDRB  ((volatile uint8_t *)addr)
	
	printf("RW @ 0x%08X\r\n", addr);

	for(i = 0; i < size; i++)  SDRB[i] = 0x00;
	
	printf("\r\nAfter Erase: \r\n");
	for(i = 0; i < size; i++)  printf("0x%02X, ",  SDRB[i]);
	
	for(i = 0; i < size; i++)  SDRB[i] = buff[i];
	
	printf("\r\nAfter Write: \r\n");
	for(i = 0; i < size; i++)  printf("0x%02X, ",  SDRB[i]);
	
	printf("\r\n\r\n\r\n");
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
