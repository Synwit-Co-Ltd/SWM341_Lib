#ifndef __SWM341_ADC_H__
#define	__SWM341_ADC_H__


typedef struct {
	uint8_t  clk_src;		// ADC clock source, can be ADC_CLKSRC_HRC, ADC_CLKSRC_XTAL, ...
	uint8_t  samplAvg;		// Sampling averaging, can be ADC_AVG_SAMPLE1, ADC_AVG_SAMPLE2, ADC_AVG_SAMPLE4, ADC_AVG_SAMPLE8
	
	uint8_t  EOC_IEn;		// End of Conversion interrupt enable, can be ADC_SEQ0, ADC_SEQ1, ADC_SEQ2, ADC_SEQ3 and their '|' operation
	uint8_t  HalfIEn;		// FIFO half full interrupt enable, can be ADC_SEQ0, ADC_SEQ1, ADC_SEQ2, ADC_SEQ3 and their '|' operation
} ADC_InitStructure;

typedef struct {
	uint16_t channels;		// SEQ convert channel, can be ADC_CH0, ADC_CH1, ... ..., ADC_CH7 and their '|' operation
	uint8_t  trig_src;		// SEQ trigger method, can be ADC_TRIGGER_SW, ADC_TRIGGER_TIMER2, ADC_TRIGGER_TIMER3, ... ...
	uint16_t conv_cnt;		// SEQ convert times, can be 1--256
	uint8_t  samp_tim;		// SEQ sample time, can be ADC_SAMPLE_1CLOCK, ADC_SAMPLE_2CLOCK, ADC_SAMPLE_4CLOCK, ADC_SAMPLE_8CLOCK
} ADC_SEQ_InitStructure;

typedef struct {
	uint16_t UpperLimit;	// Compare upper limit value
	uint16_t UpperLimitIEn;	// Convert result > UpperLimit interrupt enable
	uint16_t LowerLimit;	// Compare lower limit value
	uint16_t LowerLimitIEn;	// Convert result < LowerLimit interrupt enable
} ADC_CMP_InitStructure;

#define ADC_CH0		0x001
#define ADC_CH1		0x002
#define ADC_CH2		0x004
#define ADC_CH3		0x008
#define ADC_CH4		0x010
#define ADC_CH5		0x020
#define ADC_CH6		0x040
#define ADC_CH7		0x080
#define ADC_CH8		0x100
#define ADC_CH9		0x200
#define ADC_CH10	0x400
#define ADC_CH11	0x800

#define ADC_SEQ0	0x1
#define ADC_SEQ1	0x2
#define ADC_SEQ2	0x4
#define ADC_SEQ3	0x8

#define ADC_CLKSRC_HRC          ((0 << 2) | 0)
#define ADC_CLKSRC_XTAL         ((0 << 2) | 1)
#define ADC_CLKSRC_PLL          ((0 << 2) | 2)
#define ADC_CLKSRC_HRC_DIV4     ((2 << 2) | 0)
#define ADC_CLKSRC_XTAL_DIV4    ((2 << 2) | 1)
#define ADC_CLKSRC_PLL_DIV4     ((2 << 2) | 2)
#define ADC_CLKSRC_HRC_DIV8     ((3 << 2) | 0)
#define ADC_CLKSRC_XTAL_DIV8    ((3 << 2) | 1)
#define ADC_CLKSRC_PLL_DIV8     ((3 << 2) | 2)

#define ADC_AVG_SAMPLE1			0
#define ADC_AVG_SAMPLE2			1	// start one time, convert 2 times, and calculate the average of the two results as the conversion result
#define ADC_AVG_SAMPLE4			2
#define ADC_AVG_SAMPLE8			3

#define ADC_TRIGGER_NO			0
#define ADC_TRIGGER_SW			1	// software start
#define ADC_TRIGGER_TIMER2		2
#define ADC_TRIGGER_TIMER3		3
#define ADC_TRIGGER_EXTRIG0		4
#define ADC_TRIGGER_EXTRIG1		5
#define ADC_TRIGGER_PWM0		0x10
#define ADC_TRIGGER_PWM1		0x11
#define ADC_TRIGGER_PWM2		0x12
#define ADC_TRIGGER_PWM3		0x13
#define ADC_TRIGGER_PWM4		0x14

#define ADC_SAMPLE_1CLOCK		0
#define ADC_SAMPLE_2CLOCK		1
#define ADC_SAMPLE_4CLOCK		2
#define ADC_SAMPLE_8CLOCK		3
#define ADC_SAMPLE_16CLOCK		4
#define ADC_SAMPLE_32CLOCK		5
#define ADC_SAMPLE_64CLOCK		6
#define ADC_SAMPLE_128CLOCK		7


/* Interrupt Type */
#define ADC_IT_EOC			(1 << 0)	// End Of Conversion
#define ADC_IT_FIFO_OVF		(1 << 1)	// FIFO Overflow
#define ADC_IT_FIFO_HALF	(1 << 2)	// FIFO Half Full
#define ADC_IT_FIFO_FULL	(1 << 3)	// FIFO Full
#define ADC_IT_CMP_MAX		(1 << 4)	// convert result > COMP.MAX
#define ADC_IT_CMP_MIN		(1 << 5)	// convert result < COMP.MIN


void ADC_Init(ADC_TypeDef * ADCx, ADC_InitStructure * initStruct);
void ADC_SEQ_Init(ADC_TypeDef * ADCx, uint32_t seq, ADC_SEQ_InitStructure * initStruct);
void ADC_CMP_Init(ADC_TypeDef * ADCx, uint32_t seq, ADC_CMP_InitStructure * initStruct);
void ADC_Open(ADC_TypeDef * ADCx);
void ADC_Calibrate(ADC_TypeDef * ADCx);
void ADC_Close(ADC_TypeDef * ADCx);
void ADC_Start(ADC_TypeDef * ADCx, uint32_t seq);
void ADC_Stop(ADC_TypeDef * ADCx, uint32_t seq);

uint32_t ADC_Read(ADC_TypeDef * ADCx, uint32_t seq, uint32_t *chn);
uint32_t ADC_DataAvailable(ADC_TypeDef * ADCx, uint32_t seq);


void ADC_INTEn(ADC_TypeDef * ADCx, uint32_t seq, uint32_t it);
void ADC_INTEn(ADC_TypeDef * ADCx, uint32_t seq, uint32_t it);
void ADC_INTClr(ADC_TypeDef * ADCx, uint32_t seq, uint32_t it);
uint32_t ADC_INTStat(ADC_TypeDef * ADCx, uint32_t seq, uint32_t it);


#endif
