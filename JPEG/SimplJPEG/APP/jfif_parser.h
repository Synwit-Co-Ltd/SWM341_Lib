#ifndef __JFIF_PARSER_H__
#define __JFIF_PARSER_H__


#define JFIF_RES_OK			0
#define JFIF_FORMAT_ERR		1
#define JFIF_NO_SUPPORT		2


#define JFIF_MKR_SOF0 		0xFFC0		// Start Of Frame (SOF0 for baseline DCT)
#define JFIF_MKR_DHT  		0xFFC4		// Define Huffman Table
#define JFIF_MKR_SOI  		0xFFD8		// Start Of Image
#define JFIF_MKR_EOI  		0xFFD9		// End Of Image
#define JFIF_MKR_SOS  		0xFFDA		// Start Of Scan
#define JFIF_MKR_DQT  		0xFFDB		// Define Quantization Table
#define JFIF_MKR_DRI  		0xFFDD		// Define restart interval
#define JFIF_MKR_APP0 		0xFFE0		// Application 0
#define JFIF_MKR_APP1 		0xFFE1
#define JFIF_MKR_APP2 		0xFFE2
#define JFIF_MKR_APP3 		0xFFE3
#define JFIF_MKR_APP4 		0xFFE4
#define JFIF_MKR_APP5 		0xFFE5
#define JFIF_MKR_APP6 		0xFFE6
#define JFIF_MKR_APP7 		0xFFE7
#define JFIF_MKR_APP8 		0xFFE8
#define JFIF_MKR_APP9 		0xFFE9
#define JFIF_MKR_APPa 		0xFFEa
#define JFIF_MKR_APPb 		0xFFEb
#define JFIF_MKR_APPc		0xFFEc
#define JFIF_MKR_APPd 		0xFFEd
#define JFIF_MKR_APPe 		0xFFEe
#define JFIF_MKR_APPf		0xFFEf
#define JFIF_MKR_COM  		0xFFFE		// Comment


int jfif_parse(const uint8_t buf[], uint32_t len, jfif_info_t * jfif_info);


#endif //__JFIF_PARSER_H__
