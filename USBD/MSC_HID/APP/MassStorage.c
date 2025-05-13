#include <string.h>
#include "SWM341.h"
#include "FlashDisk.h"
#include "MassStorage.h"
#include "hid_transfer.h"


#define MSC_STATE_CBW  0
#define MSC_STATE_IN   1
#define MSC_STATE_OUT  2
#define MSC_STATE_CSW  3

volatile uint8_t MSC_State = MSC_STATE_CBW;

struct CBW g_CBW = {.dCBWSignature = CBW_SIGNATURE};   // Command Block Wrapper
struct CSW g_CSW = {.dCSWSignature = CSW_SIGNATURE};   // Command Status Wrapper


extern uint8_t  USBD_DeviceDescriptor[];
extern uint8_t  USBD_ConfigDescriptor[];
extern uint8_t *USBD_StringDescriptor[];
extern uint16_t USBD_HIDOffset[];
extern uint8_t *USBD_HIDReport[];
void MSC_ClassRequest(USB_Setup_Packet_t * pSetup);

void MSC_Init(void)
{
	FlashDiskInit();
	
	USBD_Info.Mode = USBD_MODE_DEV;
	USBD_Info.Speed = USBD_SPEED_FS;
	USBD_Info.CtrlPkSiz = EP_CTRL_PKSZ;
	USBD_Info.DescDevice = USBD_DeviceDescriptor;
	USBD_Info.DescConfig = USBD_ConfigDescriptor;
	USBD_Info.DescString = USBD_StringDescriptor;
	USBD_Info.DescHIDOffset = USBD_HIDOffset;
	USBD_Info.DescHIDReport = USBD_HIDReport;
	USBD_Info.pClassRequest_Callback = MSC_ClassRequest;
	USBD_Init();

	/*
	   Generate Mass-Storage Device serial number
	   To compliant USB-IF MSC test, we must enable serial string descriptor.
	   However, windows may fail to recognize the devices if PID/VID and serial number are all the same
	   when plug them to Windows at the sample time.
	   Therefore, we must generate different serial number for each device to avoid conflict
	   when plug more then 2 MassStorage devices to Windows at the same time.

	   NOTE: We use compiler predefine macro "__TIME__" to generate different number for serial
	   at each build but each device here for a demo.
	   User must change it to make sure all serial number is different between each device.
	 */
	uint8_t *stringSerial = USBD_StringDescriptor[3];
	for(uint32_t i = 0; i < 8; i++)  stringSerial[stringSerial[0] - 16 + i * 2] = __TIME__[i];
}


static uint8_t BulkBuffer[EP_BULK_OUT_PKSZ];
static uint8_t HIDBuffer[EP_INT_OUT_PKSZ];

volatile uint8_t BulkOutSize = 0;

void USB_Handler(void)
{
	uint32_t devif = USBD->DEVIF;
    uint32_t epif  = USBD->EPIF;

    if(devif & USBD_DEVIF_RST_Msk)
    {
        USBD->DEVIF = USBD_DEVIF_RST_Msk;
    }
	else if(devif & USBD_DEVIF_SETCFG_Msk)
	{
		USBD->DEVIF = USBD_DEVIF_SETCFG_Msk;
		
		USBD_RxReady(EP_BULK_OUT_NUM);
		
		USBD_RxReady(EP_INT_OUT_NUM);
		
		MSC_State = MSC_STATE_CBW;
	}
    else if(devif & USBD_DEVIF_SETUP_Msk)
    {
        USBD->SETUPSR = USBD_SETUPSR_DONE_Msk;
		
		if(USBD->SETUPSR & USBD_SETUPSR_SUCC_Msk)
		{
			USBD_ProcessSetupPacket();
		}
    }
	else
    {
        if(epif & USBD_EPIF_INEP0_Msk)					// Ctrl IN
        {
			if(USBD_TxSuccess(0))
			{
				USBD_CtrlIn();
			}
			USBD_TxIntClr(0);
        }
        else if(epif & USBD_EPIF_OUTEP0_Msk)			// Ctrl OUT
        {
			USBD_RxIntClr();
			if(USBD_RxSuccess())
			{
				USBD_CtrlOut();
			}
        }
		else if(epif & (1 << EP_BULK_IN_NUM))			// Bulk IN
        {
			if(USBD_TxSuccess(EP_BULK_IN_NUM))
			{
				MSC_ProcessIN();
			}
			else if(USBD_TxNAKSent(EP_BULK_IN_NUM))
			{
				/* After detecting EP_BULK_IN_NUM STALL, the host uses the standard request "Clear Feature" to clear the stall, and then reads the CSW */
				if((MSC_State == MSC_STATE_IN) && ((g_CBW.u8OPCode == UFI_INQUIRY) || (g_CBW.u8OPCode == UFI_REQUEST_SENSE)))
				{
					MSC_ProcessIN();
				}
			}
			USBD_TxIntClr(EP_BULK_IN_NUM);
        }
        else if(epif & (1 << (EP_BULK_OUT_NUM + 16)))	// Bulk OUT
        {
			USBD_RxIntClr();
			if(USBD_RxSuccess())
			{
				BulkOutSize = USBD_RxRead(BulkBuffer, sizeof(BulkBuffer));
			}
        }
		else if(epif & (1 << EP_INT_IN_NUM))			// Interrupt IN
		{
			if(USBD_TxSuccess(EP_INT_IN_NUM))
			{
				HID_SetInReport();
			}
			USBD_TxIntClr(EP_INT_IN_NUM);
		}
		else if(epif & (1 << (EP_INT_OUT_NUM + 16)))	// Interrupt OUT
		{
			USBD_RxIntClr();
			if(USBD_RxSuccess())
			{
				uint8_t size = USBD_RxRead(HIDBuffer, sizeof(HIDBuffer));
				HID_GetOutReport(HIDBuffer, size);
			}
		}
    }
}


