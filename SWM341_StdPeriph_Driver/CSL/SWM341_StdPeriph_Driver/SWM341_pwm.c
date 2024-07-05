/*******************************************************************************************************************************
* @brief	PWM driver
*
*
********************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology 
*******************************************************************************************************************************/
#include "SWM341.h"
#include "SWM341_pwm.h"


/*******************************************************************************************************************************
* @brief	PWM init
* @param	PWMx is the PWM to init
* @param	initStruct is data used to init the PWM
* @return
*******************************************************************************************************************************/
void PWM_Init(PWM_TypeDef * PWMx, PWM_InitStructure * initStruct)
{
	SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_PWM_Pos);
	
	PWMx->CR = (initStruct->Mode         << PWM_CR_MODE_Pos)   |
			   (1                        << PWM_CR_MULT_Pos)   |	// multiple count mode
	           (0                        << PWM_CR_DIR_Pos)    |	// counting up
			   (0                        << PWM_CR_CLKSRC_Pos) |	// system clock
			   ((initStruct->Clkdiv - 1) << PWM_CR_CLKDIV_Pos) |
			   (0                        << PWM_CR_RPTNUM_Pos);		// a register reload is performed each time the counter overflow
	
	PWMx->OCR = ((initStruct->IdleLevelA  ? 1 : 0) << PWM_OCR_IDLEA_Pos)  |
				((initStruct->IdleLevelB  ? 1 : 0) << PWM_OCR_IDLEB_Pos)  |
				((initStruct->IdleLevelAN ? 1 : 0) << PWM_OCR_IDLEAN_Pos) |
				((initStruct->IdleLevelBN ? 1 : 0) << PWM_OCR_IDLEBN_Pos) |
				((initStruct->OutputInvA  ? 1 : 0) << PWM_OCR_INVA_Pos)   |
				((initStruct->OutputInvB  ? 1 : 0) << PWM_OCR_INVB_Pos)   |
				((initStruct->OutputInvAN ? 1 : 0) << PWM_OCR_INVAN_Pos)  |
				((initStruct->OutputInvBN ? 1 : 0) << PWM_OCR_INVBN_Pos);
	
	PWMx->PERIOD = initStruct->Period - 1;
	
	PWMx->CMPA  = initStruct->HdutyA;
	PWMx->CMPA2 = initStruct->HdutyA2;
	PWMx->DZA   = initStruct->DeadzoneA;
	
	PWMx->CMPB  = initStruct->HdutyB;
	PWMx->CMPB2 = initStruct->HdutyB2;
	PWMx->DZB   = initStruct->DeadzoneB;
	
	PWMG->RELOADEN = 0x3F;
	
	PWMx->IF = 0x3F;
	PWMx->IE = ((initStruct->UpOvfIE    ? 1 : 0) << PWM_IE_UPOVF_Pos)  |
			   ((initStruct->DownOvfIE  ? 1 : 0) << PWM_IE_DNOVF_Pos)  |
			   ((initStruct->UpCmpAIE   ? 1 : 0) << PWM_IE_UPCMPA_Pos) |
			   ((initStruct->DownCmpAIE ? 1 : 0) << PWM_IE_DNCMPA_Pos) |
			   ((initStruct->UpCmpBIE   ? 1 : 0) << PWM_IE_UPCMPB_Pos) |
			   ((initStruct->DownCmpBIE ? 1 : 0) << PWM_IE_DNCMPB_Pos);
	
	if(initStruct->UpOvfIE || initStruct->DownOvfIE || initStruct->UpCmpAIE || 
	   initStruct->DownCmpAIE || initStruct->UpCmpBIE || initStruct->DownCmpBIE)
	{
		switch((uint32_t)PWMx)
		{
		case((uint32_t)PWM0):
			NVIC_EnableIRQ(PWM0_IRQn);
			break;
		
		case((uint32_t)PWM1):
			NVIC_EnableIRQ(PWM1_IRQn);
			break;
		
		case((uint32_t)PWM2):
			NVIC_EnableIRQ(PWM2_IRQn);
			break;
		
		case((uint32_t)PWM3):
			NVIC_EnableIRQ(PWM3_IRQn);
			break;
		
		case((uint32_t)PWM4):
			NVIC_EnableIRQ(PWM4_IRQn);
			break;
		}
	}
}

