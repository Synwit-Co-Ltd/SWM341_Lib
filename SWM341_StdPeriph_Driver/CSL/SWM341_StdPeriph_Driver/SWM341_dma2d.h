#ifndef __SWM341_DMA2D_H__
#define __SWM341_DMA2D_H__

#include <stdint.h>


typedef struct {
	uint16_t Interval;			// data block (64 word) transfer interval in unit of HCLK period, can be 1--1023
	uint8_t  IntEOTEn;			// End of Transter interrupt enable
} DMA2D_InitStructure;


typedef struct {
	uint32_t Address;
	uint32_t LineOffset;		// added at the end of each line to determine the starting address of the next line
	uint8_t  ColorMode;			// DMA2D_FMT_ARGB888, DMA2D_FMT_RGB888, DMA2D_FMT_RGB565, ...
	uint8_t  AlphaMode;			// DMA2D_AMODE_PIXEL, DMA2D_AMODE_ALPHA, DMA2D_AMODE_PMULA, ...
	uint8_t  Alpha;
	
	/* only for DMA2D_LAYER_OUT, not for DMA2D_LAYER_FG and DMA2D_LAYER_BG */
	uint16_t LineCount;			// line per screen
	uint16_t LinePixel;			// pixel per line
} DMA2D_LayerSetting;


#define DMA2D_LAYER_FG		0	// Foreground layer
#define DMA2D_LAYER_BG		1	// Background layer
#define DMA2D_LAYER_OUT		2	// Output layer

/* Color Format */
#define DMA2D_FMT_ARGB888   (0 | (0 << 4))
#define DMA2D_FMT_RGB888    (1 | (0 << 4))
#define DMA2D_FMT_RGB565    (2 | (0 << 4))
#define DMA2D_FMT_ABGR888   (0 | (1 << 4))
#define DMA2D_FMT_BGR888    (1 | (1 << 4))
#define DMA2D_FMT_BGR565    (2 | (1 << 4))

/* Alpha Mode */
#define DMA2D_AMODE_PIXEL	(0 | (0 << 5))	// use the pixel's own Alpha value
#define DMA2D_AMODE_ALPHA	(0 | (1 << 5))	// use the Alpha value specified by software
#define DMA2D_AMODE_PMULA	(0 | (2 << 5))	// use the product of the pixel's own Alpha value and the Alpha value specified by software


void DMA2D_Init(DMA2D_InitStructure * initStruct);
void DMA2D_PixelFill(DMA2D_LayerSetting * outLayer, uint32_t color);
void DMA2D_PixelMove(DMA2D_LayerSetting * fgLayer, DMA2D_LayerSetting * outLayer);
void DMA2D_PixelConvert(DMA2D_LayerSetting * fgLayer, DMA2D_LayerSetting * outLayer);
void DMA2D_PixelBlend(DMA2D_LayerSetting * fgLayer, DMA2D_LayerSetting * bgLayer, DMA2D_LayerSetting * outLayer);
uint32_t DMA2D_IsBusy(void);

void DMA2D_INTEn(void);
void DMA2D_INTDis(void);
void DMA2D_INTClr(void);
uint32_t DMA2D_INTStat(void);


#endif
