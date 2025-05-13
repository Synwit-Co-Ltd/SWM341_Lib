#ifndef __HID_TRANSFER_H__
#define __HID_TRANSFER_H__

#include <stdint.h>


#define EP_INT_IN_NUM		2
#define EP_INT_OUT_NUM		2

#define EP_INT_IN_PKSZ		64
#define EP_INT_OUT_PKSZ		64


void HID_SetInReport(void);
void HID_GetOutReport(uint8_t *buf, uint32_t u32Size);


#endif