void MSC_ClassRequest(USB_Setup_Packet_t * pSetup)
{
	if(pSetup->bRequestType & USB_EP_IN)   // Device to host
	{
		switch(pSetup->bRequest)
		{
		case 0xFE:		// GET_MAX_LUN
			/* wIndex stores bInterfaceNumber */
			if((pSetup->wIndex == USBD_ConfigDescriptor[9 + 2]) && (pSetup->wValue + pSetup->wLength == 1))
			{
				/* Data stage */
				uint8_t max_lun = 0;
				USBD_TxWrite(0, &max_lun, 1);
				
				/* Status stage */
				USBD_RxReady(0);
			}
			else
			{
				/* Stall when wrong parameter */
				USBD_Stall0();
			}
			break;
		
		case USB_HID_GET_REPORT:
		case USB_HID_GET_IDLE:
		case USB_HID_GET_PROTOCOL:
			USBD_Stall0();
			break;
		
		default:
			/* Setup error, stall the device */
			USBD_Stall0();
			break;
		}
	}
	else                    				// Host to device
	{
		switch(pSetup->bRequest)
		{
		case 0xFF:		// BULK_ONLY_MASS_STORAGE_RESET
			/* wIndex stores bInterfaceNumber */
			if((pSetup->wIndex == USBD_ConfigDescriptor[9 + 2]) && (pSetup->wValue + pSetup->wLength == 0))
			{
				/* Prepare to receive the CBW */
				MSC_State = MSC_STATE_CBW;
				
				/* Status stage */
				USBD_TxWrite(0, 0, 0);
			}
			else
			{
				/* Stall when wrong parameter */
				USBD_Stall0();
			}
			break;
		
		case USB_HID_SET_REPORT:
		{
			if((pSetup->wValue >> 8) == 3)
			{
				USBD_TxWrite(0, 0, 0);
			}
			break;
		}
		
		case USB_HID_SET_IDLE:
		{
			/* Status stage */
			USBD_TxWrite(0, 0, 0);
			break;
		}
		
		case USB_HID_SET_PROTOCOL:
			USBD_Stall0();
			break;
		
		default:
			/* Setup error, stall the device */
			USBD_Stall0();
			break;
		}
	}
}


/*----------------------------------------------------------------------------------------------------------*/
#define SECTOR_SIZE     512
#define SECTOR_COUNT    (DATA_FLASH_SIZE / SECTOR_SIZE)

uint8_t Buffer[SECTOR_SIZE];

uint64_t g_Address;
uint32_t g_Size;

uint8_t  g_Remove = 0;
uint8_t  g_PreventRemove = 0;

uint8_t  g_SenseKey[4];

