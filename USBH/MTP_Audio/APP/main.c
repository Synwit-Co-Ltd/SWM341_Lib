#include <stdlib.h>
#include <string.h>
#include "SWM341.h"
#include "usbh_core.h"
#include "usbh_mtp.h"


/* As a USB Host, read the first Wav audio file in the root directory of the Android phone through the MTP protocol and play it through the DAC */


extern USBH_Class_cb_t USBH_MTP_cb;
extern USBH_User_cb_t  USBH_USR_cb;

extern USBH_Info_t USBH_Info;
USBH_Info_t *pHost = &USBH_Info;


typedef struct {
	uint32_t ChunkID;
	uint32_t FileSize;
	uint32_t FileFormat;
	uint32_t SubChunk1ID;
	uint32_t SubChunk1Size;
	uint16_t AudioFormat;
	uint16_t NbrChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t BlockAlign;
	uint16_t BitPerSample;
	uint32_t SubChunk2ID;
	uint32_t SubChunk2Size;
} WAV_Info_t;

WAV_Info_t WavInfo;
uint32_t WavHandle;
uint32_t StorageID;

#define BUFLEN  1024
uint16_t ReadBuffer[BUFLEN*2];
uint16_t PlayBuffer[2][BUFLEN];
volatile uint8_t PlayingBuffer = 0;
volatile uint8_t FillingBuffer = 0;
volatile uint8_t Playing = 0;

void SerialInit(void);
void DAC_Config(void);

int main(void)
{
	USBH_Status status;
	uint32_t BytesRead;		// Number of bytes of data you want to read
	uint32_t bytesread;		// Number of bytes of data actually read
	
	SystemInit();
	
	SerialInit();
	
	USBH_Init(&USBH_MTP_cb, &USBH_USR_cb);
	
	SysTick_Config(CyclesPerUs * 100);
	
	while(!USBH_MTP_Ready()) __NOP();
	
	StorageID = USBH_MTP_Storage(0);
	
	uint32_t nObjs;
	while((status = USBH_MTP_GetNumObjects(pHost, StorageID, PTP_OFC_WAV, 0xFFFFFFFF, &nObjs)) == USBH_BUSY) USBH_TObreak(1000);
	if(status != USBH_OK) goto fail;
	printf("Wave file count: %d\n", nObjs);
	
	if(nObjs == 0) goto fail;
	
	PTP_ObjectHandles_t WavHandles;
	while((status = USBH_MTP_GetObjectHandles(pHost, StorageID, PTP_OFC_WAV, 0xFFFFFFFF, &WavHandles)) == USBH_BUSY) USBH_TObreak(1000);
	if(status != USBH_OK) goto fail;
	WavHandle = WavHandles.Handler[0];
	
	PTP_ObjectInfo_t ObjInfo;
	while((status = USBH_MTP_GetObjectInfo(pHost, WavHandle, &ObjInfo)) == USBH_BUSY) USBH_TObreak(1000);
	if(status != USBH_OK) goto fail;
	printf("Playing file: %s\n", ObjInfo.Filename);
	
	while((status = USBH_MTP_GetPartialObject(pHost, WavHandle, 0, sizeof(WAV_Info_t), (uint8_t*)&WavInfo, &bytesread)) == USBH_BUSY) USBH_TObreak(1000);
	if(status != USBH_OK) goto fail;
	printf("Sample rate : %u Hz\n", WavInfo.SampleRate);
	printf("Channels number : %d\n", WavInfo.NbrChannels);
	printf("Bit Per Sample: %d\n", WavInfo.BitPerSample);
	uint32_t duration = WavInfo.FileSize / WavInfo.ByteRate;
	printf("File Size : %u MB [%02u:%02u]\n", WavInfo.FileSize/1024/1024, duration/60, duration%60);
	
	if((WavInfo.BitPerSample < 9) || (WavInfo.BitPerSample > 16))
	{
		printf("BitPerSample Not Support\n");
		goto fail;
	}
	
	if(WavInfo.NbrChannels > 2)
	{
		printf("NbrChannels Not Support\n");
		goto fail;
	}
	
	/* Skip other chunks of the Wave header */
	uint32_t offset = sizeof(WAV_Info_t);
	BytesRead = WavInfo.SubChunk2Size + 8;	// Read the ID and Size of the next chunk
	while(1)
	{
		while((status = USBH_MTP_GetPartialObject(pHost, WavHandle, offset, BytesRead, (uint8_t *)ReadBuffer, &bytesread)) == USBH_BUSY) USBH_TObreak(1000);
		if(status != USBH_OK) goto fail;
		
		offset += BytesRead;
		
		if(strcmp((const char *)ReadBuffer + (BytesRead - 8), "data") == 0)
			break;
		
		BytesRead = *((uint32_t *)((uint8_t *)ReadBuffer + (BytesRead - 4)));
	}
	
	printf("Wave Data start from offset = %d\n", offset);
	
	BytesRead = 2048 * WavInfo.NbrChannels;
	while(offset < WavInfo.FileSize)
	{
		while((status = USBH_MTP_GetPartialObject(pHost, WavHandle, offset, BytesRead, (uint8_t *)ReadBuffer, &bytesread)) == USBH_BUSY) USBH_TObreak(1000);
		if(status != USBH_OK)
			goto fail;

#if 0	// Print the audio data and check that it is correct
		if(offset / BytesRead == 100)
		{
			printf("Wave Data @ %d is:\n", offset);
			for(int i = 0; i < BytesRead; i++)
			{
				printf("%04X, ", ReadBuffer[i]);
				if(i % 16 == 15) printf("\n");
			}
		}
#endif
		
		while(FillingBuffer != PlayingBuffer) __NOP();	// Wait for last FillingBuffer to be used
		FillingBuffer = 1 - PlayingBuffer;
		
		for(int i = 0; i < BUFLEN; i++)
			PlayBuffer[FillingBuffer][i] = ReadBuffer[i * WavInfo.NbrChannels];
		
		if(Playing == 0)
		{
			Playing = 1;
			
			PlayingBuffer = FillingBuffer;
			
			DAC_Config();
		}
		
		offset += BytesRead;
		
		printf(".");
	}
	printf("Play Done\n");
	
	Playing = 0;
	
	while(1==1)
	{
	}
	
fail:
	printf("fail\n");
	while(1==1)
	{
	}
}


