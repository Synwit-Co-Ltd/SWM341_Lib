2022/04/22
CSL\SWM341_StdPeriph_Driver\SWM341_flash.c 文件中，FLASH_Erase()、FLASH_Write() 改用 IAP 实现

2022/04/26
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，TIMR_TypeDef 定义中，修正 PREDIV 寄存器位置错误，导致定时器预分频不起作用问题

2022/04/28
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，UART_TypeDef 定义中，删除旧的 BRK 发送/检测功能寄存器

2022/05/10
CSL\SWM341_StdPeriph_Driver\SWM341_sdio.c 文件中，parseCSD() 定义中，将：
SD_cardInfo.CardCapacity = (SD_cardInfo.SD_csd.DeviceSize + 1) * 512 * 1024;
修正为：
SD_cardInfo.CardCapacity = (uint64_t)(SD_cardInfo.SD_csd.DeviceSize + 1) * 512 * 1024;

2022/05/11
CSL\SWM341_StdPeriph_Driver\SWM341_usbh.c 文件中，USBH_HW_Init() 定义中，将：
	USBD->DEVCR = (0 << USBD_DEVCR_DEVICE_Pos) |	// 主机模式
				  (1 << USBD_DEVCR_CSRDONE_Pos);
修改为：
	USBD->DEVCR = (0 << USBD_DEVCR_DEVICE_Pos) |	// 主机模式
				  (3 << USBD_DEVCR_SPEED_Pos)  |
				  (1 << USBD_DEVCR_CSRDONE_Pos);
同时删除 USBH_GetDeviceSpeed() 中对 USBD->DEVCR 的修改，解决上电时 USB Host 错误识别到 USB Device 插入的问题。

2022/05/24
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，SYS->HRCCR 寄存器删除 LDOON 位定义

2022/05/31
CSL\SWM341_StdPeriph_Driver\SWM341_can.c 文件中，CAN_SetBaudrate() 定义中，将：
uint32_t brp = SystemCoreClock/2/baudrate/(1 + (CAN_bs1 + 1) + (CAN_bs2 + 1)) - 1;
修正为：
uint32_t brp = (SystemCoreClock/2)/2/baudrate/(1 + (CAN_bs1 + 1) + (CAN_bs2 + 1)) - 1;

2022/06/13
CSL\SWM341_UsbHost_Lib\MSC\usbh_msc_scsi.c 文件中，将：
#define DATLEN_REQUEST_SENSE   		63
修改为：
#define DATLEN_REQUEST_SENSE   		18

USBH_MSC_RequestSense() 函数中，将：
USBH_CBWData.CBWCBLength = 10;
修改为：
USBH_CBWData.CBWCBLength = 6;

删除“USBH_BOTXfer.MSCStateBkp = USBH_BOTXfer.MSCState;”

解决 Test Unit Ready 出错执行 Request Sense 后，一直重复执行 Request Sense 无法退出问题

2022/06/15
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，SYS_TypeDef 定义中，USBPHYCR 寄存器添加 OPMODE 位定义

CSL\CMSIS\DeviceSupport\system_SWM341.c 文件中，SystemInit() 函数中，添加：
SYS->USBPHYCR &= ~SYS_USBPHYCR_OPMODE_Msk;
SYS->USBPHYCR |= (1 << SYS_USBPHYCR_OPMODE_Pos);  	//Non-Driving, DP Pull-Up disable

CSL\SWM341_StdPeriph_Driver\SWM341_usbd.c 文件中，USBD_Init() 函数中，添加：
SYS->USBPHYCR &= ~SYS_USBPHYCR_OPMODE_Msk;
SYS->USBPHYCR |= (0 << SYS_USBPHYCR_OPMODE_Pos);	//Normal Operation

2022/06/16
CSL\SWM341_StdPeriph_Driver\SWM341_usbd.c 文件中，USBD_Init() 函数中，添加：
SYS->USBCR = 0;
for(int i = 0; i < CyclesPerUs; i++) __NOP();

CSL\SWM341_StdPeriph_Driver\SWM341_usbh.c 文件中，USBH_HW_Init() 函数中，添加：
SYS->USBCR = 0;
for(int i = 0; i < CyclesPerUs; i++) __NOP();

USBH->CR = USBH_CR_FLUSHFF_Msk;

2022/06/17
CSL\SWM341_StdPeriph_Driver\SWM341_uart.c 文件中，UART_SetBaudrate() 函数中，将：
UARTx->BAUD &= ~UART_BAUD_BAUD_Msk;
修正为：
UARTx->BAUD &= ~(UART_BAUD_BAUD_Msk | UART_BAUD_FRAC_Msk);