uint8_t InquiryData[36] =
{
	0x00,                   // Peripheral Device Type
	0x80,                   // Removable Medium Bit
	0x00,                   // ISO/ECMA, ANSI Version
	0x00,                   // Response Data Format
	0x1F, 0x00, 0x00, 0x00, // Additional Length

	/* Vendor Identification */
	'S', 'y', 'n', 'w', 'i', 't', ' ',

	/* Product Identification */
	'U', 'S', 'B', ' ', 'M', 'a', 's', 's', ' ', 'S', 't', 'o', 'r', 'a', 'g', 'e', ' ',

	/* Product Revision */
	'1', '.', '0', '0'
};


static __INLINE uint32_t min(uint32_t a, uint32_t b)
{
	return (a < b) ? a : b;
}

void MSC_ProcessOUT(void)
{
	uint8_t size;
	
	if(BulkOutSize == 0)
		return;

	if(MSC_State == MSC_STATE_CBW)
	{
		memcpy((uint8_t *)&g_CBW, BulkBuffer, sizeof(g_CBW));
		
		if((g_CBW.dCBWSignature != CBW_SIGNATURE) || (BulkOutSize != sizeof(g_CBW)))
		{
			/* Invalid CBW */
			USBD_TxStall(EP_BULK_IN_NUM);
			USBD_RxStall(EP_BULK_OUT_NUM);
			return;
		}

		g_CSW.dCSWTag = g_CBW.dCBWTag;	// Prepare to echo the tag from CBW to CSW
		
		switch(g_CBW.u8OPCode)
		{
		case UFI_INQUIRY:
			if((g_CBW.dCBWDataTransferLength > 0) && (g_CBW.dCBWDataTransferLength <= 36))
			{
				USBD_TxWrite(EP_BULK_IN_NUM, (uint8_t *)InquiryData, g_CBW.dCBWDataTransferLength);
				
				g_CSW.bCSWStatus = 0;
				g_CSW.dCSWDataResidue = 0;
			}
			else
			{
				USBD_TxStall(EP_BULK_IN_NUM);
				
				/* After detecting EP_BULK_IN_NUM STALL, the host uses the standard request "Clear Feature" to clear the stall, and then reads the CSW */
				g_CSW.bCSWStatus = 1;
				g_CSW.dCSWDataResidue = 0;
			}
			MSC_State = MSC_STATE_IN;
			break;
		
		case UFI_READ_FORMAT_CAPACITY:
			g_Address = (uint32_t)Buffer;
			g_Size = g_CBW.dCBWDataTransferLength;	// The host does not know what the "Capacity List Length" of the device is, so it may read 256 bytes
			
			memset(Buffer, 0x00, sizeof(Buffer));

			/*---------- Capacity List Header -------------------------*/
			// Capacity List Length
			Buffer[3] = 0x10;

			/*---------- Current/Maximum Capacity Descriptor ----------*/
			Buffer[4] = (SECTOR_COUNT >> 24) & 0xFF;		// number of blocks
			Buffer[5] = (SECTOR_COUNT >> 16) & 0xFF;
			Buffer[6] = (SECTOR_COUNT >>  8) & 0xFF;
			Buffer[7] = (SECTOR_COUNT >>  0) & 0xFF;

			// Descriptor Code:
			// 01b = Unformatted Media - Maximum formattable capacity for this cartridge
			// 10b = Formatted Media - Current media capacity
			// 11b = No Cartridge in Drive - Maximum formattable capacity for any cartridge
			Buffer[8] = 0x02;

			Buffer[ 9] = 0x00;								// sector size: 512 bytes
			Buffer[10] = 0x02;
			Buffer[11] = 0x00;

			/*---------- Formattable Capacity Descriptor --------------*/
			Buffer[12] = (SECTOR_COUNT >> 24) & 0xFF;
			Buffer[13] = (SECTOR_COUNT >> 16) & 0xFF;
			Buffer[14] = (SECTOR_COUNT >>  8) & 0xFF;
			Buffer[15] = (SECTOR_COUNT >>  0) & 0xFF;

			Buffer[17] = 0x00;								// sector size: 512 bytes
			Buffer[18] = 0x02;
			Buffer[19] = 0x00;
		
			size = min(g_Size, EP_BULK_IN_PKSZ);

			USBD_TxWrite(EP_BULK_IN_NUM, (uint8_t *)g_Address, size);
			g_Address += size;
			g_Size -= size;
			
			MSC_State = MSC_STATE_IN;
			break;
		
		case UFI_READ_CAPACITY:
			g_Address = (uint32_t)Buffer;
			g_Size = g_CBW.dCBWDataTransferLength;
			
			memset(Buffer, 0x00, sizeof(Buffer));
			
			Buffer[0] = ((SECTOR_COUNT - 1) >> 24) & 0xFF;	// the last logical block number on the device (MSB first)
			Buffer[1] = ((SECTOR_COUNT - 1) >> 16) & 0xFF;
			Buffer[2] = ((SECTOR_COUNT - 1) >>  8) & 0xFF;
			Buffer[3] = ((SECTOR_COUNT - 1) >>  0) & 0xFF;
			Buffer[4] = 0x00;								// sector size: 512 bytes
			Buffer[5] = 0x00;
			Buffer[6] = 0x02;
			Buffer[7] = 0x00;

			size = min(g_Size, EP_BULK_IN_PKSZ);

			USBD_TxWrite(EP_BULK_IN_NUM, (uint8_t *)g_Address, size);
			g_Address += size;
			g_Size -= size;
			
			MSC_State = MSC_STATE_IN;
			break;
		
		case UFI_READ_10:
		case UFI_READ_12:
			g_Address = __REV(*(uint32_t *)g_CBW.au8Data) * 1LL * SECTOR_SIZE;
			g_Size = g_CBW.dCBWDataTransferLength;

			size = min(g_Size, EP_BULK_IN_PKSZ);

			FlashDiskRead(g_Address, size, Buffer);
			g_Address += size;
			g_Size -= size;

			USBD_TxWrite(EP_BULK_IN_NUM, Buffer, size);
			
			MSC_State = MSC_STATE_IN;
			break;

		case UFI_WRITE_10:
		case UFI_WRITE_12:
			g_Address = __REV(*(uint32_t *)g_CBW.au8Data) * 1LL * SECTOR_SIZE;
			g_Size = g_CBW.dCBWDataTransferLength;
			
			MSC_State = MSC_STATE_OUT;
			break;
		
		case UFI_TEST_UNIT_READY:
			if(g_CBW.dCBWDataTransferLength != 0)
			{
				if(g_CBW.bmCBWFlags == 0)	// OUT
				{
					USBD_RxStall(EP_BULK_OUT_NUM);
					
					g_CSW.bCSWStatus = 1;
					g_CSW.dCSWDataResidue = g_CBW.dCBWDataTransferLength;
				}
			}
			else
			{
				if(g_Remove)
				{
					g_SenseKey[0] = 0x02;    // Not ready
					g_SenseKey[1] = 0x3A;
					g_SenseKey[2] = 0x00;
					
					g_CSW.bCSWStatus = 1;
					g_CSW.dCSWDataResidue = 0;
				}
				else
				{
					g_CSW.bCSWStatus = 0;
					g_CSW.dCSWDataResidue = 0;
				}
			}
			
			MSC_State = MSC_STATE_IN;
			MSC_ProcessIN();
			break;

		case UFI_REQUEST_SENSE:
			if((g_CBW.dCBWDataTransferLength > 0) && (g_CBW.dCBWDataTransferLength <= 18))
			{
				memset(Buffer, 0x00, 64);
				
				Buffer[0]  = 0x70;
				Buffer[2]  = g_SenseKey[0];
				Buffer[7]  = 0x0A;
				Buffer[12] = g_SenseKey[1];
				Buffer[13] = g_SenseKey[2];
				
				USBD_TxWrite(EP_BULK_IN_NUM, Buffer, g_CBW.dCBWDataTransferLength);
				
				g_SenseKey[0] = 0;
				g_SenseKey[1] = 0;
				g_SenseKey[2] = 0;

				g_CSW.bCSWStatus = 0;
				g_CSW.dCSWDataResidue = 0;
			}
			else
			{
				USBD_TxStall(EP_BULK_IN_NUM);
				
				/* After detecting EP_BULK_IN_NUM STALL, the host uses the standard request "Clear Feature" to clear the stall, and then reads the CSW */
				g_CSW.bCSWStatus = 1;
				g_CSW.dCSWDataResidue = 0;
			}
			MSC_State = MSC_STATE_IN;
			break;
		
		case UFI_VERIFY_10:			
			MSC_State = MSC_STATE_IN;
			MSC_ProcessIN();
			break;
		
		case UFI_START_STOP:
			if((g_CBW.au8Data[2] & 3) == 2)
			{
				g_Remove = 1;
			}
			
			MSC_State = MSC_STATE_IN;
			MSC_ProcessIN();
			break;
		
		case UFI_PREVENT_ALLOW_MEDIUM_REMOVAL:
			if(g_CBW.au8Data[2] & 1)
			{
				g_SenseKey[0] = 0x05;  		// INVALID COMMAND
				g_SenseKey[1] = 0x24;
				g_SenseKey[2] = 0x00;
				
				g_PreventRemove = 1;
			}
			else
			{
				g_PreventRemove = 0;
			}
			
			MSC_State = MSC_STATE_IN;
			MSC_ProcessIN();
			break;

		default:
			/* Unsupported command */
			g_SenseKey[0] = 0x05;
			g_SenseKey[1] = 0x20;
			g_SenseKey[2] = 0x00;
		
			g_CSW.bCSWStatus = 0;
			g_CSW.dCSWDataResidue = 0;

			/* If CBW request for data phase, just return zero len packet to end data phase */
			if(g_CBW.dCBWDataTransferLength > 0)
			{
				if(g_CBW.bmCBWFlags & 0x80)
				{
					USBD_TxWrite(EP_BULK_IN_NUM, 0, 0);
					MSC_State = MSC_STATE_IN;
				}
			}
			else
			{
				MSC_State = MSC_STATE_IN;
				MSC_ProcessIN();
			}
			break;
		}
	}
	else if(MSC_State == MSC_STATE_OUT)
	{
		switch(g_CBW.u8OPCode)
		{
		case UFI_WRITE_10:
		case UFI_WRITE_12:
			size = min(BulkOutSize, g_Size);
			
			FlashDiskWrite(g_Address, size, BulkBuffer);
			g_Address += size;
			g_Size -= size;
		
			if(g_Size == 0)
			{
				g_CSW.bCSWStatus = 0;
				g_CSW.dCSWDataResidue = 0;
				
				USBD_TxWrite(EP_BULK_IN_NUM, (uint8_t *)&g_CSW.dCSWSignature, 13);
				MSC_State = MSC_STATE_CSW;
			}
			break;
		}
	}
	
	BulkOutSize = 0;
	USBD_RxReady(EP_BULK_OUT_NUM);
}


