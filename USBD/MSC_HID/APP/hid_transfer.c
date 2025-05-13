#include <string.h>
#include "SWM341.h"
#include "hid_transfer.h"


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
