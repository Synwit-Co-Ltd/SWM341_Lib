#ifndef __WINUSB_TRANSFER_H__
#define __WINUSB_TRANSFER_H__

#include <stdint.h>

#define USBD_VID	0x0417
#define USBD_PID    0x7477

#define WINUSB_VENDOR_CODE	0x34


#define EP_BULK_IN_NUM		1
#define EP_BULK_OUT_NUM		1

#define EP_CTRL_PKSZ		64
#define EP_BULK_IN_PKSZ		64
#define EP_BULK_OUT_PKSZ	64


/* Define Descriptor information */
#define USBD_SELF_POWERED   0
#define USBD_REMOTE_WAKEUP  0
#define USBD_MAX_POWER   	50  // 50 * 2mA = 100mA


void WINUSB_Init(void);
void WINUSB_VendorRequest(USB_Setup_Packet_t * pSetup);

void WINUSB_SetInReport(void);
void WINUSB_GetOutReport(uint8_t *buf, uint32_t u32Size);


#endif
