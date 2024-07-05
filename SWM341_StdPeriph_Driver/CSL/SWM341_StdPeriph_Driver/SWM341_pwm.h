#ifndef __SWM341_PWM_H__
#define	__SWM341_PWM_H__


typedef struct {
	uint8_t  Mode;			// PWM_EDGE_ALIGNED, PWM_CENTER_ALIGNED, PWM_ASYM_CENTER_ALIGNED
	
	uint16_t Clkdiv;		// 1--1024
	
	uint16_t Period;
	
	uint16_t HdutyA;		// high level duration, for PWM_EDGE_ALIGNED mode, PWM_CENTER_ALIGNED mode and the first half cycle in PWM_ASYM_CENTER_ALIGNED mode
	uint16_t HdutyA2;		// only for PWM_ASYM_CENTER_ALIGNED mode, high level duration for the second half cycle
	uint16_t DeadzoneA;		// 0--1023
	uint8_t  IdleLevelA;	// PWMxA output level in stop state
	uint8_t  IdleLevelAN;	// PWMxAN output level in stop state
	uint8_t  OutputInvA;	// PWMxA output invert
	uint8_t  OutputInvAN;	// PWMxAN output invert
	
	uint16_t HdutyB;
	uint16_t HdutyB2;
	uint16_t DeadzoneB;
	uint8_t  IdleLevelB;
	uint8_t  IdleLevelBN;
	uint8_t  OutputInvB;
	uint8_t  OutputInvBN;
	
	uint8_t  UpOvfIE;		// counting up overflow interrupt enable, for PWM_EDGE_ALIGNED mode the counter always counting up
	uint8_t  DownOvfIE;		// counting down overflow interrupt enable, for XXX_CENTER_ALIGNED mode, the counter first counting up until overflow, and then counting down
	uint8_t  UpCmpAIE;		// when counting up, the counter match HdutyA interrupt enabled
	uint8_t  DownCmpAIE;	// when counting down, the counter match HdutyA/HdutyA2 interrupt enabled
	uint8_t  UpCmpBIE;		// when counting up, the counter match HdutyB interrupt enabled
	uint8_t  DownCmpBIE;	// when counting down, the counter match HdutyB/HdutyB2 interrupt enabled
} PWM_InitStructure;


#define PWM_EDGE_ALIGNED		  0
#define PWM_CENTER_ALIGNED		  1		// center alignment mode, the counter always compare with HdutyA
#define PWM_ASYM_CENTER_ALIGNED   2		// asymmetric center alignment mode, the counter compare with HdutyA when counting up, with HdutyA2 when counting down


#define PWM_CH_A	0
#define PWM_CH_B	1

#define PWM0_MSK	(1 << 0)
#define PWM1_MSK	(1 << 1)
#define PWM2_MSK	(1 << 2)
#define PWM3_MSK	(1 << 3)
#define PWM4_MSK	(1 << 4)

#define PWM_BRK0	(1 << 0)
#define PWM_BRK1	(1 << 1)
#define PWM_BRK2	(1 << 2)

#define PWM_DIR_UP		0
#define PWM_DIR_DOWN	1

#define PWM_TRG_0		0		// PWM output trigger signal when the counter overflows or equal to the specified value
#define PWM_TRG_1		1
#define PWM_TRG_2		2
#define PWM_TRG_3		3
#define PWM_TRG_4		4
#define PWM_TRG_5		5
#define PWM_TRG_6		6
#define PWM_TRG_7		7

#define PWM_EVT_DIS		0		// External event signal can make PWM start counting, stop counting, pause counting, and output keep unchange
#define PWM_EVT_0		1
#define PWM_EVT_1		2
#define PWM_EVT_2		3
#define PWM_EVT_3		4
#define PWM_EVT_4		5
#define PWM_EVT_TIMR0	6
#define PWM_EVT_TIMR1	7

/* Interrupt Type */
#define PWM_IT_OVF_UP	   (1 << 0)
#define PWM_IT_OVF_DOWN	   (1 << 1)
#define PWM_IT_CMPA_UP	   (1 << 2)
#define PWM_IT_CMPB_UP	   (1 << 3)
#define PWM_IT_CMPA_DOWN   (1 << 4)
#define PWM_IT_CMPB_DOWN   (1 << 5)

#define PWM_BRKIT_BRK0	   (1 << 0)		// brake signal on pin PWM_BRK0
#define PWM_BRKIT_BRK1	   (1 << 1)
#define PWM_BRKIT_BRK2	   (1 << 2)



void PWM_Init(PWM_TypeDef * PWMx, PWM_InitStructure * initStruct);
void PWM_Start(uint32_t pwm);
void PWM_Stop(uint32_t pwm);
void PWM_Restart(uint32_t pwm);

void PWM_ReloadEn(uint32_t pwm);
void PWM_ReloadDis(uint32_t pwm);

void PWM_BrkInPolarity(uint32_t brk, uint32_t level);
void PWM_BrkConfig(PWM_TypeDef * PWMx, uint32_t chn, uint32_t brk, uint32_t out, uint32_t outN, uint32_t outHold, uint32_t stpCount);

void PWM_OvfTrigger(PWM_TypeDef * PWMx, uint32_t en_up, uint32_t en_down, uint32_t trig_chn);
void PWM_CmpTrigger(PWM_TypeDef * PWMx, uint16_t match, uint32_t dir, uint32_t width, uint32_t trig_chn, uint32_t adc_trig_pos);

void PWM_OutMask(PWM_TypeDef * PWMx, uint32_t chn, uint32_t evt, uint32_t out, uint32_t evt_n, uint32_t out_n);

void PWM_SetPeriod(PWM_TypeDef * PWMx, uint16_t period);
uint16_t PWM_GetPeriod(PWM_TypeDef * PWMx);
void PWM_SetHDuty(PWM_TypeDef * PWMx, uint32_t chn, uint16_t hduty);
uint16_t PWM_GetHDuty(PWM_TypeDef * PWMx, uint32_t chn);
void PWM_SetHDuty2(PWM_TypeDef * PWMx, uint32_t chn, uint16_t hduty, uint16_t hduty2);
void PWM_GetHDuty2(PWM_TypeDef * PWMx, uint32_t chn, uint16_t *hduty, uint16_t *hduty2);
void PWM_SetDeadzone(PWM_TypeDef * PWMx, uint32_t chn, uint16_t deadzone);
uint16_t PWM_GetDeadzone(PWM_TypeDef * PWMx, uint32_t chn);

void PWM_IntEn(PWM_TypeDef * PWMx, uint32_t it);
void PWM_IntDis(PWM_TypeDef * PWMx, uint32_t it);
void PWM_IntClr(PWM_TypeDef * PWMx, uint32_t it);
uint32_t PWM_IntStat(PWM_TypeDef * PWMx, uint32_t it);
void PWM_BrkIntEn(uint32_t brkit);
void PWM_BrkIntDis(uint32_t brkit);
void PWM_BrkIntClr(uint32_t brkit);
uint32_t PWM_BrkIntStat(uint32_t brkit);


#endif