void SysTick_Handler(void)
{
	USBH_Process();
}


void DAC_Config(void)
{
	DMA_InitStructure DMA_initStruct;
	
	PORT_Init(PORTD, PIN2, PORTD_PIN2_DAC_OUT, 0);
	
	DAC_Init(DAC, DAC_FORMAT_MSB12B);
	DAC_Open(DAC);
	
	DMA_initStruct.Mode = DMA_MODE_SINGLE;
	DMA_initStruct.Unit = DMA_UNIT_HALFWORD;
	DMA_initStruct.Count = BUFLEN;
	DMA_initStruct.SrcAddr = (uint32_t)PlayBuffer[PlayingBuffer];
	DMA_initStruct.SrcAddrInc = 1;
	DMA_initStruct.DstAddr = (uint32_t)&DAC->DHR;
	DMA_initStruct.DstAddrInc = 0;
	DMA_initStruct.Handshake = DMA_EXHS_TIMR0;
	DMA_initStruct.Priority = DMA_PRI_LOW;
	DMA_initStruct.INTEn = DMA_IT_DONE;
	
	TIMR_Init(TIMR0, TIMR_MODE_TIMER, CyclesPerUs, 1000000 / WavInfo.SampleRate, 0);
	TIMR_Start(TIMR0);

	DMA_CH_Init(DMA_CH0, &DMA_initStruct);
	DMA_CH_Open(DMA_CH0);
}


void DMA_Handler(void)
{
	DMA_CH_INTClr(DMA_CH0, DMA_IT_DONE);
	
	if(Playing)
	{
		PlayingBuffer = FillingBuffer;
		DMA->CH[DMA_CH0].SRC = (uint32_t)PlayBuffer[PlayingBuffer];
		
		DMA_CH_Open(DMA_CH0);
	}
}


void SerialInit(void)
{
	UART_InitStructure UART_initStruct;
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);
	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 0;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;
	UART_initStruct.TimeoutIEn = 0;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}

int fputc(int ch, FILE *f)
{
	UART_WriteByte(UART0, ch);
	
	while(UART_IsTXBusy(UART0)) __NOP();
 	
	return ch;
}
