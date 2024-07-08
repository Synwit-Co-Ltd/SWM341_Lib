#ifndef __SWM341_WDT_H__
#define	__SWM341_WDT_H__


void WDT_Init(WDT_TypeDef * WDTx, uint32_t int_period, uint32_t rst_period);
void WDT_ReInit(WDT_TypeDef * WDTx, uint32_t int_period, uint32_t rst_period);
void WDT_Start(WDT_TypeDef * WDTx);
void WDT_Stop(WDT_TypeDef * WDTx);

void WDT_Feed(WDT_TypeDef * WDTx);


void WDT_INTClr(WDT_TypeDef * WDTx);
uint32_t WDT_INTStat(WDT_TypeDef * WDTx);


#endif
