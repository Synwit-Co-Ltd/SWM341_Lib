#ifndef __USBH_CONF_H__
#define __USBH_CONF_H__


#define USBH_DEVICE_ADDRESS		1

#define USBH_MAX_CFG_SIZE		256		// Maximum supported configuration descriptor size
#define USBH_MAX_STR_SIZE		64		// Maximum supported string descriptor size

#define USBH_MAX_NUM_INTERFACES 3		// Number of interfaces supported (CDC requires two interfaces, gaming mouse may have three interfaces)
#define USBH_MAX_NUM_ENDPOINTS  2		// Number of endpoints supported by each interface

#define USBH_HID_REPORT_SIZE	256		// HID report descriptor size

#define USBH_CDC_BUFF_SIZE		256


#endif
