#ifndef __SWM341_RTC_H__
#define __SWM341_RTC_H__


#define RTC_CLKSRC_LRC32K	0
#define RTC_CLKSRC_XTAL32K	1

#define RTC_SUN   0x01
#define RTC_MON   0x02
#define RTC_TUE   0x04
#define RTC_WED   0x08
#define RTC_THU   0x10
#define RTC_FRI   0x20
#define RTC_SAT   0x40


typedef struct {
	uint8_t  clksrc;		//RTC_CLKSRC_RC32K��RTC_CLKSRC_XTAL32K
	uint16_t Year;
	uint8_t  Month;			//ȡֵ1--12
	uint8_t  Date;			//ȡֵ1--31
	uint8_t  Hour;			//ȡֵ0--23
	uint8_t  Minute;		//ȡֵ0--59
	uint8_t  Second;		//ȡֵ0--59
	uint8_t  SecondIEn;
	uint8_t  MinuteIEn;
} RTC_InitStructure;

typedef struct {
	uint8_t  Days;			//RTC_SUN��RTC_MON��RTC_TUE��RTC_WED��RTC_THU��RTC_FRI��RTC_SAT������������
	uint8_t  Hour;
	uint8_t  Minute;
	uint8_t  Second;
	uint8_t  AlarmIEn;
} RTC_AlarmStructure;

typedef struct {
	uint16_t Year;
	uint8_t  Month;
	uint8_t  Date;
	uint8_t  Day;			//RTC_SUN��RTC_MON��RTC_TUE��RTC_WED��RTC_THU��RTC_FRI��RTC_SAT
	uint8_t  Hour;
	uint8_t  Minute;
	uint8_t  Second;
} RTC_DateTime;


/* Interrupt Type */
#define RTC_IT_SECOND		(1 << 0)	//Second Interrupt
#define RTC_IT_MINUTE		(1 << 1)
#define RTC_IT_HOUR			(1 << 2)
#define RTC_IT_DATE			(1 << 3)
#define RTC_IT_ALARM		(1 << 4)
#define RTC_IT_SECOND_DIV2	(1 << 6)	//1/2 Second Interrupt
#define RTC_IT_SECOND_DIV4	(1 << 7)	//1/4 Second Interrupt



void RTC_Init(RTC_TypeDef * RTCx, RTC_InitStructure * initStruct);
void RTC_Start(RTC_TypeDef * RTCx);
void RTC_Stop(RTC_TypeDef * RTCx);

void RTC_GetDateTime(RTC_TypeDef * RTCx, RTC_DateTime * dateTime);

void RTC_AlarmSetup(RTC_TypeDef * RTCx, RTC_AlarmStructure * alarmStruct);


void RTC_INTEn(RTC_TypeDef * RTCx, uint32_t it);
void RTC_INTDis(RTC_TypeDef * RTCx, uint32_t it);
void RTC_INTClr(RTC_TypeDef * RTCx, uint32_t it);
uint32_t RTC_INTStat(RTC_TypeDef * RTCx, uint32_t it);

#endif //__SWM341_RTC_H__
