#include "SWM341.h"
#include "usbh_core.h"


void USBH_USR_Init(void)
{
}

void USBH_USR_DeInit(void)
{
}

void USBH_USR_DeviceAttached(void)
{
	printf("USB Device Attached\n");
}

void USBH_USR_Device_DescAvailable(USB_DevDesc_t *devDesc)
{
	printf("idVendor: %04X\n", devDesc->idVendor);
	printf("idProduct: %04X\n", devDesc->idProduct);
}

void USBH_USR_Config_DescAvailable(USB_CfgDesc_t * cfgDesc, USB_IntfDesc_t *intfDesc, USB_EpDesc_t *epDesc)
{
}

void USBH_USR_Vendor_String(char *strVendor)
{
}

void USBH_USR_Product_String(char *strProduct)
{
}

void USBH_USR_SerialNum_String(char *strSerialNum)
{
}

void USBH_USR_EnumerationDone(void)
{
	printf("Enumeration Done\n");
} 

void USBH_USR_DeviceNotSupported(void)
{
	printf("Device Not Supported\n");
}  

void USBH_USR_DeviceDisconnected(void)
{
}

uint32_t USBH_USR_UserInput(void)
{
	return 1;
}

int USBH_USR_Application(void)
{	
	return 0;
}


USBH_User_cb_t  USBH_USR_cb = {
	.Init = USBH_USR_Init,
	.DeInit = USBH_USR_DeInit,
	.DeviceAttached = USBH_USR_DeviceAttached,
	.DeviceDescAvailable = USBH_USR_Device_DescAvailable,
	.ConfigDescAvailable = USBH_USR_Config_DescAvailable,
	.VendorString = USBH_USR_Vendor_String,
	.ProductString = USBH_USR_Product_String,
	.SerialNumString = USBH_USR_SerialNum_String,
	.EnumerationDone = USBH_USR_EnumerationDone,
	.UserInput = USBH_USR_UserInput,
	.UserApplication = USBH_USR_Application,
	.DeviceNotSupported = USBH_USR_DeviceNotSupported,
	.DeviceDisconnected = USBH_USR_DeviceDisconnected
};
