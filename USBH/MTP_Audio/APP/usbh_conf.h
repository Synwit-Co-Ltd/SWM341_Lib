#ifndef __USBH_CONF_H__
#define __USBH_CONF_H__


#define USBH_DELAY_MAX			1000	// If the USBH_Process is executed every 100us, the device communicates with the device after being inserted 100ms later

#define USBH_DEVICE_ADDRESS		1

#define USBH_MAX_CFG_SIZE		128		// Maximum supported configuration descriptor size
#define USBH_MAX_STR_SIZE		32		// Maximum supported string descriptor size

#define USBH_MAX_NUM_INTERFACES 2		// Number of interfaces supported (CDC requires two interfaces)
#define USBH_MAX_NUM_ENDPOINTS  3		// Number of endpoints supported by each interface

#define USBH_HID_REPORT_SIZE	128

#define USBH_CDC_BUFF_SIZE		256


#endif