2022/06/20
CSL\SWM341_StdPeriph_Driver\SWM341_sdio.c 文件中，SDIO_Init() 函数中，添加：
SD_cardInfo.CardBlockSize = 512;

2022/06/28
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，SYS_TypeDef 定义中，USBPHYCR 寄存器添加 IDEN、XCVR、PLLEN 位定义

CSL\SWM341_StdPeriph_Driver\SWM341_sdram.c 文件中，添加 SDRAM_Enable()、SDRAM_Disable() 函数定义

2022/06/30
CSL\SWM341_StdPeriph_Driver\SWM341_sfc.c 文件中
由于 SFC 硬件忙查询时长最大只有 128 ms，而 Flash 的扇区擦、块擦、片擦最大时长分别为 400 ms、800 ms、80 s，因此擦除函数 SFC_Erase()、SFC_EraseEx() 不再使用硬件忙查询

函数 SFC_Init() 中，将硬件忙查询间隔由 1 ms 改为 0.2 ms，可减小 SFC_Write() 的等待时间

2022/07/18
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，PWMG_TypeDef 定义中，寄存器的每个域都应该是 5 位，对应 5 个 PWM

2022/07/19
CSL\SWM341_StdPeriph_Driver\SWM341_sfc.h 文件中，将：
#define SFC_CMD_ERASE_BLOCK64KB		0xDB
修正为：
#define SFC_CMD_ERASE_BLOCK64KB		0xD8	//W25Q32

2022/07/25
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，IRQn_Type 定义中，将 PWMHALT_IRQn 修改为 PWMBRK_IRQn

CSL\CMSIS\DeviceSupport\startup\arm\startup_SWM341.s 文件中，将 PWMHALT_Handler 修改为 PWMBRK_Handler
CSL\CMSIS\DeviceSupport\startup\iar\startup_SWM341.s 文件中，将 PWMHALT_Handler 修改为 PWMBRK_Handler
CSL\CMSIS\DeviceSupport\startup\gcc\startup_SWM341.s 文件中，将 PWMHALT_Handler 修改为 PWMBRK_Handler

2022/07/26
CSL\CMSIS\DeviceSupport\system_SWM341.c 文件中，将：
#define __LSE		(   32000UL)		//低速外部时钟
修正为：
#define __LSE		(   32768UL)		//低速外部时钟

CSL\SWM341_StdPeriph_Driver\SWM341_rtc.c 文件中，RTC_Init() 函数中，将：
SYS->XTALCR |= (1 << SYS_XTALCR_32KON_Pos) | (1 << SYS_XTALCR_32KDRV_Pos);
修正为：
SYS->XTALCR |= (1 << SYS_XTALCR_32KON_Pos) | (7 << SYS_XTALCR_32KDRV_Pos);

2022/07/26
CSL\SWM341_StdPeriph_Driver\SWM341_jpeg.c 文件中，JPEG_Decode() 函数中，对 JPEG->IMGSTR 赋值运算中，所有被除数均加上“.0”变成浮点数，否则 ceil() 运算不起作用

2022/08/12
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，LCD_TypeDef 定义中，寄存器 CR 的 BURSTLEN 位定义由：
#define LCD_CR_BURSTLEN_Pos			21		//Burst Length，0 Burst INCR4   1 Burst INCR8   2 Burst INCR16
#define LCD_CR_BURSTLEN_Msk			(0x03 << LCD_CR_BURSTLEN_Pos)
修正为：
#define LCD_CR_BURSTLEN_Pos			9		//Burst Length，0 Burst INCR4   1 Burst INCR8
#define LCD_CR_BURSTLEN_Msk			(0x01 << LCD_CR_BURSTLEN_Pos)

CSL\SWM341_StdPeriph_Driver\SWM341_lcd.c 文件中，LCD_Init() 函数中，将：
(0                         << LCD_CR_BURSTLEN_Pos) |
修改为：
(1                         << LCD_CR_BURSTLEN_Pos) |

2022/08/19
CSL\SWM341_StdPeriph_Driver\SWM341_usbh.c 文件中，USBH_ReadRxBuffer() 定义中，将：
memcpy(buff, (uint8_t *)USBH->RXBUF, size);
替换为：
USBD_memcpy(buff, (uint8_t *)USBH->RXBUF, size);

