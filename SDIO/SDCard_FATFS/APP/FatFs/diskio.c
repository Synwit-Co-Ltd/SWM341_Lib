/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

#include "SWM341.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

static int sd_initialized = 0;

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	//int result;

	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_status();

		// translate the reslut code here
	
		stat = STA_NOINIT;

		return stat;

	case DEV_MMC :
		//result = MMC_disk_status();

		// translate the reslut code here
	
		if(sd_initialized)
			stat = RES_OK;
		else
			stat = STA_NOINIT;

		return stat;

	case DEV_USB :
		//result = USB_disk_status();

		// translate the reslut code here
	
		stat = STA_NOINIT;

		return stat;
	}
	
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_initialize();

		// translate the reslut code here
	
		stat = STA_NOINIT;

		return stat;

	case DEV_MMC :
		//result = MMC_disk_initialize();

		// translate the reslut code here
	
		PORT_Init(PORTM, PIN2, PORTM_PIN2_SD_CLK, 0);
		PORT_Init(PORTM, PIN4, PORTM_PIN4_SD_CMD, 1);
		PORT_Init(PORTM, PIN5, PORTM_PIN5_SD_D0,  1);
		PORT_Init(PORTM, PIN6, PORTM_PIN6_SD_D1,  1);
		PORT_Init(PORTN, PIN0, PORTN_PIN0_SD_D2,  1);
		PORT_Init(PORTN, PIN1, PORTN_PIN1_SD_D3,  1);
		
		result = SDIO_Init(10000000);
		if(result == SD_RES_OK)
		{
			stat = RES_OK;
			
			sd_initialized = 1;
		}
		else
		{
			stat = STA_NOINIT;
			
			sd_initialized = 0;
		}

		return stat;

	case DEV_USB :
		//result = USB_disk_initialize();

		// translate the reslut code here
	
		stat = STA_NOINIT;

		return stat;
	}
	
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here
	
		res = RES_PARERR;

		return res;

	case DEV_MMC :
		// translate the arguments here

		//result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here
	
		if(count == 1)
		{
			result = SDIO_BlockRead(sector, (uint32_t *)buff);
		}
		else
		{
			result = SDIO_MultiBlockRead(sector, count, (uint32_t *)buff);
		}
	
		if(result == SD_RES_OK)
			res = RES_OK;
		else
			res = RES_ERROR;
		
		return res;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here
	
		res = RES_PARERR;

		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here
	
		res = RES_PARERR;

		return res;

	case DEV_MMC :
		// translate the arguments here

		//result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here
	
		if(count == 1)
		{
			result = SDIO_BlockWrite(sector, (uint32_t *)buff);
		}
		else
		{
			result = SDIO_MultiBlockWrite(sector, count, (uint32_t *)buff);
		}
		
		if(result == SD_RES_OK)
			res = RES_OK;
		else
			res = RES_ERROR;

		return res;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here
	
		res = RES_PARERR;

		return res;
	}

	return RES_PARERR;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
	//int result;

	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive
	
		res = RES_PARERR;

		return res;

	case DEV_MMC :

		// Process of the command for the MMC/SD card
	
		res = RES_OK;

		return res;

	case DEV_USB :

		// Process of the command the USB drive
	
		res = RES_PARERR;

		return res;
	}

	return RES_PARERR;
}