/*******************************************************************************************************************************
* @brief	PWM start
* @param	pwm is the PWM to start, can be PWM0_MSK, PWM1_MSK, PWM2_MSK, PWM3_MSK, PWM4_MSK and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_Start(uint32_t pwm)
{
	PWMG->START |= pwm;
}

/*******************************************************************************************************************************
* @brief	PWM stop
* @param	pwm is the PWM to stop, can be PWM0_MSK, PWM1_MSK, PWM2_MSK, PWM3_MSK, PWM4_MSK and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_Stop(uint32_t pwm)
{
	PWMG->START &= ~pwm;
}

/*******************************************************************************************************************************
* @brief	PWM restart, PWM counter clear, and use new period, hduty and deadzone setting
* @param	pwm is the PWM to restart, can be PWM0_MSK, PWM1_MSK, PWM2_MSK, PWM3_MSK, PWM4_MSK and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_Restart(uint32_t pwm)
{
	PWMG->RESTART = (pwm << PWMG_RESTART_PWM0_Pos);
}

/*******************************************************************************************************************************
* @brief	PWM reload enable
* @param	pwm is the PWM to set, can be PWM0_MSK, PWM1_MSK, PWM2_MSK, PWM3_MSK, PWM4_MSK and their '|' operation
* @return
* @note		Only when reload enabled, values written to registers such as PERIOD, CMPA, CMPB, DZA, DZB, etc. are loaded (when 
*			the counter overflows) into the internal working register
*******************************************************************************************************************************/
void PWM_ReloadEn(uint32_t pwm)
{
	PWMG->RELOADEN |= pwm;
}

/*******************************************************************************************************************************
* @brief	PWM reload disable
* @param	pwm is the PWM to set, can be PWM0_MSK, PWM1_MSK, PWM2_MSK, PWM3_MSK, PWM4_MSK and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_ReloadDis(uint32_t pwm)
{
	PWMG->RELOADEN &= ~pwm;
}

/*******************************************************************************************************************************
* @brief	PWM brake input polarity set
* @param	brk is the brake input to set, can be PWM_BRK0, PWM_BRK1, PWM_BRK2 and their '|' operation
* @param	level is brake level, 0 brake when brake input is low level
* @return
*******************************************************************************************************************************/
void PWM_BrkInPolarity(uint32_t brk, uint32_t level)
{
	if(level)
		PWMG->BRKPOL |= brk;
	else
		PWMG->BRKPOL &= ~brk;
}

/*******************************************************************************************************************************
* @brief	PWM brake configure
* @param	PWMx is the PWM to set
* @param	chn is the PWM channel to set, can be PWM_CH_A or PWM_CH_B
* @param	brk is the brake signal to use, can be PWM_BRK0, PWM_BRK1, PWM_BRK2 and their '|' operation
* @param	out is PWMxA/PWMxB output level when brake signal active
* @param	outN is PWMxAN/PWMxBN output level when brake signal active
* @param	outHold set PWM output when brake signal disappear: 0 immediately recover, 1 stay unchange until overflow
* @param	stpCount set PWM counter behave when brake signal active: 0 continue counting, 1 stop counting
* @return
*******************************************************************************************************************************/
void PWM_BrkConfig(PWM_TypeDef * PWMx, uint32_t chn, uint32_t brk, uint32_t out, uint32_t outN, uint32_t outHold, uint32_t stpCount)
{
	if(chn == PWM_CH_A)
	{
		PWMx->BRKIN &= ~(0x7 << PWM_BRKIN_BRK0A_Pos);
		PWMx->BRKIN |=  (brk << PWM_BRKIN_BRK0A_Pos);
		
		PWMx->BRKCR &= ~(PWM_BRKCR_OUTA_Msk | PWM_BRKCR_OUTAN_Msk | PWM_BRKCR_OFFA_Msk);
		PWMx->BRKCR |= (out      << PWM_BRKCR_OUTA_Pos)  |
					   (outN     << PWM_BRKCR_OUTAN_Pos) |
					   (outHold  << PWM_BRKCR_OFFA_Pos)  |
					   (stpCount << PWM_BRKCR_STPCNT_Pos);
	}
	else
	{
		PWMx->BRKIN &= ~(0x7 << PWM_BRKIN_BRK0B_Pos);
		PWMx->BRKIN |=  (brk << PWM_BRKIN_BRK0B_Pos);
		
		PWMx->BRKCR &= ~(PWM_BRKCR_OUTB_Msk | PWM_BRKCR_OUTBN_Msk | PWM_BRKCR_OFFB_Msk);
		PWMx->BRKCR |= (out      << PWM_BRKCR_OUTB_Pos)  |
					   (outN     << PWM_BRKCR_OUTBN_Pos) |
					   (outHold  << PWM_BRKCR_OFFB_Pos)  |
					   (stpCount << PWM_BRKCR_STPCNT_Pos);
	}
}