2022/08/25
CSL\SWM341_UsbHost_Lib\usbh_core.c 文件中，USBH_HandleEnum() 定义中，将：
	if(USBH_GetDescriptor(phost, USB_DESC_STRING, phost->Device.Dev_Desc.iManufacturer, (uint8_t *)phost->Device.strVender, sizeof(phost->Device.strVender)) == USBH_OK)
    {
		phost->EnumState = ENUM_GET_PRODUCT_STRING_DESC;
		
        if(phost->usr_cb->VendorString)
            phost->usr_cb->VendorString(phost->Device.strVender);
    }
修改为：
	USBH_Status stat = USBH_GetDescriptor(phost, USB_DESC_STRING, phost->Device.Dev_Desc.iManufacturer, (uint8_t *)phost->Device.strVender, sizeof(phost->Device.strVender));
    if(stat == USBH_OK)
    {
		phost->EnumState = ENUM_GET_PRODUCT_STRING_DESC;
		
        if(phost->usr_cb->VendorString)
            phost->usr_cb->VendorString(phost->Device.strVender);
    }
	else if(stat == USBH_NOT_SUPPORTED)
	{
		phost->EnumState = ENUM_GET_PRODUCT_STRING_DESC;
	}
即，如果从机对获取 VENDOR_STRING 描述响应 STALL，则不再读取 VENDOR_STRING，对 PRODUCT_STRING、SERIALNUM_STRING 也做类似处理

2022/09/30
修改 CSL\SWM341_StdPeriph_Driver\SWM341_dma.c 和 SWM341_dma.h 文件，使用 Scatter-Gather 模式实现 ping-pong 功能

2022/10/11
CSL\SWM341_StdPeriph_Driver\SWM341_dac.c 文件中，DAC_Init() 函数中，添加：
	SYS->DACCR &= ~SYS_DACCR_VRADJ_Msk;
	SYS->DACCR |= ((SYS->BACKUP[2] & 0x1F) << SYS_DACCR_VRADJ_Pos);

CSL\CMSIS\DeviceSupport\SWM341.h 文件中，SYS_TypeDef 定义中，将：
#define SYS_ACMPCR2_HALL0_Pos		0		//1 ACMP0输出连接HALL0输入
#define SYS_ACMPCR2_HALL0_Msk		(0x01 << SYS_ACMPCR2_HALL0_Pos)
修正为：
#define SYS_ACMPCR2_BRK0_Pos		0		//1 ACMP0输出连接用作PWM_BRK0
#define SYS_ACMPCR2_BRK0_Msk		(0x01 << SYS_ACMPCR2_BRK0_Pos)
ACMPCR2 的 HALL0、HALL1 位做相同处理了

CSL\SWM341_StdPeriph_Driver\SWM341_dma.c 文件中，添加 DMA_CH_SetCount()、DMA_CH_SetSrcAddress()、DMA_CH_SetDstAddress()、DMA_CH_GetRemaining() 四个函数定义

CSL\SWM341_StdPeriph_Driver\SWM341_dma.c 文件中，DMA_CH_Init() 定义中，将：
((initStruct->Count - 1) << DMA_CR_LEN_Pos);
修改为：
((initStruct->Count ? initStruct->Count - 1 : 0) << DMA_CR_LEN_Pos);

2022/10/12
CSL\SWM341_StdPeriph_Driver\SWM341_uart.c 文件中，将：
void UART_INTRXThresholdEn(UART_TypeDef * UARTx);
void UART_INTRXThresholdDis(UART_TypeDef * UARTx);
uint32_t UART_INTRXThresholdStat(UART_TypeDef * UARTx);
void UART_INTTXThresholdEn(UART_TypeDef * UARTx);
void UART_INTTXThresholdDis(UART_TypeDef * UARTx);
uint32_t UART_INTTXThresholdStat(UART_TypeDef * UARTx);
void UART_INTTimeoutEn(UART_TypeDef * UARTx);
void UART_INTTimeoutDis(UART_TypeDef * UARTx);
uint32_t UART_INTTimeoutStat(UART_TypeDef * UARTx);
void UART_INTTXDoneEn(UART_TypeDef * UARTx);
void UART_INTTXDoneDis(UART_TypeDef * UARTx);
uint32_t UART_INTTXDoneStat(UART_TypeDef * UARTx);
简化为：
void UART_INTEn(UART_TypeDef * UARTx, uint32_t it);
void UART_INTDis(UART_TypeDef * UARTx, uint32_t it);
void UART_INTClr(UART_TypeDef * UARTx, uint32_t it);
uint32_t UART_INTStat(UART_TypeDef * UARTx, uint32_t it);

CSL\SWM341_StdPeriph_Driver\SWM341_uart.c 文件中，UART_Init() 函数中，将：
	UARTx->TOCR |= (initStruct->TimeoutTime << UART_TOCR_TIME_Pos);
