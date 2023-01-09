#include "SWM341.h"
#include "vcom_serial.h"


uint8_t USBD_DeviceDescriptor[] =
{
    18,         			// bLength
    USB_DESC_DEVICE,    	// bDescriptorType
    0x00, 0x02,         	// bcdUSB
    0x02,               	// bDeviceClass
    0x00,               	// bDeviceSubClass
    0x00,               	// bDeviceProtocol
    EP_CTRL_PKSZ,   		// bMaxPacketSize0
    USBD_VID & 0xFF, ((USBD_VID >> 8) & 0xFF),  // idVendor
    USBD_PID & 0xFF, ((USBD_PID >> 8) & 0xFF),  // idProduct
    0x00, 0x03,     		// bcdDevice
    0x01,           		// iManufacture
    0x02,           		// iProduct
    0x00,           		// iSerialNumber - no serial
    0x01            		// bNumConfigurations
};


uint8_t USBD_ConfigDescriptor[] =
{
    9,     					// bLength
    USB_DESC_CONFIG,    	// bDescriptorType
    9+9+5+5+4+5+7+9+7+7, 0x00, 	// wTotalLength
    0x02,           		// bNumInterfaces
    0x01,           		// bConfigurationValue
    0x00,           		// iConfiguration
    0xC0,           		// bmAttributes
    0x32,           		// MaxPower

    // INTERFACE descriptor
    9,  					// bLength
    USB_DESC_INTERFACE, 	// bDescriptorType
    0x00,           		// bInterfaceNumber
    0x00,           		// bAlternateSetting
    0x01,           		// bNumEndpoints
    0x02,           		// bInterfaceClass
    0x02,           		// bInterfaceSubClass
    0x01,           		// bInterfaceProtocol
    0x00,           		// iInterface

    // Communication Class Specified INTERFACE descriptor
    0x05,           		// Size of the descriptor, in bytes
    0x24,           		// CS_INTERFACE descriptor type
    0x00,           		// Header functional descriptor subtype
    0x10, 0x01,     		// Communication device compliant to the communication spec. ver. 1.10

    // Communication Class Specified INTERFACE descriptor
    0x05,           		// Size of the descriptor, in bytes
    0x24,           		// CS_INTERFACE descriptor type
    0x01,           		// Call management functional descriptor
    0x00,           		// BIT0: Whether device handle call management itself.
							// BIT1: Whether device can send/receive call management information over a Data Class Interface 0
    0x01,           		// Interface number of data class interface optionally used for call management

    // Communication Class Specified INTERFACE descriptor
    0x04,           		// Size of the descriptor, in bytes
    0x24,           		// CS_INTERFACE descriptor type
    0x02,           		// Abstract control management functional descriptor subtype
    0x00,           		// bmCapabilities

    // Communication Class Specified INTERFACE descriptor
    0x05,           		// bLength
    0x24,           		// bDescriptorType: CS_INTERFACE descriptor type
    0x06,           		// bDescriptorSubType
    0x00,           		// bMasterInterface
    0x01,           		// bSlaveInterface0

    // ENDPOINT descriptor 
    7,                   				// bLength
    USB_DESC_ENDPOINT,              	// bDescriptorType
    (USB_EP_IN | EP_INT_IN_NUM),   	 	// bEndpointAddress
    USB_EP_INT,                     	// bmAttributes
    EP_INT_IN_PKSZ, 0x00,         		// wMaxPacketSize
    0x01,                           	// bInterval

    // INTERFACE descriptor 
    9,  					// bLength
    USB_DESC_INTERFACE, 	// bDescriptorType
    0x01,           		// bInterfaceNumber
    0x00,           		// bAlternateSetting
    0x02,           		// bNumEndpoints
    0x0A,           		// bInterfaceClass
    0x00,           		// bInterfaceSubClass
    0x00,           		// bInterfaceProtocol
    0x00,           		// iInterface

    // ENDPOINT descriptor 
    7,                   				// bLength
    USB_DESC_ENDPOINT,              	// bDescriptorType
    (USB_EP_IN | EP_BULK_IN_NUM),    	// bEndpointAddress
    USB_EP_BULK,                        // bmAttributes
    EP_BULK_IN_PKSZ, 0x00,         		// wMaxPacketSize
    0x00,                           	// bInterval

    // ENDPOINT descriptor 
    7,                   				// bLength
    USB_DESC_ENDPOINT,              	// bDescriptorType
    (USB_EP_OUT | EP_BULK_OUT_NUM),  	// bEndpointAddress
    USB_EP_BULK,                        // bmAttributes
    EP_BULK_OUT_PKSZ, 0x00,         	// wMaxPacketSize
    0x00,                           	// bInterval
};


uint8_t StringLang[4] =
{
    4,              	// bLength 
    USB_DESC_STRING,   	// bDescriptorType 
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
    32,
    USB_DESC_STRING,
    'U', 0, 'S', 0, 'B', 0, ' ', 0, 'V', 0, 'i', 0, 'r', 0, 't', 0, 'u', 0, 'a', 0, 'l', 0, ' ', 0, 'C', 0, 'O', 0, 'M', 0
};

uint8_t *USBD_StringDescriptor[] =
{
    StringLang,
    VendorStringDesc,
    ProductStringDesc,
    NULL,
};
