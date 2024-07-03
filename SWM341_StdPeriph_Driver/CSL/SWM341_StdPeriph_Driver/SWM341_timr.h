#ifndef __SWM341_TIMR_H__
#define __SWM341_TIMR_H__


#define TIMR_MODE_TIMER		((0 << 2) | 0)
#define TIMR_MODE_IC		((1 << 2) | 0)		// Input Capture
#define TIMR_MODE_OC		((2 << 2) | 0)		// Output Compare
#define TIMR_MODE_COUNTER	((0 << 2) | 2)		// rising edge Counter


/* Interrupt Type */
#define TIMR_IT_TO			TIMR_IE_TO_Msk		// timeout
#define TIMR_IT_OC			TIMR_IE_OC0_Msk		// output compare match
#define TIMR_IT_IC_LOW		TIMR_IE_ICR_Msk		// input capture low level
#define TIMR_IT_IC_HIGH		TIMR_IE_ICF_Msk		// input capture high level


void TIMR_Init(TIMR_TypeDef * TIMRx, uint32_t mode, uint16_t prediv, uint32_t period, uint32_t int_en);
void TIMR_Start(TIMR_TypeDef * TIMRx);
void TIMR_Stop(TIMR_TypeDef * TIMRx);
void TIMR_Halt(TIMR_TypeDef * TIMRx);
void TIMR_Resume(TIMR_TypeDef * TIMRx);

uint32_t TIMR_GetCurValue(TIMR_TypeDef * TIMRx);

void TIMR_INTEn(TIMR_TypeDef * TIMRx, uint32_t it);
void TIMR_INTDis(TIMR_TypeDef * TIMRx, uint32_t it);
void TIMR_INTClr(TIMR_TypeDef * TIMRx, uint32_t it);
uint32_t TIMR_INTStat(TIMR_TypeDef * TIMRx, uint32_t it);


void TIMR_OC_Init(TIMR_TypeDef * TIMRx, uint32_t match, uint32_t match_int_en, uint32_t init_lvl);
void TIMR_OC_OutputEn(TIMR_TypeDef * TIMRx);
void TIMR_OC_OutputDis(TIMR_TypeDef * TIMRx, uint32_t level);
void TIMR_OC_SetMatch(TIMR_TypeDef * TIMRx, uint32_t match);
uint32_t TIMR_OC_GetMatch(TIMR_TypeDef * TIMRx);


void TIMR_IC_Init(TIMR_TypeDef * TIMRx, uint32_t captureH_int_en, uint32_t captureL_int_en);
uint32_t TIMR_IC_GetCaptureH(TIMR_TypeDef * TIMRx);
uint32_t TIMR_IC_GetCaptureL(TIMR_TypeDef * TIMRx);


#endif
