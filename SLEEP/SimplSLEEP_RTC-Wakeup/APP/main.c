#include "SWM341.h"


void SerialInit(void);
void RTC_Config(void);

int main(void)
{
	SystemInit();
	
	SerialInit();
	
	SYS->LRCCR = (1 << SYS_LRCCR_ON_Pos);			// Turn on 32KHz LRC oscillator
	
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);				// output, connect a LED
	
	RTC_Config();
	SYS->RTCWKCR |= (1 << SYS_RTCWKCR_EN_Pos);		// enable RTC wake-up
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN9);					// turn on the LED
		for(int i = 0; i < SystemCoreClock/4; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN9);					// turn off the LED
		
		__disable_irq();
		switchTo20MHz();							// Before sleep, switch to 20MHz
		
		SYS->RTCWKSR = SYS_RTCWKSR_FLAG_Msk;		// clear wake-up flag
		SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);	// enter sleep mode
		while((SYS->RTCWKSR & SYS_RTCWKSR_FLAG_Msk) == 0) __NOP();
		
		switchToPLL(1, 3, 60, PLL_OUT_DIV8, 0);		// After waking up, switch to PLL
		__enable_irq();
	}
}


RTC_DateTime dateTime;
RTC_AlarmStructure alarmStruct;

void RTC_Config(void)
{
	RTC_InitStructure RTC_initStruct;
	
	RTC_initStruct.clksrc = RTC_CLKSRC_LRC32K;
	RTC_initStruct.Year = 2018;
	RTC_initStruct.Month = 3;
	RTC_initStruct.Date = 23;
	RTC_initStruct.Hour = 10;
	RTC_initStruct.Minute = 5;
	RTC_initStruct.Second = 5;
	RTC_initStruct.SecondIEn = 0;
	RTC_initStruct.MinuteIEn = 0;
	RTC_Init(RTC, &RTC_initStruct);
	
	RTC_Start(RTC);
	
	alarmStruct.Days = RTC_SUN | RTC_MON | RTC_TUE | RTC_WED | RTC_THU | RTC_FRI | RTC_SAT;
	alarmStruct.Hour = 10;
	alarmStruct.Minute = 5;
	alarmStruct.Second = 10;
	alarmStruct.AlarmIEn = 1;
	
	RTC_AlarmSetup(RTC, &alarmStruct);
}

void RTC_Handler(void)
{
	if(RTC_INTStat(RTC, RTC_IT_ALARM))
	{
		RTC_INTClr(RTC, RTC_IT_ALARM);
		
		RTC_GetDateTime(RTC, &dateTime);
		printf("Now Time: %02d : %02d\r\n", dateTime.Minute, dateTime.Second);
		
		if(dateTime.Second >= 55)
		{
			alarmStruct.Second = 0;
			alarmStruct.Minute = dateTime.Minute + 1;
		}
		else
		{
			alarmStruct.Second = dateTime.Second + 5;
			alarmStruct.Minute = dateTime.Minute;
		}
		RTC_AlarmSetup(RTC, &alarmStruct);
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