/*******************************************************************************************************************************
* @brief	PWM counter overflow trigger signal configure
* @param	PWMx is the PWM to set
* @param	en_up = 1: when counting up, counter overflow will generate trigger signal
* @param	en_down = 1: when counting down, counter overflow will generate trigger signal
* @param	trig_chn select which the generated trigger signal will be used as: PWM_TRG_0, PWM_TRG_1, ..., or PWM_TRG_7
* @return
*******************************************************************************************************************************/
void PWM_OvfTrigger(PWM_TypeDef * PWMx, uint32_t en_up, uint32_t en_down, uint32_t trig_chn)
{
	PWMx->OVFTRG = (en_up    << PWM_OVFTRG_UPEN_Pos) |
				   (en_down  << PWM_OVFTRG_DNEN_Pos) |
				   (trig_chn << PWM_OVFTRG_MUX_Pos);
}

/*******************************************************************************************************************************
* @brief	PWM counter match trigger signal configure
* @param	PWMx is the PWM to set
* @param	match: when counter == match, PWM will generate a trigger signal
* @param	dir = PWM_DIR_UP: when counting up, do the compare; dir = PWM_DIR_DOWN: when counting down, do the compare
* @param	width is the trigger signal width: width * 4 clock cycle, so no trigger signal output when width == 0
* @param	trig_chn select which the generated trigger signal will be used as: PWM_TRG_0, PWM_TRG_1, ..., or PWM_TRG_7
* @param	adc_trig_pos: when the specified width trigger signal is sent, the adc trigger signal is also sent at the 
*			adc_trig_pos/8 position of the trigger signal width, with the value 0--7
* @return
*******************************************************************************************************************************/
void PWM_CmpTrigger(PWM_TypeDef * PWMx, uint16_t match, uint32_t dir, uint32_t width, uint32_t trig_chn, uint32_t adc_trig_pos)
{
	PWMx->CMPTRG = (1        << PWM_CMPTRG_EN_Pos)    |
				   (match    << PWM_CMPTRG_CMP_Pos)   |
				   (dir      << PWM_CMPTRG_DIR_Pos)   |
				   (width    << PWM_CMPTRG_WIDTH_Pos) |
				   (trig_chn << PWM_CMPTRG_MUX_Pos);
	
	PWM0->CMPTRG &= ~PWM_CMPTRG_ATP_Msk;
	PWM0->CMPTRG |= (adc_trig_pos << PWM_CMPTRG_ATP_Pos);
}

/*******************************************************************************************************************************
* @brief	PWM output mask configure
* @param	PWMx is the PWM to set
* @param	chn is the PWM channel to set, can be PWM_CH_A or PWM_CH_B
* @param	evt select event input to mask PWMxA/PWMxB output, can be PWM_EVT_DIS, PWM_EVT_0, PWM_EVT_1, ..., PWM_EVT_6
* @param	out set PWMxA/PWMxB output level when event input selected by evt is high
* @param	evt_n select event input to mask PWMxAN/PWMxBN output, can be PWM_EVT_DIS, PWM_EVT_0, PWM_EVT_1, ..., PWM_EVT_6
* @param	out_n set PWMxAN/PWMxBN output level when event input selected by evt_n is high
* @return
* @note		currently, PWM_EVT_0 connected to PWM_TRG_0, PWM_EVT_1 connected to PWM_TRG_1, and so on.
*******************************************************************************************************************************/
void PWM_OutMask(PWM_TypeDef * PWMx, uint32_t chn, uint32_t evt, uint32_t out, uint32_t evt_n, uint32_t out_n)
{
	if(chn == PWM_CH_A)
	{
		PWMx->EVMUX &= ~(PWM_EVMUX_MASKA_Msk | PWM_EVMUX_MASKAN_Msk);
		PWMx->EVMUX |= (evt   << PWM_EVMUX_MASKA_Pos) |
					   (evt_n << PWM_EVMUX_MASKAN_Pos);
		
		PWMx->EVMSK &= ~(PWM_EVMSK_OUTA_Msk | PWM_EVMSK_OUTAN_Msk);
		PWMx->EVMSK |= (out   << PWM_EVMSK_OUTA_Pos)  |
					   (out_n << PWM_EVMSK_OUTAN_Pos) |
					   (1     << PWM_EVMSK_IMME_Pos);
	}
	else
	{
		PWMx->EVMUX &= ~(PWM_EVMUX_MASKB_Msk | PWM_EVMUX_MASKBN_Msk);
		PWMx->EVMUX |= (evt   << PWM_EVMUX_MASKB_Pos) |
					   (evt_n << PWM_EVMUX_MASKBN_Pos);
		
		PWMx->EVMSK &= ~(PWM_EVMSK_OUTB_Msk | PWM_EVMSK_OUTBN_Msk);
		PWMx->EVMSK |= (out   << PWM_EVMSK_OUTB_Pos)  |
					   (out_n << PWM_EVMSK_OUTBN_Pos) |
					   (1     << PWM_EVMSK_IMME_Pos);
	}
}

