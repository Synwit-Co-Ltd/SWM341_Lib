#include "SWM341.h"
#include "hid_dac.h"

uint16_t DAC_nPoint = 100;	// number of points
uint16_t DAC_Buffer[1024] = {
    2048,  2176,  2304,  2431,  2557,  2680,  2801,  2919,  3034,  3144,
    3251,  3352,  3449,  3540,  3625,  3704,  3776,  3841,  3900,  3951,
    3994,  4030,  4058,  4078,  4090,  4095,  4090,  4078,  4058,  4030,
    3994,  3951,  3900,  3841,  3776,  3704,  3625,  3540,  3449,  3352,
    3251,  3144,  3034,  2919,  2801,  2680,  2557,  2431,  2304,  2176,
    2047,  1919,  1791,  1664,  1538,  1415,  1294,  1176,  1061,   951,
     844,   743,   646,   555,   470,   391,   319,   254,   195,   144,
     101,    65,    37,    17,     5,     1,     5,    17,    37,    65,
     101,   144,   195,   254,   319,   391,   470,   555,   646,   743,
     844,   951,  1061,  1176,  1294,  1415,  1538,  1664,  1791,  1919,
};


void SerialInit(void);

int main(void)
{
	uint32_t i, j;
	
	SystemInit();

	SerialInit();
	
	PORT_Init(PORTD, PIN2, PORTD_PIN2_DAC_OUT, 0);
	DAC_Init(DAC, DAC_FORMAT_LSB12B);
	DAC_Open(DAC);
	
	HID_Init();
	
	USBD_Open();
	
	while(1==1)
	{
		for(i = 0; i < DAC_nPoint; i++)
		{
			DAC->DHR = DAC_Buffer[i];
			
			while(DAC->SR & DAC_SR_DHRFULL_Msk) __NOP();
			
			for(j = 0; j < CyclesPerUs*100; j++) __NOP();
		}
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
	
	while(UART_IsTXBusy(UART0)) __NOP();

	return ch;
}
