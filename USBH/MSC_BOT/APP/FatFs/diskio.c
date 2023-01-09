#include "ff.h"
#include "diskio.h"

#include "SWM341.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"


#define PDRV_USB  2   // "usb" 在 FF_VOLUME_STRS 中的索引


DSTATUS disk_initialize(BYTE pdrv)
{
	DSTATUS stat = STA_NOINIT;
	
	switch(pdrv)
	{
	case PDRV_USB:
		stat = disk_status(pdrv);
		break;
	
	default:
		break;
	}
	
	return stat;
}


DSTATUS disk_status(BYTE pdrv)
{
	DSTATUS stat = STA_NOINIT;
	
	switch(pdrv)
	{
	case PDRV_USB:
		stat = USBH_MSC_Info.Ready ? 0x00 : STA_NOINIT;
		break;
	
	default:
		break;
	}
	
	return stat;
}


DRESULT disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
	uint32_t res;
	
	if(disk_status(pdrv) == STA_NOINIT)
		return RES_NOTRDY;

	if(count == 0)
		return RES_PARERR;
	
	switch(pdrv)
	{
	case PDRV_USB:
		do {
			res = USBH_MSC_Read10(buff, sector, 512*count);
		} while(res == USBH_MSC_BUSY);

		if(res == USBH_MSC_OK)
			return RES_OK;
		else
			return RES_ERROR;
			
	default:
		break;
	}
	
	return RES_ERROR;
}


DRESULT disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
	uint32_t res;
	
	if(disk_status(pdrv) == STA_NOINIT)
		return RES_NOTRDY;

	if(count == 0)
		return RES_PARERR;
	
	switch(pdrv)
	{
	case PDRV_USB:
		do {
			res = USBH_MSC_Write10((BYTE*)buff, sector, 512*count);
		} while(res == USBH_MSC_BUSY);

		if(res == USBH_MSC_OK)
			return RES_OK;
		else
			return RES_ERROR;
			
	default:
		break;
	}
	
	return RES_ERROR;
}


DRESULT disk_ioctl(BYTE pdrv, BYTE ctrl, void *buff)
{
	DRESULT res = RES_OK;

	if(disk_status(pdrv) == STA_NOINIT)
		return RES_NOTRDY;

	switch(pdrv)
	{
	case PDRV_USB:
		switch(ctrl)
		{
		case CTRL_SYNC:
			break;

		case GET_SECTOR_COUNT:
			*((DWORD *)buff) = USBH_MSC_Info.Capacity;
			break;

		case GET_SECTOR_SIZE:
			*((WORD  *)buff) = USBH_MSC_Info.PageSize;
			break;

		case GET_BLOCK_SIZE:
			*((WORD  *)buff) = USBH_MSC_Info.PageSize;
			break;

		default:
			res = RES_PARERR;
			break;
		}
		break;
	
	default:
		res = RES_ERROR;
		break;
	}
	
	return res;
}

