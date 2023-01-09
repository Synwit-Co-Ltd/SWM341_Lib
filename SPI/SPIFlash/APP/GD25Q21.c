/****************************************************************************************************************************************** 
* �ļ�����:	GD25Q21.c
* ����˵��:	GD25Q21 SPI Flash ��������
* ����֧��:	http://www.synwit.com.cn/e/tool/gbook/?bid=1
* ע������:
* �汾����: V1.0.0		2017��10��26��
* ������¼: 
*******************************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology  
*******************************************************************************************************************************************/
#include "SWM341.h"

#include "GD25Q21.h"


/****************************************************************************************************************************************** 
* ��������:	GD25Q21_Init()
* ����˵��:	��ʼ��GD25Q21ʹ�õ�SPI�ӿڣ�����ȡоƬID���ж����Ƿ���GD25Q21оƬ
* ��    ��: ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void GD25Q21_Init(void)
{
	uint16_t chip_id;
	uint8_t manufacture_id, device_id;
	SPI_InitStructure SPI_initStruct;
	
	GPIO_Init(GPIOM, PIN3, 1, 0, 0, 0);
#define GD25Q21_Assert()	GPIO_ClrBit(GPIOM, PIN3)
#define GD25Q21_Deassert()	GPIO_SetBit(GPIOM, PIN3)
	GD25Q21_Deassert();
	
	PORT_Init(PORTM, PIN2, PORTM_PIN2_SPI0_SCLK, 0);
	PORT_Init(PORTM, PIN4, PORTM_PIN4_SPI0_MISO, 1);	//��MOSI��MISO���ӣ��Է������ա�Ȼ���ӡ
	PORT_Init(PORTM, PIN5, PORTM_PIN5_SPI0_MOSI, 0);
	
	SPI_initStruct.clkDiv = SPI_CLKDIV_32;
	SPI_initStruct.FrameFormat = SPI_FORMAT_SPI;
	SPI_initStruct.SampleEdge = SPI_FIRST_EDGE;
	SPI_initStruct.IdleLevel = SPI_LOW_LEVEL;
	SPI_initStruct.WordSize = 8;
	SPI_initStruct.Master = 1;
	SPI_initStruct.RXThreshold = 0;
	SPI_initStruct.RXThresholdIEn = 0;
	SPI_initStruct.TXThreshold = 0;
	SPI_initStruct.TXThresholdIEn = 0;
	SPI_initStruct.TXCompleteIEn = 0;
	SPI_Init(SPI0, &SPI_initStruct);
	
	SPI_Open(SPI0);
	
	chip_id = GD25Q21_ReadChipID();
	manufacture_id = chip_id >> 8;
	device_id = chip_id & 0xFF;
	
	if((manufacture_id == GD25Q21_MANUFACTOR_ID) && (device_id == GD25Q21_DEVICE_ID))
	{
		printf("GD25Q21 found\r\n");
		
		while(GD25Q21_IsBusy()) __NOP();	// �ȴ�д��������
	}
	else
	{
		printf("Manufactor ID: %02X, Device ID: %02X, fail to identify\r\n", manufacture_id, device_id);
	}
}

/****************************************************************************************************************************************** 
* ��������:	GD25Q21_ReadChipID()
* ����˵��:	��ȡоƬID
* ��    ��: ��
* ��    ��: uint16_t			оƬID����16λ����8λΪ����ID����8λΪDevice ID
* ע������: ��
******************************************************************************************************************************************/
uint16_t GD25Q21_ReadChipID(void)
{
	uint8_t manufacture_id, device_id;
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_READ_CHIP_ID);
	SPI_ReadWrite(SPI0, 0x00);
	SPI_ReadWrite(SPI0, 0x00);
	SPI_ReadWrite(SPI0, 0x00);
	
	manufacture_id = SPI_ReadWrite(SPI0, 0xFF);
	device_id      = SPI_ReadWrite(SPI0, 0xFF);
	
	GD25Q21_Deassert();
	
	return (manufacture_id << 8) | device_id;
}

