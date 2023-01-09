#include <string.h>
#include "SWM341.h"
#include "hid_transfer.h"

extern uint8_t  USBD_DeviceDescriptor[];
extern uint8_t  USBD_ConfigDescriptor[];
extern uint8_t *USBD_StringDescriptor[];
extern uint16_t USBD_HIDOffset[];
extern uint8_t *USBD_HIDReport[];

void HID_Init(void)
{	
	USBD_Info.Mode = USBD_MODE_DEV;
	USBD_Info.Speed = USBD_SPEED_FS;
	USBD_Info.CtrlPkSiz = EP_CTRL_PKSZ;
	USBD_Info.DescDevice = USBD_DeviceDescriptor;
	USBD_Info.DescConfig = USBD_ConfigDescriptor;
	USBD_Info.DescString = USBD_StringDescriptor;
	USBD_Info.DescHIDOffset = USBD_HIDOffset;
	USBD_Info.DescHIDReport = USBD_HIDReport;
	USBD_Info.pClassRequest_Callback = HID_ClassRequest;
	USBD_Init();
}


static uint8_t Buffer[EP_INT_OUT_PKSZ];

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
		
		USBD_RxReady(EP_INT_OUT_NUM);
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
        if(epif & USBD_EPIF_INEP0_Msk)					// Control IN
        {
			if(USBD_TxSuccess(0))
			{
				USBD_CtrlIn();
			}
			USBD_TxIntClr(0);
        }
        else if(epif & USBD_EPIF_OUTEP0_Msk)			// Control OUT
        {
			USBD_RxIntClr();
			if(USBD_RxSuccess())
			{
				USBD_CtrlOut();
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
				uint8_t size = USBD_RxRead(Buffer, sizeof(Buffer));
				HID_GetOutReport(Buffer, size);
			}
        }
    }
}


void HID_ClassRequest(USB_Setup_Packet_t * pSetup)
{
    if(pSetup->bRequestType & 0x80)		// Device to Host
    {
        switch(pSetup->bRequest)
        {
        case USB_HID_GET_REPORT:
        case USB_HID_GET_IDLE:
        case USB_HID_GET_PROTOCOL:
        default:
			USBD_Stall0();
            break;
        }
    }
    else								// Host to Device
    {
        switch(pSetup->bRequest)
        {
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
        default:
			USBD_Stall0();
            break;
        }
    }
}


/***************************************************************/
/* HID Transfer Commands */
#define HID_CMD_SIGNATURE	0x43444948
#define HID_CMD_ERASE    	0x71
#define HID_CMD_WRITE    	0xC3
#define HID_CMD_READ     	0xD2
#define HID_CMD_NONE     	0x00

#define SECTOR_SIZE      	1024
#define SECTOR_COUNT		4
#define PAGE_SIZE        	256
#define PAGE_COUNT			(SECTOR_SIZE * SECTOR_COUNT / PAGE_SIZE)

static uint8_t  RAMDisk[SECTOR_SIZE * SECTOR_COUNT] = {0};
static uint8_t  PageBuf[PAGE_SIZE] = {0};	// Page buffer to upload/download through HID report
static uint32_t IndexInPageBuf = 0;


typedef struct __attribute__((__packed__))
{
    uint8_t  u8Cmd;
    uint8_t  u8Size;
    uint32_t u32Arg1;
    uint32_t u32Arg2;
    uint32_t u32Signature;
    uint32_t u32Checksum;
} CMD_T;

CMD_T gCmd;


int32_t HID_CmdEraseSectors(CMD_T *pCmd)
{
    uint32_t sector = pCmd->u32Arg1;
    uint32_t count  = pCmd->u32Arg2;
    
    memset(RAMDisk + sector * SECTOR_SIZE, 0xFF, count * SECTOR_SIZE);

    pCmd->u8Cmd = HID_CMD_NONE;		// note the command has been done

    return 0;
}


int32_t HID_CmdWritePage(CMD_T *pCmd)
{
    IndexInPageBuf = 0;

    return 0;
}


int32_t HID_CmdReadPage(CMD_T *pCmd)
{
    uint32_t page = pCmd->u32Arg1;

	/* Update data to page buffer to upload */
	memcpy(PageBuf, RAMDisk + page * PAGE_SIZE, sizeof(PageBuf));
	IndexInPageBuf = PAGE_SIZE;
	
	HID_SetInReport();

    return 0;
}


int32_t ProcessCommand(uint8_t *buf, uint32_t len)
{
    uint32_t i, sum = 0;

    memcpy((uint8_t *)&gCmd, buf, len);

    /* Check signature */
    if(gCmd.u32Signature != HID_CMD_SIGNATURE)
        return -1;

    /* Check checksum  */
	for(i = 0; i < gCmd.u8Size; i++) sum += ((uint8_t *)&gCmd)[i];
    if(sum != gCmd.u32Checksum)
        return -1;

    switch(gCmd.u8Cmd)
    {
    case HID_CMD_ERASE:
        HID_CmdEraseSectors(&gCmd);
        break;
    
	case HID_CMD_WRITE:
        HID_CmdWritePage(&gCmd);
        break;
	
    case HID_CMD_READ:
        HID_CmdReadPage(&gCmd);
        break;
    
    default:
        return -1;
    }

    return 0;
}


void HID_GetOutReport(uint8_t *buf, uint32_t size)
{
    if(gCmd.u8Cmd == HID_CMD_WRITE)		// data phase of write command
    {
        memcpy(&PageBuf[IndexInPageBuf], buf, size);
        IndexInPageBuf += size;

        if(IndexInPageBuf >= PAGE_SIZE)
        {
			uint32_t page = gCmd.u32Arg1;
            memcpy(RAMDisk + page * PAGE_SIZE, PageBuf, sizeof(PageBuf));

			gCmd.u8Cmd = HID_CMD_NONE;	// Write command complete
        }
    }
    else
    {
        ProcessCommand(buf, size);
    }
	
	USBD_RxReady(EP_INT_OUT_NUM);
}


void HID_SetInReport(void)
{
    if(gCmd.u8Cmd == HID_CMD_READ)		// data phase of read command
    {
        if(IndexInPageBuf)
        {
			/* Prepare the data for next HID IN transfer */
            USBD_TxWrite(EP_INT_IN_NUM, (uint8_t *)&PageBuf[PAGE_SIZE - IndexInPageBuf], EP_INT_IN_PKSZ);
			
            IndexInPageBuf -= EP_INT_IN_PKSZ;
        }
        else
        {
            gCmd.u8Cmd = HID_CMD_NONE;	// Read command complete
        }
    }
}
