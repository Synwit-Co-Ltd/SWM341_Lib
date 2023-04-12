#ifndef __USBH_CONF_H__
#define __USBH_CONF_H__


#define USBH_DEVICE_ADDRESS		1

#define USBH_MAX_CFG_SIZE		256		// 支持的最大配置描述符大小
#define USBH_MAX_STR_SIZE		64		// 支持的最大字符串描述符大小

#define USBH_MAX_NUM_INTERFACES 3		// 支持接口个数（CDC需要两个接口，游戏鼠标可能有三个接口）
#define USBH_MAX_NUM_ENDPOINTS  2		// 每个接口支持端口数

#define USBH_HID_REPORT_SIZE	256		// HID 报告描述符大小

#define USBH_CDC_BUFF_SIZE		256


#endif //__USBH_CONF_H__