/****************************************************************************************************************************************** 
* ��������:	GD25Q21_ReadStatus()
* ����˵��:	��ȡоƬ״̬�Ĵ���
* ��    ��: ��
* ��    ��: uint16_t			оƬ״̬
* ע������: ��
******************************************************************************************************************************************/
uint16_t GD25Q21_ReadStatus(void)
{
	uint16_t status;
	
	GD25Q21_Assert();
		
	SPI_ReadWrite(SPI0, GD25Q21_CMD_READ_STATUS_L);
	status = SPI_ReadWrite(SPI0, 0xFF);
	
	GD25Q21_Deassert();
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_READ_STATUS_H);
	status |= SPI_ReadWrite(SPI0, 0xFF) << 8;
	
	GD25Q21_Deassert();
		
	return status;
}

/****************************************************************************************************************************************** 
* ��������:	GD25Q21_IsBusy()
* ����˵��:	оƬæ��⣬ִ�в�����д���������Ҫִ��æ������ж�оƬ�Ƿ��Ѿ���ɲ���
* ��    ��: ��
* ��    ��: uint32_t			1 оƬ����ִ���ڲ�������д�����    0 оƬδ��ִ���ڲ�����
* ע������: ��
******************************************************************************************************************************************/
uint32_t GD25Q21_IsBusy(void)
{
	return (GD25Q21_ReadStatus() & GD25Q21_STATUS_WIP_Msk) ? 1 : 0;
}

/****************************************************************************************************************************************** 
* ��������:	GD25Q21_EraseSector()
* ����˵��:	��������
* ��    ��: uint32_t addr		Ҫ�����������ĵ�ַ��ÿ����4K�ֽ�
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void GD25Q21_EraseSector(uint32_t addr)
{
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_WRITE_ENABLE);
	
	GD25Q21_Deassert();
	
	GD25Q21_Assert();
		
	SPI_ReadWrite(SPI0, GD25Q21_CMD_ERASE_SECTOR);
	SPI_ReadWrite(SPI0, (addr >> 16) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  8) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  0) & 0xFF);
	
	GD25Q21_Deassert();
}

/****************************************************************************************************************************************** 
* ��������:	GD25Q21_ReadData()
* ����˵��:	��GD25Q21ָ����ַ��ȡָ�����ֽ�����
* ��    ��: uint32_t addr		Ҫ��ȡ��������GD25Q21�ϵĵ�ַ
*			uint8_t buff[]		��ȡ�������ݴ洢��buff��
*			uint32_t cnt		Ҫ��ȡ���ݵ��ֽڸ���
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void GD25Q21_ReadData(uint32_t addr, uint8_t buff[], uint32_t cnt)
{
	uint32_t i;
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_READ_DATA);
	SPI_ReadWrite(SPI0, (addr >> 16) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  8) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  0) & 0xFF);
	
	for(i = 0; i < cnt; i++)
	{
		buff[i] = SPI_ReadWrite(SPI0, 0xFF);
	}
	
	GD25Q21_Deassert();
}

/****************************************************************************************************************************************** 
* ��������:	GD25Q21_WritePage()
* ����˵��:	��GD25Q21ָ����ַ��д��ָ�����ֽ�����
* ��    ��: uint32_t addr		����Ҫд�뵽GD25Q21��λ��
*			uint8_t data[]		Ҫд�������
*			uint16_t cnt		Ҫд������ݸ�����ÿҳ256�ֽڣ�ÿ��д������ݱ�����ͬһҳ�ĵ�ַ��Χ��
* ��    ��: ��
* ע������: ��
******************************************************************************************************************************************/
void GD25Q21_WritePage(uint32_t addr, uint8_t data[], uint16_t cnt)
{
	uint32_t i;
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_WRITE_ENABLE);
	
	GD25Q21_Deassert();
	
	GD25Q21_Assert();
	
	SPI_ReadWrite(SPI0, GD25Q21_CMD_WRITE_PAGE);
	SPI_ReadWrite(SPI0, (addr >> 16) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  8) & 0xFF);
	SPI_ReadWrite(SPI0, (addr >>  0) & 0xFF);
	
	for(i = 0; i < cnt; i++)
	{
		SPI_ReadWrite(SPI0, data[i]);
	}
	
	GD25Q21_Deassert();
}
