#include "SWM341.h"
#include "MassStorage.h"


uint8_t USBD_DeviceDescriptor[] =
{
    18,                     // bLength
    USB_DESC_DEVICE,        // bDescriptorType
    0x00, 0x02,             // bcdUSB
    0x00,                   // bDeviceClass
    0x00,                   // bDeviceSubClass
    0x00,                   // bDeviceProtocol
    EP_CTRL_PKSZ,       	// bMaxPacketSize0
    USBD_VID & 0xFF, ((USBD_VID >> 8) & 0xFF),  // idVendor
    USBD_PID & 0xFF, ((USBD_PID >> 8) & 0xFF),  // idProduct
    0x00, 0x00,             // bcdDevice
    0x01,                   // iManufacture
    0x02,                   // iProduct
    0x03,                   // iSerialNumber
    0x01                    // bNumConfigurations
};


uint8_t USBD_ConfigDescriptor[] =
{
    9,                      // bLength
    USB_DESC_CONFIG,        // bDescriptorType
    9+9+7+7, 0,           	// wTotalLength
    0x01,                   // bNumInterfaces
    0x01,                   // bConfigurationValue
    0x00,                   // iConfiguration
    0x80 | (USBD_SELF_POWERED << 6) | (USBD_REMOTE_WAKEUP << 5),    // bmAttributes
    USBD_MAX_POWER,         // MaxPower

    /* I/F Descriptor */
    9,                      // bLength
    USB_DESC_INTERFACE,     // bDescriptorType
    0x00,                   // bInterfaceNumber
    0x00,                   // bAlternateSetting
    0x02,                   // bNumEndpoints
    0x08,                   // bInterfaceClass MSC
    0x06,                   // bInterfaceSubClass SCSI
    0x50,                   // bInterfaceProtocol BBB
    0x00,                   // iInterface

    /* EP Descriptor: interrupt in. */
    7,                              // bLength
    USB_DESC_ENDPOINT,              // bDescriptorType
    (USB_EP_IN | EP_BULK_IN_NUM),  	// bEndpointAddress
    USB_EP_BULK,                    // bmAttributes
    EP_BULK_IN_PKSZ, 0x00,         	// wMaxPacketSize
    0x00,    						// bInterval

    /* EP Descriptor: interrupt out. */
    7,                              // bLength
    USB_DESC_ENDPOINT,              // bDescriptorType
    (USB_EP_OUT | EP_BULK_OUT_NUM), // bEndpointAddress
    USB_EP_BULK,                    // bmAttributes
    EP_BULK_OUT_PKSZ, 0x00,         // wMaxPacketSize
    0x00,    						// bInterval
};


uint8_t LangStringDesc[4] =
{
    4,                      // bLength
    USB_DESC_STRING,        // bDescriptorType
    0x09, 0x04
};

uint8_t VendorStringDesc[] =
{
    14,
    USB_DESC_STRING,
    'S', 0, 'y', 0, 'n', 0, 'w', 0, 'i', 0, 't', 0
};

uint8_t ProductStringDesc[] =
{
    26,
    USB_DESC_STRING,
    'M', 0, 'a', 0, 's', 0, 's', 0, ' ', 0, 'S', 0, 't', 0, 'o', 0, 'r', 0, 'a', 0, 'g', 0, 'e', 0
};

uint8_t SerialStringDesc[] =
{
    26,
    USB_DESC_STRING,
    'A', 0, '0', 0, '0', 0, '0', 0, '0', 0, '8', 0, '0', 0, '4', 0, '0', 0, '1', 0, '1', 0, '5', 0

};

uint8_t *USBD_StringDescriptor[5] =
{
    LangStringDesc,
    VendorStringDesc,
    ProductStringDesc,
	SerialStringDesc,
    NULL,
};