/*******************************************************************************************************************************
* @brief	PWM period set
* @param	PWMx is the PWM to set
* @param	period is new period value to set
* @return
*******************************************************************************************************************************/
void PWM_SetPeriod(PWM_TypeDef * PWMx, uint16_t period)
{
	PWMx->PERIOD = period - 1;
}

/*******************************************************************************************************************************
* @brief	PWM period value query
* @param	PWMx is the PWM to query
* @return	current period value
*******************************************************************************************************************************/
uint16_t PWM_GetPeriod(PWM_TypeDef * PWMx)
{
	return PWMx->PERIOD + 1;
}

/*******************************************************************************************************************************
* @brief	PWM duty set
* @param	PWMx is the PWM to set
* @param	chn is the PWM channel to set, can be PWM_CH_A or PWM_CH_B
* @param	hduty is new high level duration value to set
* @return
*******************************************************************************************************************************/
void PWM_SetHDuty(PWM_TypeDef * PWMx, uint32_t chn, uint16_t hduty)
{
	if(chn == PWM_CH_A)
		PWMx->CMPA = hduty;
	else
		PWMx->CMPB = hduty;
}

/*******************************************************************************************************************************
* @brief	PWM duty value query
* @param	PWMx is the PWM to query
* @param	chn is the PWM channel to query, can be PWM_CH_A or PWM_CH_B
* @return	current high level duration value
*******************************************************************************************************************************/
uint16_t PWM_GetHDuty(PWM_TypeDef * PWMx, uint32_t chn)
{
	if(chn == PWM_CH_A)
		return PWMx->CMPA;
	else
		return PWMx->CMPB;
}

/*******************************************************************************************************************************
* @brief	PWM (in PWM_ASYM_CENTER_ALIGNED mode) duty set
* @param	PWMx is the PWM to set
* @param	chn is the PWM channel to set, can be PWM_CH_A or PWM_CH_B
* @param	hduty is new high level duration value (for the first half cycle) to set
* @param	hduty2 is new high level duration value (for the second half cycle) to set
* @return
*******************************************************************************************************************************/
void PWM_SetHDuty2(PWM_TypeDef * PWMx, uint32_t chn, uint16_t hduty, uint16_t hduty2)
{
	if(chn == PWM_CH_A)
	{
		PWMx->CMPA  = hduty;
		PWMx->CMPA2 = hduty2;
	}
	else
	{
		PWMx->CMPB  = hduty;
		PWMx->CMPB2 = hduty2;
	}
}

/*******************************************************************************************************************************
* @brief	PWM (in PWM_ASYM_CENTER_ALIGNED mode) duty value query
* @param	PWMx is the PWM to query
* @param	chn is the PWM channel to query, can be PWM_CH_A or PWM_CH_B
* @param	hduty is current high level duration value for the first half cycle
* @param	hduty2 is current high level duration value for the second half cycle
* @return
*******************************************************************************************************************************/
void PWM_GetHDuty2(PWM_TypeDef * PWMx, uint32_t chn, uint16_t *hduty, uint16_t *hduty2)
{
	if(chn == PWM_CH_A)
	{
		*hduty  = PWMx->CMPA;
		*hduty2 = PWMx->CMPA2;
	}
	else
	{
		*hduty  = PWMx->CMPB;
		*hduty2 = PWMx->CMPB2;
	}
}

/*******************************************************************************************************************************
* @brief	PWM deadzone set
* @param	PWMx is the PWM to set
* @param	chn is the PWM channel to set, can be PWM_CH_A or PWM_CH_B
* @param	deadzone is new deadzone value to set, can be 0--1023
* @return
*******************************************************************************************************************************/
void PWM_SetDeadzone(PWM_TypeDef * PWMx, uint32_t chn, uint16_t deadzone)
{
	if(chn == PWM_CH_A)
		PWMx->DZA = deadzone;
	else
		PWMx->DZB = deadzone;
}

