#ifndef __USBH_CONF_H__
#define __USBH_CONF_H__


#define USBH_DELAY_MAX			1000	// 若每 100us 执行一次 USBH_Process，则设备插入后延时 100ms 再与设备通信

#define USBH_DEVICE_ADDRESS		1

#define USBH_MAX_CFG_SIZE		128		// 支持的最大配置描述符大小
#define USBH_MAX_STR_SIZE		32		// 支持的最大字符串描述符大小

#define USBH_MAX_NUM_INTERFACES 2		// 支持接口个数（CDC需要两个接口）
#define USBH_MAX_NUM_ENDPOINTS  3		// 每个接口支持端口数

#define USBH_HID_REPORT_SIZE	128

#define USBH_CDC_BUFF_SIZE		256


#endif //__USBH_CONF_H__
