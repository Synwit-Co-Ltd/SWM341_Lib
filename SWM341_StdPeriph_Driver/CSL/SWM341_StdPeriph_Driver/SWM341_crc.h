#ifndef __SWM341_CRC_H__
#define __SWM341_CRC_H__


typedef struct {
	uint32_t init_crc;			// initial value
	uint8_t  Poly;				// CRC polynomial, can be CRC_POLY_11021, CRC_POLY_107, CRC_POLY_18005, CRC_POLY_104C11DB7
	uint8_t  in_width;			// input data width, can be CRC_WIDTH_32, CRC_WIDTH_16, CRC_WIDTH_8
	uint8_t  in_rev;			// input data reverse, can be CRC_REV_NOT, CRC_REV_ALL, CRC_REV_IN_BYTE, CRC_REV_BYTE
	bool     in_not;			// input data inverse
	uint8_t  out_rev;			// output data reverse, can be CRC_REV_NOT, CRC_REV_ALL, CRC_REV_IN_BYTE, CRC_REV_BYTE
	bool     out_not;			// output data inverse
} CRC_InitStructure;


#define CRC_POLY_11021		0	// x^16+x^12+x^5+1
#define CRC_POLY_107		1	// x^8+x^2+x+1
#define CRC_POLY_18005		2	// x^16+x^15+x^2+1
#define CRC_POLY_104C11DB7	3	// x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1

#define CRC_WIDTH_32		0
#define CRC_WIDTH_16		1
#define CRC_WIDTH_8			2

#define CRC_REV_NOT			0	// bit order not change
#define CRC_REV_ALL			1	// bit order completely reverse
#define CRC_REV_IN_BYTE		2	// bit order intra-byte reverse
#define CRC_REV_BYTE		3	// byte order reverse


void CRC_Init(CRC_TypeDef * CRCx, CRC_InitStructure * initStruct);
void CRC_SetInitVal(CRC_TypeDef * CRCx, uint32_t init_crc);


/*******************************************************************************************************************************
* @brief	write data to do CRC
* @param	data is the data to write to CRC module
* @return
*******************************************************************************************************************************/
static __INLINE void CRC_Write(uint32_t data)
{
	CRC->DATAIN = data;
}

/*******************************************************************************************************************************
* @brief	read out CRC calculation result
* @param
* @return	CRC calculation result
*******************************************************************************************************************************/
static __INLINE uint32_t CRC_Result(void)
{
	return CRC->RESULT;
}


#endif
