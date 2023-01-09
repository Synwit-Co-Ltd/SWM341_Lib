#include "SWM341.h"
#include "winusb_transfer.h"


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
    0x00, 0x01,             // bcdDevice
    0x01,                   // iManufacture
    0x02,                   // iProduct
    0x00,                   // iSerialNumber - no serial
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

    /* I/F descr */
    9,                      // bLength
    USB_DESC_INTERFACE,     // bDescriptorType
    0x00,                   // bInterfaceNumber
    0x00,                   // bAlternateSetting
    0x02,                   // bNumEndpoints
    0xFF,                   // bInterfaceClass
    0x00,                   // bInterfaceSubClass
    0x00,                   // bInterfaceProtocol
    0x00,                   // iInterface

    /* EP Descriptor: bulk in. */
    7,                              // bLength
    USB_DESC_ENDPOINT,              // bDescriptorType
    (USB_EP_IN | EP_BULK_IN_NUM),  	// bEndpointAddress
    USB_EP_BULK,                    // bmAttributes
    EP_BULK_IN_PKSZ, 0x00,         	// wMaxPacketSize
    0,    							// bInterval

    /* EP Descriptor: bulk out. */
    7,                              // bLength
    USB_DESC_ENDPOINT,              // bDescriptorType
    (USB_EP_OUT | EP_BULK_OUT_NUM), // bEndpointAddress
    USB_EP_BULK,                    // bmAttributes
    EP_BULK_OUT_PKSZ, 0x00,         // wMaxPacketSize
    0     							// bInterval
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
    'S', 0, 'y', 0, 'n', 0, 'w', 0, 'i', 0, 't', 0,
};

uint8_t ProductStringDesc[] =
{
    32,
    USB_DESC_STRING,
    'W', 0, 'I', 0, 'N', 0, 'U', 0, 'S', 0, 'B', 0, ' ', 0, 'T', 0, 'r', 0, 'a', 0, 'n', 0, 's', 0, 'f', 0, 'e', 0, 'r', 0
};

uint8_t *USBD_StringDescriptor[4] =
{
    LangStringDesc,
    VendorStringDesc,
    ProductStringDesc,
    NULL,
};

uint8_t USBD_MSOSStringDesc[] =
{
    18,
    USB_DESC_STRING,
    'M', 0, 'S', 0, 'F', 0, 'T', 0, '1', 0, '0', 0, '0', 0, WINUSB_VENDOR_CODE, 0
};

uint8_t USBD_CompatibleIDDesc[] = 
{
	0x28, 0x00, 0x00, 0x00,           					// dwLength
	0x00, 0x01,                             			// bcdVersion
	0x04, 0x00,                             			// wIndex
	0x01,                                       		// bCount, number of sections
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,       	// Reserved
	0x00,                   							// bInterfaceNumber
	0x01,                  								// Reserved
	'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00, 			// compactiableID
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 	// subCompactiableID
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00   				// Reserved
};
