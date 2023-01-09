#include "SWM341.h"
#include "hid_adc.h"


uint8_t HID_ReportDescriptor[] =
{
	0x06, 0x00, 0xFF,       // Usage Page (Vendor Defined Page 1)
	0x09, 0x01,             // Usage (Vendor Usage 1)
	0xA1, 0x01,             // Collection (Application)
	0x19, 0x01,             // Usage Minimum
	0x29, 0x08,             // Usage Maximum    // 8 input usages total
	0x15, 0x00,             // Logical Minimum (0x00)
	0x26, 0xFF, 0x0F,       // Logical Maximum (0x0FFF = 4095)
	0x75, 0x10,             // Report Size: 16-bit field size
	0x95, 0x08,             // Report Count: 8 16-bit fields
	0x81, 0x00,             // Input (Data, Array, Abs): Instantiates input packet fields based on the above report size, count, logical min/max, and usage.
	0xC0                    // End Collection
};


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
    0x00,                   // iSerialNumber - no serial
    0x01                    // bNumConfigurations
};


uint8_t USBD_ConfigDescriptor[] =
{
    9,                      // bLength
    USB_DESC_CONFIG,        // bDescriptorType
    9+9+9+7, 0,           	// wTotalLength
    0x01,                   // bNumInterfaces
    0x01,                   // bConfigurationValue
    0x00,                   // iConfiguration
    0x80 | (USBD_SELF_POWERED << 6) | (USBD_REMOTE_WAKEUP << 5),    // bmAttributes
    USBD_MAX_POWER,         // MaxPower

    /* I/F descr: HID */
    9,                      // bLength
    USB_DESC_INTERFACE,     // bDescriptorType
    0x00,                   // bInterfaceNumber
    0x00,                   // bAlternateSetting
    0x01,                   // bNumEndpoints
    0x03,                   // bInterfaceClass
    0x00,                   // bInterfaceSubClass
    0x00,                   // bInterfaceProtocol
    0x00,                   // iInterface

    /* HID Descriptor */
    9,                      // bLength
    USB_DESC_HID,           // bDescriptorType
    0x10, 0x01,             // HID Class Spec. release number
    0x00,                   // H/W target country
    0x01,                   // Number of HID class descriptors to follow
    USB_DESC_HID_RPT,       // Descriptor type
    sizeof(HID_ReportDescriptor) & 0xFF, ((sizeof(HID_ReportDescriptor) >> 8) & 0xFF),  // Total length of report descriptor

    /* EP Descriptor: interrupt in. */
    7,                              // bLength
    USB_DESC_ENDPOINT,              // bDescriptorType
    (USB_EP_IN | EP_INT_IN_NUM),  	// bEndpointAddress
    USB_EP_INT,                     // bmAttributes
    EP_INT_IN_PKSZ, 0x00,         	// wMaxPacketSize
    10,    							// bInterval
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
    16,
    USB_DESC_STRING,
    'H', 0, 'I', 0, 'D', 0, ' ', 0, 'A', 0, 'D', 0, 'C', 0
};

uint8_t *USBD_StringDescriptor[4] =
{
    LangStringDesc,
    VendorStringDesc,
    ProductStringDesc,
    NULL,
};

uint16_t USBD_HIDOffset[2] =
{
    9+9,
    0,
};

uint8_t *USBD_HIDReport[2] =
{
    HID_ReportDescriptor,
    NULL,
};
