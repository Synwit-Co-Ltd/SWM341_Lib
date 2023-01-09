#include "SWM341.h"
#include "winusb_transfer.h"


uint8_t USBD_DeviceDescriptor[] =
{
    18,                     // bLength
    USB_DESC_DEVICE,        // bDescriptorType
    0x01, 0x02,             // bcdUSB£¬MS OS 2.0 ÃèÊö·ûÒªÇó USB 2.1
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



uint8_t USBD_BOSDescriptor[] =
{
	5,
	USB_DESC_BOS,
	5+20+8, 0,					// wTotalLength
	1,							// bNumDeviceCaps
	
	/*** MS OS 2.0 descriptor platform capability descriptor ***/
	28,
	USB_DESC_CAPABILITY,
	5,							// bDevCapabilityType: PLATFORM (05H)
	0x00,
	0xDF, 0x60, 0xDD, 0xD8,		// PlatformCapabilityUUID: MS_OS_20_Platform_Capability_ID (D8DD60DF-4589-4CC7-9CD2-659D9E648A9F)
	0x89, 0x45, 0xC7, 0x4C,
	0x9C, 0xD2, 0x65, 0x9D,
	0x9E, 0x64, 0x8A, 0x9F,
	
	0x00, 0x00, 0x03, 0x06,		// dwWindowsVersion: 0x06030000 for Windows 8.1
	10+20, 0x00,      			// wTotalLength: size of MS OS 2.0 descriptor set
    WINUSB_VENDOR_CODE,   		// bMS_VendorCode
    0x00,                 		// bAltEnumCmd
};


#define MS_OS_20_SET_HEADER_DESCRIPTOR        0x00
#define MS_OS_20_SUBSET_HEADER_CONFIGURATION  0x01
#define MS_OS_20_SUBSET_HEADER_FUNCTION       0x02
#define MS_OS_20_FEATURE_COMPATIBLE_ID        0x03
#define MS_OS_20_FEATURE_REG_PROPERTY         0x04
#define MS_OS_20_FEATURE_MIN_RESUME_TIME      0x05
#define MS_OS_20_FEATURE_MODEL_ID             0x06
#define MS_OS_20_FEATURE_CCGP_DEVICE          0x07
#define MS_OS_20_FEATURE_VENDOR_REVISION      0x08

uint8_t MS_OS_20_DescriptorSet[] = 
{
	/*** Microsoft OS 2.0 Descriptor Set Header ***/
	10, 0,
	MS_OS_20_SET_HEADER_DESCRIPTOR, 0,
	0x00, 0x00, 0x03, 0x06,		// dwWindowsVersion: 0x06030000 for Windows 8.1
	10+20, 0,					// wTotalLength
	
	/*** Microsoft OS 2.0 compatible ID descriptor ***/
	20, 0,
	MS_OS_20_FEATURE_COMPATIBLE_ID, 0,
	'W',  'I',  'N',  'U',  'S',  'B',  0x00, 0x00,		// CompatibleID
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// SubCompatibleID
};
