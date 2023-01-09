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
#include "FlashDisk.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define DEV_SPI		3	//SPI Flash


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_status();

		// translate the reslut code here
	
		stat = STA_NOINIT;

		return stat;

	case DEV_MMC :
		//result = MMC_disk_status();

		// translate the reslut code here
	
		stat = STA_NOINIT;

		return stat;

	case DEV_USB :
		//result = USB_disk_status();

		// translate the reslut code here
	
		stat = STA_NOINIT;

		return stat;
	
	case DEV_SPI :
		result = SFC_ReadJEDEC();
		if((result != 0x000000) && (result != 0xFFFFFF))
			stat = 0;
		else
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
		
		stat = STA_NOINIT;
		
		return stat;

	case DEV_USB :
		//result = USB_disk_initialize();

		// translate the reslut code here
	
		stat = STA_NOINIT;

		return stat;
	
	case DEV_SPI :
	{
		SFC_InitStructure SFC_initStruct;
	
		/* SFC使用专用的FSPI（Flash SPI）接口连接SPI Flash */
		PORT_Init(PORTD, PIN5, PORTD_PIN5_FSPI_SCLK,  0);
		PORT_Init(PORTD, PIN6, PORTD_PIN6_FSPI_SSEL,  0);
		PORT_Init(PORTD, PIN8, PORTD_PIN8_FSPI_MOSI,  1);
		PORT_Init(PORTD, PIN7, PORTD_PIN7_FSPI_MISO,  1);
		PORT_Init(PORTD, PIN3, PORTD_PIN3_FSPI_DATA2, 1);
		PORT_Init(PORTD, PIN4, PORTD_PIN4_FSPI_DATA3, 1);
		
		SFC_initStruct.ClkDiv = SFC_CLKDIV_4;
		SFC_initStruct.Cmd_Read = 0xEB;
		SFC_initStruct.Width_Read = SFC_RDWIDTH_4;
		SFC_initStruct.Cmd_PageProgram = 0x32;
		SFC_initStruct.Width_PageProgram = SFC_PPWIDTH_4;
		SFC_Init(&SFC_initStruct);
		
		return disk_status(pdrv);
	}
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
	
		res = RES_PARERR;
		
		return res;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here
	
		res = RES_PARERR;

		return res;
	
	case DEV_SPI :
		FlashDiskRead(sector * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE * count, buff);
		return RES_OK;
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
	
		res = RES_PARERR;

		return res;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here
	
		res = RES_PARERR;

		return res;
	
	case DEV_SPI :
		FlashDiskWrite(sector * FLASH_SECTOR_SIZE, FLASH_SECTOR_SIZE * count, buff);
		return RES_OK;
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
	
		res = RES_PARERR;

		return res;

	case DEV_USB :

		// Process of the command the USB drive
	
		res = RES_PARERR;

		return res;
	
	case DEV_SPI :
		switch(cmd)
		{
        case CTRL_SYNC:
			FlashDiskFlush();
            return RES_OK;

        case GET_SECTOR_SIZE:
            *((WORD *)buff) = FLASH_SECTOR_SIZE;
            return RES_OK;
		
		case GET_SECTOR_COUNT:
            *((DWORD *)buff) = DATA_FLASH_SIZE / FLASH_SECTOR_SIZE;
            return RES_OK;

        case GET_BLOCK_SIZE:
            *((DWORD *)buff) = FLASH_BLOCK_SIZE / FLASH_SECTOR_SIZE; // erase block size in units of sector size
            return RES_OK;
		
        default:
            return RES_PARERR;
		}
	}

	return RES_PARERR;
}

