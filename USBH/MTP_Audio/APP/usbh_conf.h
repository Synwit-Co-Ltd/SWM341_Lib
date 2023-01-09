#ifndef __USBH_CONF_H__
#define __USBH_CONF_H__


#define USBH_DELAY_MAX			1000	// ��ÿ 100us ִ��һ�� USBH_Process�����豸�������ʱ 100ms �����豸ͨ��

#define USBH_DEVICE_ADDRESS		1

#define USBH_MAX_CFG_SIZE		128		// ֧�ֵ����������������С
#define USBH_MAX_STR_SIZE		32		// ֧�ֵ�����ַ�����������С

#define USBH_MAX_NUM_INTERFACES 2		// ֧�ֽӿڸ�����CDC��Ҫ�����ӿڣ�
#define USBH_MAX_NUM_ENDPOINTS  3		// ÿ���ӿ�֧�ֶ˿���

#define USBH_HID_REPORT_SIZE	128

#define USBH_CDC_BUFF_SIZE		256


#endif //__USBH_CONF_H__
