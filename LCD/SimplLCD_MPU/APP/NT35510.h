#ifndef __NT35510_H__
#define __NT35510_H__


#define NT35510_VPIX  800
#define NT35510_HPIX  480



void NT35510_Init(void);
void NT35510_Clear(uint16_t rgb);
void NT35510_DrawPoint(uint16_t x, uint16_t y, uint16_t rgb);

void NT35510_DMAWrite(uint32_t * buff, uint32_t start_line, uint32_t count_line);


#endif
