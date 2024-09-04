#ifndef __SWM341_LCD_H__
#define __SWM341_LCD_H__


typedef struct {
	uint8_t  ClkDiv;		// 2--65
	uint8_t  Format;		// LCD_FMT_RGB565, LCD_FMT_RGB888, LCD_FMT_SRGB565, LCD_FMT_SRGB888
	uint16_t HnPixel;		// Number of horizontal pixels, up to 1024
	uint16_t VnPixel;		// Number of vertical pixels, up to 1024
	uint8_t  Hfp;			// horizonal front porch, up to  64
	uint16_t Hbp;			// horizonal back porch,  up to 256
	uint8_t  Vfp;			// vertical front porch,  up to  64
	uint16_t Vbp;			// vertical back porch,   up to 256
	uint16_t HsyncWidth;	// up to 256
	uint16_t VsyncWidth;	// up to 256
	
	uint32_t DataSource;	// display data address
	uint32_t Background;	// background color
	uint8_t  SampleEdge;	// sample clock edge: LCD_SAMPLE_RISE, LCD_SAMPLE_FALL
	
	uint8_t  IntEOTEn;		// End of Transter interrupt enable
} LCD_InitStructure;


typedef struct {
	uint8_t  Alpha;
	uint16_t HStart;		// Horizontal starting position: 0 ~ HnPixel-1
	uint16_t HStop;			// Horizontal termination position (included): cannot less than HStart, and HStop - HStart must be odd number
	uint16_t VStart;
	uint16_t VStop;
	uint32_t DataSource;	// display data address
} LCD_LayerInitStructure;


#define LCD_FMT_RGB565	0
#define LCD_FMT_RGB888	1
#define LCD_FMT_SRGB565	2	// Serial RGB
#define LCD_FMT_SRGB888	3

#define LCD_SAMPLE_RISE	0	// sample data at rising edge of DOTCLK
#define LCD_SAMPLE_FALL	1	// sample data at falling edge of DOTCLK

#define LCD_LAYER_1	  	0
#define LCD_LAYER_2     1


void LCD_Init(LCD_TypeDef * LCDx, LCD_InitStructure * initStruct);
void LCD_LayerInit(LCD_TypeDef * LCDx, uint32_t layerx, LCD_LayerInitStructure * initStruct);
void LCD_SetLayerPos(LCD_TypeDef * LCDx, uint32_t layerx, uint16_t hstart, uint16_t hstop, uint16_t vstart, uint16_t vstop);
void LCD_Start(LCD_TypeDef * LCDx);
void LCD_Stop(LCD_TypeDef * LCDx);
void LCD_ReStart(LCD_TypeDef * LCDx);
uint32_t LCD_IsBusy(LCD_TypeDef * LCDx);

void LCD_INTEn(LCD_TypeDef * LCDx);
void LCD_INTDis(LCD_TypeDef * LCDx);
void LCD_INTClr(LCD_TypeDef * LCDx);
uint32_t LCD_INTStat(LCD_TypeDef * LCDx);



typedef struct {
	uint8_t  RDHoldTime;	// LCD_RD Low level holding time, can be 1--32
	uint8_t  WRHoldTime;	// LCD_WR Low level holding time, can be 1--16
	uint8_t  CSFall_WRFall;	// LCD_CS falling edge to LCD_WR falling edge delay, can be 1--4
	uint8_t  WRRise_CSRise;	// LCD_WR rising edge to LCD_CS rising edge delay, can be 1--4
	uint8_t  RDCSRise_Fall;	// for read, LCD_CS rising edge to falling edge delay, can be 1--32
	uint8_t  WRCSRise_Fall;	// for write, LCD_CS rising edge to falling edge delay, can be 1--16
} MPULCD_InitStructure;


void MPULCD_Init(LCD_TypeDef * LCDx, MPULCD_InitStructure * initStruct);

void LCD_WR_REG(LCD_TypeDef * LCDx, uint16_t reg);
void LCD_WR_DATA(LCD_TypeDef * LCDx, uint16_t val);
void LCD_WriteReg(LCD_TypeDef * LCDx, uint16_t reg, uint16_t val);
uint16_t LCD_ReadReg(LCD_TypeDef * LCDx, uint16_t reg);

void MPULCD_DMAStart(LCD_TypeDef * LCDx, uint32_t * buff, uint16_t hpix, uint16_t vpix);
uint32_t MPULCD_DMABusy(LCD_TypeDef * LCDx);


#endif
