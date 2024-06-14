#include <string.h>
#include "SWM341.h"
#include "hid_dac.h"

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


static uint8_t Buffer[64];


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
            if((pSetup->wValue >> 8) == 3)
            {
                USBD_TxWrite(0, 0, 0);
            }
            break;
		
        case USB_HID_SET_IDLE:
            /* Status stage */
            USBD_TxWrite(0, 0, 0);
            break;
		
        case USB_HID_SET_PROTOCOL:
        default:
			USBD_Stall0();
            break;
        }
    }
}


extern uint16_t DAC_nPoint;
extern uint16_t DAC_Buffer[1024];

void HID_GetOutReport(uint8_t *buf, uint32_t size)
{
	uint8_t chnl = buf[0];						// 此包中数据属于哪个 DAC 通道
	uint8_t count = buf[1];						// 此包中数据个数
	uint16_t offset = buf[2] + (buf[3] << 8);	// 此包中数据在波形中的位置
	
//	printf("Copy %d-bytes from %08X to %08X\n", count*2, (int)&buf[4], (int)&DAC_Buffer[offset]);
	
	DAC_nPoint = offset + count;
	if(DAC_nPoint > 1024)
		return;
	
	memcpy(&DAC_Buffer[offset], &buf[4], count*2);
	
	USBD_RxReady(EP_INT_OUT_NUM);
}


void HID_SetInReport(void)
{
}
