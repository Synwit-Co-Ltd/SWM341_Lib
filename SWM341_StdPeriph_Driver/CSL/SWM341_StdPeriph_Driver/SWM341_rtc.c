/*******************************************************************************************************************************
* @brief	RTC driver
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
#include "SWM341_rtc.h"


static uint32_t calcWeekDay(uint32_t year, uint32_t month, uint32_t date);


/*******************************************************************************************************************************
* @brief	RTC init
* @param	RTCx is the RTC to init
* @param	initStruct is data used to init the RTC
* @return
*******************************************************************************************************************************/
void RTC_Init(RTC_TypeDef * RTCx, RTC_InitStructure * initStruct)
{		
	if(initStruct->clksrc == RTC_CLKSRC_XTAL32K)
	{
		SYS->XTALCR |= (1 << SYS_XTALCR_32KON_Pos) | (7 << SYS_XTALCR_32KDRV_Pos);
	}
	else
	{
		SYS->LRCCR = (1 << SYS_LRCCR_ON_Pos);
	}
	SYS->CLKSEL &= ~SYS_CLKSEL_RTC_Msk;
	SYS->CLKSEL |= (initStruct->clksrc << SYS_CLKSEL_RTC_Pos);
	
	SYS->CLKEN1 |= (1 << SYS_CLKEN1_RTC_Pos);
	
	RTC_Stop(RTCx);
	
	while(RTCx->CFGABLE == 0);
	
	RTCx->MINSEC = (initStruct->Second << RTC_MINSEC_SEC_Pos) |
				   (initStruct->Minute << RTC_MINSEC_MIN_Pos);
	
	RTCx->DATHUR = (initStruct->Hour << RTC_DATHUR_HOUR_Pos) |
				   (initStruct->Date << RTC_DATHUR_DATE_Pos);
	
	RTCx->MONDAY = (calcWeekDay(initStruct->Year, initStruct->Month, initStruct->Date) << RTC_MONDAY_DAY_Pos) |
				   (initStruct->Month << RTC_MONDAY_MON_Pos);
	
	RTCx->YEAR = initStruct->Year;
	
	RTCx->LOAD = (1 << RTC_LOAD_TIME_Pos);
	
	RTCx->IF = 0x1F;
	RTCx->IE = (initStruct->SecondIEn << RTC_IE_SEC_Pos) |
			   (initStruct->MinuteIEn << RTC_IE_MIN_Pos);
	
	if(initStruct->SecondIEn | initStruct->MinuteIEn)
	{
		NVIC_EnableIRQ(RTC_IRQn);
	}
	else
	{
		NVIC_DisableIRQ(RTC_IRQn);
	}
}

/*******************************************************************************************************************************
* @brief	RTC start
* @param	RTCx is the RTC to start
* @return
*******************************************************************************************************************************/
void RTC_Start(RTC_TypeDef * RTCx)
{
	RTCx->EN = 1;
}

/*******************************************************************************************************************************
* @brief	RTC stop
* @param	RTCx is the RTC to stop
* @return
*******************************************************************************************************************************/
void RTC_Stop(RTC_TypeDef * RTCx)
{
	RTCx->EN = 0;
}

/*******************************************************************************************************************************
* @brief	get current time and date
* @param	RTCx is the RTC to query
* @param	dateTime is used to save obtained time and date
* @return
*******************************************************************************************************************************/
void RTC_GetDateTime(RTC_TypeDef * RTCx, RTC_DateTime * dateTime)
{
	dateTime->Year = RTCx->YEAR;
	dateTime->Month = (RTCx->MONDAY & RTC_MONDAY_MON_Msk) >> RTC_MONDAY_MON_Pos;
	dateTime->Date = (RTCx->DATHUR & RTC_DATHUR_DATE_Msk) >> RTC_DATHUR_DATE_Pos;
	dateTime->Day = 1 << ((RTCx->MONDAY & RTC_MONDAY_DAY_Msk) >> RTC_MONDAY_DAY_Pos);
	dateTime->Hour = (RTCx->DATHUR & RTC_DATHUR_HOUR_Msk) >> RTC_DATHUR_HOUR_Pos;
	dateTime->Minute = (RTCx->MINSEC & RTC_MINSEC_MIN_Msk) >> RTC_MINSEC_MIN_Pos;
	dateTime->Second = (RTCx->MINSEC & RTC_MINSEC_SEC_Msk) >> RTC_MINSEC_SEC_Pos;
}