void MSC_ProcessIN(void)
{
	uint8_t size;
	
	if(MSC_State == MSC_STATE_CSW)
	{
		/* After sending the CSW, prepare to receive the next CBW */
		MSC_State = MSC_STATE_CBW;
	}
	else if(MSC_State == MSC_STATE_IN)
	{
		switch(g_CBW.u8OPCode)
		{
		case UFI_INQUIRY:
		case UFI_REQUEST_SENSE:
		case UFI_TEST_UNIT_READY:
			break;
		
		case UFI_READ_FORMAT_CAPACITY:
		case UFI_READ_CAPACITY:
			if(g_Size > 0)
			{
				size = min(g_Size, EP_BULK_IN_PKSZ);
				
				USBD_TxWrite(EP_BULK_IN_NUM, (uint8_t *)g_Address, size);

				g_Address += size;
				g_Size -= size;
				return;
			}
			
			g_CSW.bCSWStatus = 0;
			g_CSW.dCSWDataResidue = 0;
			break;
		
		case UFI_READ_10:
		case UFI_READ_12:
			if(g_Size > 0)
			{
				size = min(g_Size, EP_BULK_IN_PKSZ);

				FlashDiskRead(g_Address, size, Buffer);
				g_Address += size;
				g_Size -= size;

				USBD_TxWrite(EP_BULK_IN_NUM, Buffer, size);
				return;
			}
			
			g_CSW.bCSWStatus = 0;
			g_CSW.dCSWDataResidue = 0;
			break;
		
		case UFI_VERIFY_10:
		case UFI_START_STOP:
		case UFI_PREVENT_ALLOW_MEDIUM_REMOVAL:
		{
			int tmp = g_CBW.dCBWDataTransferLength - SECTOR_SIZE;

			g_CSW.dCSWDataResidue = (tmp < 0) ? 0 : tmp;
			g_CSW.bCSWStatus = 0;
			break;
		}
		
		default:
			/* Unsupported commmand. */
			g_CSW.dCSWDataResidue = g_CBW.dCBWDataTransferLength;
			g_CSW.bCSWStatus = 1;
			break;
		}

		USBD_TxWrite(EP_BULK_IN_NUM, (uint8_t *)&g_CSW.dCSWSignature, 13);
		MSC_State = MSC_STATE_CSW;
	}
}
