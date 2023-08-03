#include "SWM341.h"

void SerialInit(void);
void RTC_Config(void);

int main(void)
{	
	uint32_t i;
	
	SystemInit();
	
	SerialInit();
	
	SYS->LRCCR = (1 << SYS_LRCCR_ON_Pos);			//����32K��Ƶ����
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);				//����� ��LED
	
	RTC_Config();
	SYS->RTCWKCR |= (1 << SYS_RTCWKCR_EN_Pos);		//����RTC����
	
	while(1==1)
	{
		GPIO_SetBit(GPIOA, PIN5);					//����LED
		for(i = 0; i < SystemCoreClock/4; i++) __NOP();
		GPIO_ClrBit(GPIOA, PIN5);					//Ϩ��LED
		
		__disable_irq();
		switchTo20MHz();	//����ǰ���л��� 20MHz
		
		SYS->RTCWKSR = SYS_RTCWKSR_FLAG_Msk;		//���־
		SYS->SLEEP |= (1 << SYS_SLEEP_SLEEP_Pos);	//����˯��ģʽ
		while((SYS->RTCWKSR & SYS_RTCWKSR_FLAG_Msk) == 0) __NOP();
		
		switchToPLL(0);		//���Ѻ��л��� PLL
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
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);	//GPIOM.0����ΪUART0��������
	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);	//GPIOM.1����ΪUART0�������
 	
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
* ��������: fputc()
* ����˵��: printf()ʹ�ô˺������ʵ�ʵĴ��ڴ�ӡ����
* ��    ��: int ch		Ҫ��ӡ���ַ�
*			FILE *f		�ļ����
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0));
 	
	return ch;
}
