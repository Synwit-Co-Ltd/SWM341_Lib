#include "SWM341.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


/* 开启 FPU 支持：
 * 1）将 port.c 替换为 APP\FreeRTOS\portable\GCC\ARM_CM4F 目录下的 port.c
 * 2）将 Options => C/C++ (AC6) => Include Paths 页面中的 .\APP\FreeRTOS\portable\GCC\ARM_CM3 替换为
 *														  .\APP\FreeRTOS\portable\GCC\ARM_CM4F
 * 3）将 Options => Target 页面中的 Floating Point Hardware 选项由 Not Used 改为 Single Precision
*/


QueueHandle_t queueADC;

void TaskADC(void *arg);
void TaskPWM(void *arg);
void SerialInit(void);

int main(void)
{ 	
 	SystemInit();
	
	SerialInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);		//调试指示信号
	
	xTaskCreate(TaskADC, (const char *)"ADC", 128, NULL, 2, NULL);
	xTaskCreate(TaskPWM, (const char *)"PWM", 128, NULL, 3, NULL);
	
	queueADC = xQueueCreate(16, 2);
	
	vTaskStartScheduler();
}

/****************************************************************************************************************************************** 
* 函数名称:	TaskADC()
* 功能说明: 启动ADC采集任务
* 输    入: void *arg
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
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
	ADC_Init(ADC0, &ADC_initStruct);					//配置ADC
	
	ADC_SEQ_initStruct.channels = ADC_CH5;
	ADC_SEQ_initStruct.trig_src = ADC_TRIGGER_SW;
	ADC_SEQ_initStruct.conv_cnt = 1;
	ADC_SEQ_initStruct.samp_tim = ADC_SAMPLE_1CLOCK;
	ADC_SEQ_Init(ADC0, ADC_SEQ0, &ADC_SEQ_initStruct);
	
	NVIC_SetPriority(ADC0_IRQn, 5);
	
	ADC_Open(ADC0);										//使能ADC
	
	while(1)
	{
		ADC_Start(ADC0, ADC_SEQ0);
		
		vTaskDelay(100);
	}
}

void ADC0_Handler(void)
{
	uint32_t val, chn;
	
	ADC0->IF = (1 << ADC_IF_SEQ0EOC_Pos);
	
	val = ADC_Read(ADC0, ADC_SEQ0, &chn);
	
	xQueueSendFromISR(queueADC, &val, 0);
	
	GPIO_InvBit(GPIOA, PIN5);
}

/****************************************************************************************************************************************** 
* 函数名称:	TaskPWM()
* 功能说明: 等待ADC转换结果，根据ADC转换结果设置PWM占空比
* 输    入: void *arg
* 输    出: 无
* 注意事项: 无
******************************************************************************************************************************************/
void TaskPWM(void *arg)
{
	uint16_t duty;
	PWM_InitStructure PWM_initStruct;
	
//	PORT_Init(PORTM, PIN1, PORTM_PIN1_PWM0A,  0);	//UART0.TXD
	PORT_Init(PORTM, PIN4, PORTM_PIN4_PWM0AN, 0);
	PORT_Init(PORTM, PIN2, PORTM_PIN2_PWM0B,  0);
	PORT_Init(PORTM, PIN5, PORTM_PIN5_PWM0BN, 0);
	
	PWM_initStruct.Mode = PWM_CENTER_ALIGNED;
	PWM_initStruct.Clkdiv = 6;					//F_PWM = 30M/6 = 5M	
	PWM_initStruct.Period = 10000;				//5M/10000 = 500Hz，中心对称模式下频率降低到250Hz
	PWM_initStruct.HdutyA =  2500;				//2500/10000 = 25%
	PWM_initStruct.DeadzoneA = 50;				//50/5M = 10us
	PWM_initStruct.IdleLevelA = 0;
	PWM_initStruct.IdleLevelAN= 0;
	PWM_initStruct.OutputInvA = 0;
	PWM_initStruct.OutputInvAN= 0;
	PWM_initStruct.HdutyB =  7500;				//5000/10000 = 50%
	PWM_initStruct.DeadzoneB = 50;				//50/5M = 10us
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
		if(xQueueReceive(queueADC, &duty, 10) == pdTRUE)
		{
			printf("%d,", duty);
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
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);	//GPIOM.0配置为UART0输入引脚
	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);	//GPIOM.1配置为UART0输出引脚
 	
 	UART_initStruct.Baudrate = 57600;
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
 	while(UART_IsTXFIFOFull(UART0));
	
	UART_WriteByte(UART0, ch);
 	
	return ch;
}
