#ifndef __HID_TRANSFER_H__
#define __HID_TRANSFER_H__

#include <stdint.h>

#define USBD_VID	0x0416
#define USBD_PID    0x5020


#define EP_INT_IN_NUM		1
#define EP_INT_OUT_NUM		1

#define EP_CTRL_PKSZ		32
#define EP_INT_IN_PKSZ		32
#define EP_INT_OUT_PKSZ		32


/* Define Descriptor information */
#define USBD_SELF_POWERED  	0
#define USBD_REMOTE_WAKEUP  0
#define USBD_MAX_POWER      50  // 50 * 2mA = 100mA


void HID_Init(void);
void HID_ClassRequest(USB_Setup_Packet_t * pSetup);

void HID_SetInReport(void);
void HID_GetOutReport(uint8_t *buf, uint32_t u32Size);


#endif
