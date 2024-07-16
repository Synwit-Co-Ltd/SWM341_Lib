#include "SWM341.h"

#include "rtthread.h"


void TaskADC(void *arg);
void TaskPWM(void *arg);

struct rt_thread taskADC;
static uint8_t stackADC[512];
static uint8_t priorityADC = 4;

struct rt_thread taskPWM;
static uint8_t stackPWM[512];
static uint8_t priorityPWM = 5;

struct rt_mailbox queueADC;
static uint32_t mbPool[16];


void SerialInit(void);

int main(void)
{	
	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		// debug indication signal
	
	rt_mb_init(&queueADC, "ADC", mbPool, sizeof(mbPool)/sizeof(mbPool[0]), RT_IPC_FLAG_FIFO);
		
	rt_thread_init(&taskADC, "ADC", TaskADC, RT_NULL, stackADC, sizeof(stackADC), priorityADC, 20);
	rt_thread_startup(&taskADC);
	
	rt_thread_init(&taskPWM, "PWM", TaskPWM, RT_NULL, stackPWM, sizeof(stackPWM), priorityPWM, 20);
	rt_thread_startup(&taskPWM);
		
	while(1)
	{		
		// do nothing
		
		rt_thread_delay(500 / (1000 / RT_TICK_PER_SECOND));
	}
}

/*******************************************************************************************************************************
* @brief	ADC task, periodically start ADC conversion
* @param
* @return
*******************************************************************************************************************************/
void TaskADC(void *arg)
{
	ADC_InitStructure ADC_initStruct;
	ADC_SEQ_InitStructure ADC_SEQ_initStruct;
	
	PORT_Init(PORTC, PIN6,  PORTC_PIN6_ADC0_CH0,  0);	//PC.6  => ADC.CH0
	PORT_Init(PORTC, PIN5,  PORTC_PIN5_ADC0_CH1,  0);	//PC.5  => ADC.CH1
	PORT_Init(PORTC, PIN4,  PORTC_PIN4_ADC0_CH2,  0);	//PC.4  => ADC.CH2
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_ADC0_CH3,  0);	//PC.3  => ADC.CH3
	PORT_Init(PORTC, PIN2,  PORTC_PIN2_ADC0_CH4,  0);	//PC.2  => ADC.CH4
	PORT_Init(PORTC, PIN1,  PORTC_PIN1_ADC0_CH5,  0);	//PC.1  => ADC.CH5
	PORT_Init(PORTC, PIN0,  PORTC_PIN0_ADC0_CH6,  0);	//PC.0  => ADC.CH6
	PORT_Init(PORTA, PIN15, PORTA_PIN15_ADC0_CH7, 0);	//PA.15 => ADC.CH7
	
	ADC_initStruct.clk_src = ADC_CLKSRC_HRC_DIV8;
	ADC_initStruct.samplAvg = ADC_AVG_SAMPLE1;
	ADC_initStruct.EOC_IEn = ADC_SEQ0;	
	ADC_initStruct.HalfIEn = 0;
	ADC_Init(ADC0, &ADC_initStruct);
	
	ADC_SEQ_initStruct.channels = ADC_CH5;
	ADC_SEQ_initStruct.trig_src = ADC_TRIGGER_SW;
	ADC_SEQ_initStruct.conv_cnt = 1;
	ADC_SEQ_initStruct.samp_tim = ADC_SAMPLE_1CLOCK;
	ADC_SEQ_Init(ADC0, ADC_SEQ0, &ADC_SEQ_initStruct);
	
	ADC_Open(ADC0);
	
	while(1)
	{
		ADC_Start(ADC0, ADC_SEQ0);
		
		rt_thread_delay(100 / (1000 / RT_TICK_PER_SECOND));
	}
}

void ADC0_Handler(void)
{	
	uint32_t chn;
	
	ADC0->IF = (1 << ADC_IF_SEQ0EOC_Pos);
	
	rt_mb_send(&queueADC, ADC_Read(ADC0, ADC_SEQ0, &chn));
	
	GPIO_InvBit(GPIOA, PIN5);
}

/*******************************************************************************************************************************
* @brief	PWM task, wait for ADC conversion result and set the PWM duty cycle according to the ADC conversion result
* @param
* @return
*******************************************************************************************************************************/
void TaskPWM(void *arg)
{
	rt_uint32_t duty;
	PWM_InitStructure PWM_initStruct;
	
//	PORT_Init(PORTM, PIN1, PORTM_PIN1_PWM0A,  0);	// UART0 TXD pin
	PORT_Init(PORTM, PIN4, PORTM_PIN4_PWM0AN, 0);
	PORT_Init(PORTM, PIN2, PORTM_PIN2_PWM0B,  0);
	PORT_Init(PORTM, PIN5, PORTM_PIN5_PWM0BN, 0);
	
	PWM_initStruct.Mode = PWM_CENTER_ALIGNED;
	PWM_initStruct.Clkdiv = 6;					// F_PWM = 30M/6 = 5M	
	PWM_initStruct.Period = 10000;				// 5M/10000 = 500Hz, 250Hz for center alignment mode
	PWM_initStruct.HdutyA =  2500;				// 2500/10000 = 25%
	PWM_initStruct.DeadzoneA = 50;				// 50/5M = 10us
	PWM_initStruct.IdleLevelA = 0;
	PWM_initStruct.IdleLevelAN= 0;
	PWM_initStruct.OutputInvA = 0;
	PWM_initStruct.OutputInvAN= 0;
	PWM_initStruct.HdutyB =  7500;				// 5000/10000 = 50%
	PWM_initStruct.DeadzoneB = 50;				// 50/5M = 10us
	PWM_initStruct.IdleLevelB = 0;
	PWM_initStruct.IdleLevelBN= 0;
	PWM_initStruct.OutputInvB = 0;
	PWM_initStruct.OutputInvBN= 0;
	PWM_initStruct.UpOvfIE    = 0;
	PWM_initStruct.DownOvfIE  = 0;
	PWM_initStruct.UpCmpAIE   = 0;
	PWM_initStruct.DownCmpAIE = 0;
	PWM_initStruct.UpCmpBIE   = 0;
	PWM_initStruct.DownCmpBIE = 0;
	PWM_Init(PWM0, &PWM_initStruct);
	
	PWM_Start(PWM0_MSK);
	
	while(1)
	{
		if(rt_mb_recv(&queueADC, &duty, 100) == RT_EOK)
		{
			printf("%d,", (int)duty);
			if(duty <  100) duty =  100;
			if(duty > 4000) duty = 4000;
			
			PWM_SetHDuty(PWM0, PWM_CH_A, 10000 * duty / 4096);
			PWM_SetHDuty(PWM0, PWM_CH_B, 10000 * (4096 - duty) / 4096);
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
