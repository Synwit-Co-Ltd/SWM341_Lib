#ifndef __SWM341_QEI_H__
#define __SWM341_QEI_H__


typedef struct {
	uint8_t  mode;					// QEI_MODE_X2, QEI_MODE_X4
	uint16_t maxcnt;				// maximum count value
	uint8_t  swapAB;				// =1 swap pin A and B
	uint8_t  intINDEXEn;			// index pulse detected interrupt enable
	uint8_t  intMATCHEn;			// POSCNT increments to equal MAXCNT, or POSCNT decrement from MAXCNT to 0 interrupt enable
	uint8_t  intCNTOVEn;			// Counter Overrun interrupt enable
	uint8_t  intERROREn;			// Counter Error interrupt enable
} QEI_InitStructure;


#define QEI_MODE_X2			0
#define QEI_MODE_X4			1

/* interrupt type */
#define QEI_IT_INDEX		(1 << 0)
#define QEI_IT_MATCH		(1 << 1)
#define QEI_IT_CNTOV		(1 << 2)
#define QEI_IT_ERROR		(1 << 3)


void QEI_Init(QEI_TypeDef * QEIx, QEI_InitStructure * initStruct);
void QEI_Start(QEI_TypeDef * QEIx);
void QEI_Stop(QEI_TypeDef * QEIx);

uint32_t QEI_IndexLvl(QEI_TypeDef * QEIx);
uint32_t QEI_CountDir(QEI_TypeDef * QEIx);

void QEI_IntEn(QEI_TypeDef * QEIx, uint32_t it);
void QEI_IntDis(QEI_TypeDef * QEIx, uint32_t it);
void QEI_IntClr(QEI_TypeDef * QEIx, uint32_t it);
uint32_t QEI_IntStat(QEI_TypeDef * QEIx, uint32_t it);


#endif