/*******************************************************************************************************************************
* @brief	PWM deadzone value query
* @param	PWMx is the PWM to query
* @param	chn is the PWM channel to query, can be PWM_CH_A or PWM_CH_B
* @return	current deadzone value
*******************************************************************************************************************************/
uint16_t PWM_GetDeadzone(PWM_TypeDef * PWMx, uint32_t chn)
{
	if(chn == PWM_CH_A)
		return PWMx->DZA;
	else
		return PWMx->DZB;
}

/*******************************************************************************************************************************
* @brief	PWM interrupt enable
* @param	PWMx is the PWM to set
* @param	it is interrupt type, can be PWM_IT_OVF_UP, PWM_IT_OVF_DOWN, PWM_IT_CMPA_UP, PWM_IT_CMPB_UP, PWM_IT_CMPA_DOWN, 
*			PWM_IT_CMPB_DOWN and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_IntEn(PWM_TypeDef * PWMx, uint32_t it)
{
	PWMx->IE |= it;
}

/*******************************************************************************************************************************
* @brief	PWM interrupt disable
* @param	PWMx is the PWM to set
* @param	it is interrupt type, can be PWM_IT_OVF_UP, PWM_IT_OVF_DOWN, PWM_IT_CMPA_UP, PWM_IT_CMPB_UP, PWM_IT_CMPA_DOWN, 
*			PWM_IT_CMPB_DOWN and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_IntDis(PWM_TypeDef * PWMx, uint32_t it)
{
	PWMx->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	PWM interrupt flag clear
* @param	PWMx is the PWM to clear
* @param	it is interrupt type, can be PWM_IT_OVF_UP, PWM_IT_OVF_DOWN, PWM_IT_CMPA_UP, PWM_IT_CMPB_UP, PWM_IT_CMPA_DOWN, 
*			PWM_IT_CMPB_DOWN and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_IntClr(PWM_TypeDef * PWMx, uint32_t it)
{
	PWMx->IF = it;
}

/*******************************************************************************************************************************
* @brief	PWM interrupt state query
* @param	PWMx is the PWM to query
* @param	it is interrupt type, can be PWM_IT_OVF_UP, PWM_IT_OVF_DOWN, PWM_IT_CMPA_UP, PWM_IT_CMPB_UP, PWM_IT_CMPA_DOWN, 
*			PWM_IT_CMPB_DOWN and their '|' operation
* @return	0 interrupt not happen, !=0 interrupt happened
*******************************************************************************************************************************/
uint32_t PWM_IntStat(PWM_TypeDef * PWMx, uint32_t it)
{
	return (PWMx->IF & it);
}

/*******************************************************************************************************************************
* @brief	PWM brake interrupt enable
* @param	brkit is brake interrupt type, can be PWM_BRKIT_BRK0, PWM_BRKIT_BRK0, PWM_BRKIT_BRK0 and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_BrkIntEn(uint32_t brkit)
{
	PWMG->BRKIE |= brkit;
}

/*******************************************************************************************************************************
* @brief	PWM brake interrupt disable
* @param	brkit is brake interrupt type, can be PWM_BRKIT_BRK0, PWM_BRKIT_BRK0, PWM_BRKIT_BRK0 and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_BrkIntDis(uint32_t brkit)
{
	PWMG->BRKIE &= ~brkit;
}

/*******************************************************************************************************************************
* @brief	PWM brake interrupt flag clear
* @param	brkit is brake interrupt type, can be PWM_BRKIT_BRK0, PWM_BRKIT_BRK0, PWM_BRKIT_BRK0 and their '|' operation
* @return
*******************************************************************************************************************************/
void PWM_BrkIntClr(uint32_t brkit)
{
	PWMG->BRKIF = brkit;
}

/*******************************************************************************************************************************
* @brief	PWM brake interrupt state query
* @param	brkit is brake interrupt type, can be PWM_BRKIT_BRK0, PWM_BRKIT_BRK0, PWM_BRKIT_BRK0 and their '|' operation
* @return	0 interrupt not happen, !=0 interrupt happened
*******************************************************************************************************************************/
uint32_t PWM_BrkIntStat(uint32_t brkit)
{
	return (PWMG->BRKIF & brkit);
}