/*******************************************************************************************************************************
* @brief	set alarm time
* @param	RTCx is the RTC to set
* @param	alarmStruct is data used to set alarm time
* @return
*******************************************************************************************************************************/
void RTC_AlarmSetup(RTC_TypeDef * RTCx, RTC_AlarmStructure * alarmStruct)
{
	while(RTCx->CFGABLE == 0);
	
	RTCx->MINSECAL = (alarmStruct->Second << RTC_MINSECAL_SEC_Pos) |
					 (alarmStruct->Minute << RTC_MINSECAL_MIN_Pos);
	
	RTCx->DAYHURAL = (alarmStruct->Hour << RTC_DAYHURAL_HOUR_Pos) |
					 (alarmStruct->Days << RTC_DAYHURAL_SUN_Pos);
	
	RTCx->LOAD = (1 << RTC_LOAD_TIME_Pos) | (1 << RTC_LOAD_ALARM_Pos);
	
	RTCx->IF = (1 << RTC_IF_ALARM_Pos);
	RTCx->IE &= ~RTC_IE_ALARM_Msk;
	RTCx->IE |= (alarmStruct->AlarmIEn << RTC_IE_ALARM_Pos);
	
	if(alarmStruct->AlarmIEn)
	{
		NVIC_EnableIRQ(RTC_IRQn);
	}
	else
	{
		NVIC_DisableIRQ(RTC_IRQn);
	}
}

/*******************************************************************************************************************************
* @brief	calculate the day of the week for the specified day
* @param	year, month, date is used to specify the day to calculate
* @param	month
* @param	date
* @return
*******************************************************************************************************************************/
static uint32_t calcWeekDay(uint32_t year, uint32_t month, uint32_t date)
{
    uint32_t i, cnt = 0;
    const uint32_t daysOfMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	for(i = 1; i < month; i++) cnt += daysOfMonth[i];
	
    cnt += date;
	
    if((year%4 == 0) && ((year%100 != 0) || (year%400 == 0)) && (month >= 3)) cnt += 1;
	
    cnt += (year - 1901) * 365;
	
    for(i = 1901; i < year; i++)
    {
        if((i%4 == 0) && ((i%100 != 0) || (i%400 == 0))) cnt += 1;
    }
	
    return (cnt+1) % 7;
}

/*******************************************************************************************************************************
* @brief	RTC interrupt enable
* @param	RTCx is the RTC to set
* @param	it is interrupt type, can be RTC_IT_SECOND, RTC_IT_MINUTE, RTC_IT_HOUR, RTC_IT_DATE, RTC_IT_ALARM, 
*			RTC_IT_SECOND_DIV2, RTC_IT_SECOND_DIV4 and their '|' operation
* @return
*******************************************************************************************************************************/
void RTC_INTEn(RTC_TypeDef * RTCx, uint32_t it)
{
	RTCx->IE |= it;
}

/*******************************************************************************************************************************
* @brief	RTC interrupt disable
* @param	RTCx is the RTC to set
* @param	it is interrupt type, can be RTC_IT_SECOND, RTC_IT_MINUTE, RTC_IT_HOUR, RTC_IT_DATE, RTC_IT_ALARM, 
*			RTC_IT_SECOND_DIV2, RTC_IT_SECOND_DIV4 and their '|' operation
* @return
*******************************************************************************************************************************/
void RTC_INTDis(RTC_TypeDef * RTCx, uint32_t it)
{
	RTCx->IE &= ~it;
}

/*******************************************************************************************************************************
* @brief	RTC interrupt flag clear
* @param	RTCx is the RTC to clear
* @param	it is interrupt type, can be RTC_IT_SECOND, RTC_IT_MINUTE, RTC_IT_HOUR, RTC_IT_DATE, RTC_IT_ALARM, 
*			RTC_IT_SECOND_DIV2, RTC_IT_SECOND_DIV4 and their '|' operation
* @return
*******************************************************************************************************************************/
void RTC_INTClr(RTC_TypeDef * RTCx, uint32_t it)
{
	RTCx->IF = it;
}

/*******************************************************************************************************************************
* @brief	RTC interrupt state query
* @param	RTCx is the RTC to query
* @param	it is interrupt type, can be RTC_IT_SECOND, RTC_IT_MINUTE, RTC_IT_HOUR, RTC_IT_DATE, RTC_IT_ALARM, 
*			RTC_IT_SECOND_DIV2, RTC_IT_SECOND_DIV4 and their '|' operation
* @return	1 interrupt happened, 0 interrupt not happen
*******************************************************************************************************************************/
uint32_t RTC_INTStat(RTC_TypeDef * RTCx, uint32_t it)
{
	return (RTCx->IF & it) ? 1 : 0;
}