修改为：
	UARTx->TOCR |= (1 << UART_TOCR_MODE_Pos) |
				   (initStruct->TimeoutTime << UART_TOCR_TIME_Pos);

CSL\SWM341_UsbHost_Lib\MTP\usbh_mtp.c 文件中，添加 Notify 端口读取

2022/10/19
CSL\SWM341_StdPeriph_Driver\SWM341_usbh.c 文件中，USBH_HW_Init() 函数中，将：
	SYS->USBCR |= (1 << SYS_USBCR_RSTPLL_Pos); __DSB();
修改为：
	SYS->USBCR |= (1 << SYS_USBCR_RSTPLL_Pos); __DSB();
	for(int i = 0; i < CyclesPerUs; i++) __NOP();
即在退出复位状态后不要立即访问 USB 寄存器，而是延时一下再访问，防止编译器优化等级设为 Ofast 时芯片卡死

2022/10/21
CSL\SWM341_StdPeriph_Driver\SWM341_can.h 文件中，将：
#define CAN_INT_RX_NOTEMPTY	(0x01 << 0)		//RX Buffer Not Empty
#define CAN_INT_RX_OVERFLOW	(0x01 << 3)		//RX Buffer Overflow
#define CAN_INT_TX_EMPTY	(0x01 << 1)		//TX Buffer Empty
#define CAN_INT_ARBLOST		(0x01 << 6)		//Arbitration lost
#define CAN_INT_ERR			(0x01 << 7)
#define CAN_INT_ERR_WARN	(0x01 << 2)		//TXERR/RXERR计数值达到Error Warning Limit
#define CAN_INT_ERR_PASS	(0x01 << 5)		//TXERR/RXERR计数值达到127
#define CAN_INT_WAKEUP		(0x01 << 4)
修改为：
#define CAN_IT_RX_NOTEMPTY	(0x01 << 0)		//RX Buffer Not Empty
#define CAN_IT_RX_OVERFLOW	(0x01 << 3)		//RX Buffer Overflow
#define CAN_IT_TX_EMPTY		(0x01 << 1)		//TX Buffer Empty
#define CAN_IT_ARBLOST		(0x01 << 6)		//Arbitration lost
#define CAN_IT_ERR			(0x01 << 7)
#define CAN_IT_ERR_WARN		(0x01 << 2)		//TXERR/RXERR计数值达到Error Warning Limit
#define CAN_IT_ERR_PASS		(0x01 << 5)		//TXERR/RXERR计数值达到127
#define CAN_IT_WAKEUP		(0x01 << 4)

2022/11/02
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，ADC_TypeDef 定义中，将：
#define ADC_GO_SEQ2_Msk				(0x01 << ADC_GO_SEQ3_Pos)
修正为：
#define ADC_GO_SEQ2_Msk				(0x01 << ADC_GO_SEQ2_Pos)

2022/11/09
CSL\CMSIS\DeviceSupport\SWM341.h 文件中，DMA2D_TypeDef 定义中，将：
	DAM2D_PFCCR_AMODE_Pos
修正为：
	DMA2D_PFCCR_AMODE_Pos

CSL\SWM341_StdPeriph_Driver\SWM341_dma2d.c 文件中，DMA2D_PixelBlend() 函数中，将：
	(fgLayer->AlphaMode << DAM2D_PFCCR_AMODE_Pos) |
	(bgLayer->AlphaMode << DAM2D_PFCCR_AMODE_Pos) |
修正为：
	(fgLayer->AlphaMode << DMA2D_PFCCR_AINV_Pos) |
	(bgLayer->AlphaMode << DMA2D_PFCCR_AINV_Pos) |

2022/11/30
CSL\SWM341_StdPeriph_Driver\SWM341_sfc.c 文件中，SFC_Init() 函数中，添加：
	if((initStruct->Width_Read == SFC_RDWIDTH_4) || (initStruct->Width_PageProgram == SFC_PPWIDTH_4))
	{
		if(SFC_QuadState() == 0)
			SFC_QuadSwitch(1);
	}
四线读写时，置位 SPI Flash 的 QE 位

2022/12/06
CSL\SWM341_StdPeriph_Driver\SWM341_sfc.c 文件中，SFC->GO = 1 后的等待全部改为：
	__DSB(); __ISB();
	while(SFC->GO) __NOP();

2023/02/08
CSL\SWM341_StdPeriph_Driver\SWM341_spi.c 文件中，I2S_Init() 函数中，添加 SPIx->I2SCR.CHLEN 设置
