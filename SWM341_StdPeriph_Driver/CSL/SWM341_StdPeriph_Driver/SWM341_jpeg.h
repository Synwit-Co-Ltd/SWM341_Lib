#ifndef __SWM341_JPEG_H__
#define __SWM341_JPEG_H__

#include <stdint.h>


typedef struct {
	uint8_t  DoneIEn;			// decode done interrupt enable
	uint8_t  ErrorIEn;			// decode error interrupt enable
} JPEG_InitStructure;


typedef struct {
	uint8_t  format;			// decode output format, can be JPEG_OUT_YUV, JPEG_OUT_YUVsp, JPEG_OUT_XRGB888, ...
	
	/* for RGB output */
	uint8_t  dither;			// RGB565 dithering enable
	uint32_t RGBAddr;
	uint32_t RGBWidth;			// RGB Line Width, usually assigned to jfif_info.Width, i.e. the width of the image
								// If the picture is narrower than the screen, but you want to decode the picture directly to graphic memory, it must be assigned to the screen width
	
	/* for YUV output */
	uint32_t YAddr;
	uint32_t CbAddr;
	uint32_t CrAddr;
} jpeg_outset_t;				// output setting


#define JPEG_OUT_YUV 		0
#define JPEG_OUT_YUVsp		1	// semi-planar
#define JPEG_OUT_XRGB888 	2
#define JPEG_OUT_RGB888 	3
#define JPEG_OUT_RGB565		4
#define JPEG_OUT_YVU 		(0 | (1 << 4))
#define JPEG_OUT_YVUsp		(1 | (1 << 4))
#define JPEG_OUT_XBGR888 	(2 | (1 << 5))
#define JPEG_OUT_BGR888 	(3 | (1 << 5))
#define JPEG_OUT_BGR565		(4 | (1 << 5))
#define JPEG_OUT_BGRX888 	(2 | (1 << 6))
#define JPEG_OUT_RGBX888 	(2 | (1 << 5) | (1 << 6))


#define JPEG_FMT_H2V2   	0	// YUV420
#define JPEG_FMT_H2V1   	1	// YUV422
#define JPEG_FMT_H1V1   	2	// YUV444



#define JFIF_QTAB_MAX   3
#define JFIF_HTAB_MAX   2

typedef struct {
	uint16_t Width;
	uint16_t Height;
	
	struct {
		uint8_t id;				// 1: Y   2: Cb   3: Cr
		uint8_t hfactor;		// Color component horizontal sampling factor
		uint8_t vfactor;		// Color component vertical sampling factor
		uint8_t qtab_id;		// Quantization table used for color components
		uint8_t htab_id_dc;		// Hoffmann table used for DC components
		uint8_t htab_id_ac;		// Hoffmann table used for AC components
	} CompInfo[3];				// Component Information
	uint8_t CompCnt;
	
	uint8_t QTable[JFIF_QTAB_MAX][64];
	uint8_t QTableCnt;
	
	struct {
		struct {
			uint16_t codeWord[16];
			uint8_t  codeLen[16];
			uint8_t  codeVal[16];
		} DC;
		
		struct {
			uint16_t codeWord[162];
			uint8_t  codeLen[162];
			uint8_t  codeVal[162];
		} AC;
	} HTable[JFIF_HTAB_MAX];
	uint8_t HTableCnt;
	
	uint32_t CodeAddr;			// data to decode
	uint32_t CodeLen;
} jfif_info_t;



void JPEG_Init(JPEG_TypeDef * JPEGx, JPEG_InitStructure * initStruct);
void JPEG_Decode(JPEG_TypeDef * JPEGx, jfif_info_t * jfif_info, jpeg_outset_t * jpeg_outset);
uint32_t JPEG_DecodeBusy(JPEG_TypeDef * JPEGx);


#endif
