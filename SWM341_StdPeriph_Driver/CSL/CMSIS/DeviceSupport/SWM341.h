#ifndef __SWM341_H__
#define __SWM341_H__

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers **********************************************/
  NonMaskableInt_IRQn     = -14,    /*!< 2 Non Maskable Interrupt                        */
  MemoryManagement_IRQn   = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt         */
  BusFault_IRQn           = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                 */
  UsageFault_IRQn         = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt               */
  SVCall_IRQn             = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                  */
  DebugMonitor_IRQn       = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt            */
  PendSV_IRQn             = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                  */
  SysTick_IRQn            = -1,     /*!< 15 Cortex-M4 System Tick Interrupt              */
  
/******  Cortex-M4 specific Interrupt Numbers ************************************************/
  UART0_IRQn              = 0,
  UART1_IRQn              = 1,
  TIMR0_IRQn              = 2,
  TIMR1_IRQn              = 3,
  DMA_IRQn                = 4,
  SPI0_IRQn               = 5,
  PWM0_IRQn               = 6,
  WDT_IRQn                = 7,
  UART2_IRQn              = 8,
  PWM1_IRQn               = 9,
  ADC0_IRQn               = 10,
  BTIMR0_IRQn             = 11,
  HALL0_IRQn              = 12,
  PWM2_IRQn               = 13,
  PWMBRK_IRQn             = 14,
  I2C0_IRQn               = 15,
  CAN0_IRQn               = 16,
  SPI1_IRQn               = 17,
  RTC_IRQn                = 18,
  PWM3_IRQn               = 19,
  TIMR2_IRQn              = 20,
  UART3_IRQn              = 21,
  TIMR3_IRQn              = 22,
  ADC1_IRQn               = 23,
  BOD_IRQn                = 24,
  CORDIC_IRQn             = 25,
  BTIMR1_IRQn             = 26,
  PWM4_IRQn               = 27,
  HALL3_IRQn              = 28,
  BTIMR2_IRQn             = 29,
  I2C1_IRQn               = 30,
  BTIMR3_IRQn             = 31,
  ACMP_IRQn               = 32,
  XTALSTOP_IRQn           = 33,
  FSPI_IRQn               = 34,
  GPIOA_IRQn              = 35,
  GPIOB_IRQn              = 36,
  GPIOC_IRQn              = 37,
  GPIOD_IRQn              = 38,
  GPIOM_IRQn              = 39,
  GPION_IRQn              = 40,
  GPIOA0_IRQn             = 41,
  GPIOA1_IRQn             = 42,
  GPIOA5_IRQn             = 43,
  GPIOA6_IRQn             = 44,
  GPIOA10_IRQn            = 45,
  GPIOA11_IRQn            = 46,
  GPIOA12_IRQn            = 47,
  GPIOA13_IRQn            = 48,
  GPIOB0_IRQn             = 49,
  GPIOB1_IRQn             = 50,
  GPIOB2_IRQn             = 51,
  GPIOC0_IRQn             = 52,
  GPIOC1_IRQn             = 53,
  GPIOC2_IRQn             = 54,
  GPIOC3_IRQn             = 55,
  GPIOC4_IRQn             = 56,
  GPIOD3_IRQn             = 57,
  GPIOD4_IRQn             = 58,
  GPIOD5_IRQn             = 59,
  GPIOD6_IRQn             = 60,
  GPIOD7_IRQn             = 61,
  GPIOD8_IRQn             = 62,
  GPIOC9_IRQn             = 63,
  GPIOC10_IRQn            = 64,
  GPIOC11_IRQn            = 65,
  GPIOC12_IRQn            = 66,
  GPIOM0_IRQn             = 67,
  GPIOM1_IRQn             = 68,
  GPIOM2_IRQn             = 69,
  GPIOM3_IRQn             = 70,
  GPIOM4_IRQn             = 71,
  DIV_IRQn                = 72,
  LCD_IRQn                = 73,
  GPIOE_IRQn              = 74,
  JPEG_IRQn               = 75,
  SDIO_IRQn               = 76,
  USB_IRQn                = 77,
  CAN1_IRQn				  = 78,
  TIMR4_IRQn			  = 79,
  BTIMR4_IRQn			  = 80,
  BTIMR5_IRQn			  = 81,
  BTIMR6_IRQn			  = 82,
  BTIMR7_IRQn			  = 83,
  BTIMR8_IRQn			  = 84,
  BTIMR9_IRQn			  = 85,
  BTIMR10_IRQn			  = 86,
  BTIMR11_IRQn			  = 87,
  DMA2D_IRQn              = 88,
  QEI_IRQn				  = 90,
} IRQn_Type;

/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M4 Processor and Core Peripherals */
#define __CM33_REV                0x0000  /*!< Core revision r0p1                            */
#define __NVIC_PRIO_BITS          3       /*!< SWM341 uses 3 Bits for the Priority Levels    */
#define __Vendor_SysTickConfig    0       /*!< Set to 1 if different SysTick Config is used  */
#define __DSP_PRESENT             1		  /*!< DSP extension present                         */
#define __FPU_PRESENT             1       /*!< FPU present                                   */
#define __MPU_PRESENT             1       /*!< MPU present                                   */

#if   defined ( __CC_ARM )
  #pragma anon_unions
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "core_cm33.h"
#include "system_SWM341.h"


/******************************************************************************/
/*				Device Specific Peripheral registers structures			 */
/******************************************************************************/
typedef struct {
	__IO uint32_t CLKSEL;				    // Clock Select

	__IO uint32_t CLKDIVx_ON;				// [0] CLK_DIVx clock source switch

	__IO uint32_t CLKEN0;					// Clock Enable
	
	__IO uint32_t CLKEN1;

	__IO uint32_t SLEEP;
	
		 uint32_t RESERVED[4];
	
	__IO uint32_t RSTSR;					// Reset Status
	
		 uint32_t RESERVED2[2];
	
	__IO uint32_t RTCWKCR;					// RTC Wakeup Control
	
	__IO uint32_t RTCWKSR;
	
		 uint32_t RESERVED3[14];
	
	__IO uint32_t USBPHYCR;
	
	__IO uint32_t USBCR;
	
		 uint32_t RESERVED4[2];
		
	__I  uint32_t CHIPID[4];
	
	__IO uint32_t BACKUP[4];				// Data Backup Register
	
		 uint32_t RESERVED5[12];
	
	__IO uint32_t PRNGCR;
	__IO uint32_t PRNGDL;					// PRNG Data Low Word
	__IO uint32_t PRNGDH;
	
		 uint32_t RESERVED6[9];
	
	__IO uint32_t PAWKEN;				    // PORTA Wakeup Enable
	__IO uint32_t PBWKEN;
	__IO uint32_t PCWKEN;
	__IO uint32_t PDWKEN;
	__IO uint32_t PEWKEN;
	
         uint32_t RESERVED7[3];
	
	__IO uint32_t PMWKEN;
	__IO uint32_t PNWKEN;
	
	     uint32_t RESERVED8[2];

	__IO uint32_t PAWKSR;				    // PORTA Wakeup Status, Write 1 to clear
	__IO uint32_t PBWKSR;
	__IO uint32_t PCWKSR;
	__IO uint32_t PDWKSR;
	__IO uint32_t PEWKSR;
	
		 uint32_t RESERVED9[3];
	
	__IO uint32_t PMWKSR;
	__IO uint32_t PNWKSR;
	
		 uint32_t RESERVED10[(0x400-0x154)/4-1];
	
	__IO uint32_t IOFILT0;					// IO Filter 0
	__IO uint32_t IOFILT1;

	uint32_t RESERVED11[(0x720-0x404)/4-1];
	
	__IO uint32_t PRSTEN;					// Peripheral reset enabled. PRSTR0 and PRSTR1 can be written only when the value of PRSTEN is 0x55
	__IO uint32_t PRSTR0;
	__IO uint32_t PRSTR1;
	
    //Analog Control: 0x400AA000
         uint32_t RESERVED12[(0x400AA000-0x40000728)/4-1];
	
	__IO uint32_t HRCCR;					// High speed RC Control Register
		 uint32_t RESERVED13[3];
    
    __IO uint32_t BODCR;
	__IO uint32_t BODSR;
	
	__IO uint32_t ADCCR;
	
		 uint32_t RESERVED14;
	
	__IO uint32_t XTALCR;
	__IO uint32_t XTALSR;
	
		 uint32_t RESERVED15[6];
	
	__IO uint32_t PLLCR;
    __IO uint32_t PLLDIV;
		 uint32_t RESERVED16;
    __IO uint32_t PLLLOCK;                  // [0] 1 PLL locked
	
    __IO uint32_t LRCCR;					// Low speed RC Control Register
   
         uint32_t RESERVED17[7];
	
	__IO uint32_t OPACR;					// OPA Control Register
	
		 uint32_t RESERVED18[3];
	
	__IO uint32_t ACMPCR;					// Analog Comparator Control Register
	__IO uint32_t ACMPSR;					// Analog Comparator Status Register
	__IO uint32_t ACMPCR2;
	
		 uint32_t RESERVED19;
		
	__IO uint32_t DACCR;
	
		 uint32_t RESERVED20;
	
	__IO uint32_t TEMPCR;					// Temperature Sensor
} SYS_TypeDef;


#define SYS_CLKSEL_SYS_Pos			0		// System clock select, 1 HRC, 0 CLK_DIVx
#define SYS_CLKSEL_SYS_Msk			(0x01 << SYS_CLKSEL_SYS_Pos)
#define SYS_CLKSEL_CLK_DIVx_Pos		1		// CLK_DIVx clock select, 0 CLK_DIV1, 1 CLK_DIV8
#define SYS_CLKSEL_CLK_DIVx_Msk		(0x01 << SYS_CLKSEL_CLK_DIVx_Pos)
#define SYS_CLKSEL_CLK_Pos			2		// Clock Source, 0 LRC, 1 PLL, 2 XTAL_32K, 3 XTAL, 4 HRC
#define SYS_CLKSEL_CLK_Msk			(0x07 << SYS_CLKSEL_CLK_Pos)
#define SYS_CLKSEL_RTC_Pos			5		// RTC clock select, 0 LRC, 1 XTAL_32K
#define SYS_CLKSEL_RTC_Msk			(0x01 << SYS_CLKSEL_RTC_Pos)
#define SYS_CLKSEL_IOFILT_Pos		6		// IO Filter clock select, 0 HRC, 2 XTAL, 3 LRC
#define SYS_CLKSEL_IOFILT_Msk		(0x03 << SYS_CLKSEL_IOFILT_Pos)
#define SYS_CLKSEL_SDIO_Pos			10		// SDIO clock select, 0 SYSCLK/2, 1 SYSCLK/8, 2 SYSCLK/4, 3 SYSCLK
#define SYS_CLKSEL_SDIO_Msk			(0x03 << SYS_CLKSEL_SDIO_Pos)
#define SYS_CLKSEL_WDT_Pos			12		// WDT clock select, 0 HRC, 1 XTAL, 2 LRC, 3 XTAL_32K
#define SYS_CLKSEL_WDT_Msk			(0x03 << SYS_CLKSEL_WDT_Pos)
#define SYS_CLKSEL_AD0_Pos			16		// ADC0 clock select, 0 HRC, 1 XTAL, 2 PLL
#define SYS_CLKSEL_AD0_Msk			(0x03 << SYS_CLKSEL_AD0_Pos)
#define SYS_CLKSEL_AD0DIV_Pos		18		// ADC0 clock divider, 0 div1, 1 div2, 2 div4, 3 div8
#define SYS_CLKSEL_AD0DIV_Msk		(0x03 << SYS_CLKSEL_AD0DIV_Pos)
#define SYS_CLKSEL_AD1_Pos			20
#define SYS_CLKSEL_AD1_Msk			(0x03 << SYS_CLKSEL_AD1_Pos)
#define SYS_CLKSEL_AD1DIV_Pos		22
#define SYS_CLKSEL_AD1DIV_Msk		(0x03 << SYS_CLKSEL_AD1DIV_Pos)
#define SYS_CLKSEL_SLEEP_Pos		24		// sleep clock, 0 LRC, 1 XTAL_32K
#define SYS_CLKSEL_SLEEP_Msk		(0x01 << SYS_CLKSEL_SLEEP_Pos)

#define SYS_CLKDIV_ON_Pos           0
#define SYS_CLKDIV_ON_Msk           (0x01 << SYS_CLKDIV_ON_Pos)

#define SYS_CLKEN0_GPIOA_Pos		0
#define SYS_CLKEN0_GPIOA_Msk		(0x01 << SYS_CLKEN0_GPIOA_Pos)
#define SYS_CLKEN0_GPIOB_Pos		1
#define SYS_CLKEN0_GPIOB_Msk		(0x01 << SYS_CLKEN0_GPIOB_Pos)
#define SYS_CLKEN0_GPIOC_Pos		2
#define SYS_CLKEN0_GPIOC_Msk		(0x01 << SYS_CLKEN0_GPIOC_Pos)
#define SYS_CLKEN0_GPIOD_Pos		3
#define SYS_CLKEN0_GPIOD_Msk		(0x01 << SYS_CLKEN0_GPIOD_Pos)
#define SYS_CLKEN0_GPIOM_Pos		4
#define SYS_CLKEN0_GPIOM_Msk		(0x01 << SYS_CLKEN0_GPIOM_Pos)
#define SYS_CLKEN0_GPION_Pos		5
#define SYS_CLKEN0_GPION_Msk		(0x01 << SYS_CLKEN0_GPION_Pos)
#define SYS_CLKEN0_UART0_Pos		6
#define SYS_CLKEN0_UART0_Msk		(0x01 << SYS_CLKEN0_UART0_Pos)
#define SYS_CLKEN0_UART1_Pos		7
#define SYS_CLKEN0_UART1_Msk		(0x01 << SYS_CLKEN0_UART1_Pos)
#define SYS_CLKEN0_UART2_Pos		8
#define SYS_CLKEN0_UART2_Msk		(0x01 << SYS_CLKEN0_UART2_Pos)
#define SYS_CLKEN0_UART3_Pos		9
#define SYS_CLKEN0_UART3_Msk		(0x01 << SYS_CLKEN0_UART3_Pos)
#define SYS_CLKEN0_WDT_Pos			10
#define SYS_CLKEN0_WDT_Msk			(0x01 << SYS_CLKEN0_WDT_Pos)
#define SYS_CLKEN0_TIMR_Pos			11
#define SYS_CLKEN0_TIMR_Msk			(0x01 << SYS_CLKEN0_TIMR_Pos)
#define SYS_CLKEN0_PWM_Pos			12
#define SYS_CLKEN0_PWM_Msk			(0x01 << SYS_CLKEN0_PWM_Pos)
#define SYS_CLKEN0_SPI0_Pos			13
#define SYS_CLKEN0_SPI0_Msk			(0x01 << SYS_CLKEN0_SPI0_Pos)
#define SYS_CLKEN0_SPI1_Pos			14
#define SYS_CLKEN0_SPI1_Msk			(0x01 << SYS_CLKEN0_SPI1_Pos)
#define SYS_CLKEN0_I2C0_Pos			15
#define SYS_CLKEN0_I2C0_Msk			(0x01 << SYS_CLKEN0_I2C0_Pos)
#define SYS_CLKEN0_I2C1_Pos			16
#define SYS_CLKEN0_I2C1_Msk			(0x01 << SYS_CLKEN0_I2C1_Pos)
#define SYS_CLKEN0_CRC_Pos			19
#define SYS_CLKEN0_CRC_Msk			(0x01 << SYS_CLKEN0_CRC_Pos)
#define SYS_CLKEN0_CORDIC_Pos		20
#define SYS_CLKEN0_CORDIC_Msk		(0x01 << SYS_CLKEN0_CORDIC_Pos)
#define SYS_CLKEN0_DIV_Pos			21
#define SYS_CLKEN0_DIV_Msk			(0x01 << SYS_CLKEN0_DIV_Pos)
#define SYS_CLKEN0_SDIO_Pos			22
#define SYS_CLKEN0_SDIO_Msk			(0x01 << SYS_CLKEN0_SDIO_Pos)
#define SYS_CLKEN0_USB_Pos			24
#define SYS_CLKEN0_USB_Msk			(0x01 << SYS_CLKEN0_USB_Pos)
#define SYS_CLKEN0_ANAC_Pos			25		// analog control unit clock enable
#define SYS_CLKEN0_ANAC_Msk			(0x01 << SYS_CLKEN0_ANAC_Pos)
#define SYS_CLKEN0_ADC0_Pos			26
#define SYS_CLKEN0_ADC0_Msk			(0x01 << SYS_CLKEN0_ADC0_Pos)
#define SYS_CLKEN0_CAN0_Pos			28
#define SYS_CLKEN0_CAN0_Msk			(0x01 << SYS_CLKEN0_CAN0_Pos)
#define SYS_CLKEN0_DMA2D_Pos		29
#define SYS_CLKEN0_DMA2D_Msk		(0x01 << SYS_CLKEN0_DMA2D_Pos)
#define SYS_CLKEN0_LCD_Pos			30
#define SYS_CLKEN0_LCD_Msk			(0x01 << SYS_CLKEN0_LCD_Pos)

#define SYS_CLKEN1_GPIOE_Pos		0
#define SYS_CLKEN1_GPIOE_Msk		(0x01 << SYS_CLKEN1_GPIOE_Pos)
#define SYS_CLKEN1_SPI2_Pos			8
#define SYS_CLKEN1_SPI2_Msk			(0x01 << SYS_CLKEN1_SPI2_Pos)
#define SYS_CLKEN1_SDRAM_Pos		12
#define SYS_CLKEN1_SDRAM_Msk		(0x01 << SYS_CLKEN1_SDRAM_Pos)
#define SYS_CLKEN1_SFC_Pos			13
#define SYS_CLKEN1_SFC_Msk			(0x01 << SYS_CLKEN1_SFC_Pos)
#define SYS_CLKEN1_ADC1_Pos			16
#define SYS_CLKEN1_ADC1_Msk			(0x01 << SYS_CLKEN1_ADC1_Pos)
#define SYS_CLKEN1_CAN1_Pos			17
#define SYS_CLKEN1_CAN1_Msk			(0x01 << SYS_CLKEN1_CAN1_Pos)
#define SYS_CLKEN1_RTC_Pos			19
#define SYS_CLKEN1_RTC_Msk			(0x01 << SYS_CLKEN1_RTC_Pos)
#define SYS_CLKEN1_IOFILT_Pos		20		//IO Filter
#define SYS_CLKEN1_IOFILT_Msk		(0x01 << SYS_CLKEN1_IOFILT_Pos)
#define SYS_CLKEN1_BTIMR_Pos		22
#define SYS_CLKEN1_BTIMR_Msk		(0x01 << SYS_CLKEN1_BTIMR_Pos)
#define SYS_CLKEN1_JPEG_Pos			25
#define SYS_CLKEN1_JPEG_Msk			(0x01 << SYS_CLKEN1_JPEG_Pos)
#define SYS_CLKEN1_DAC_Pos			26
#define SYS_CLKEN1_DAC_Msk			(0x01 << SYS_CLKEN1_DAC_Pos)
#define SYS_CLKEN1_QEI_Pos			27
#define SYS_CLKEN1_QEI_Msk			(0x01 << SYS_CLKEN1_QEI_Pos)

#define SYS_SLEEP_SLEEP_Pos			0		// write 1 to enter sleep mode
#define SYS_SLEEP_SLEEP_Msk			(0x01 << SYS_SLEEP_SLEEP_Pos)
#define SYS_SLEEP_STOP_Pos			1		// write 1 to enter stop mode
#define SYS_SLEEP_STOP_Msk			(0x01 << SYS_SLEEP_STOP_Pos)

#define SYS_RSTSR_POR_Pos			0		// POR reset happened, write 1 to clear
#define SYS_RSTSR_POR_Msk			(0x01 << SYS_RSTSR_POR_Pos)
#define SYS_RSTSR_WDT_Pos			1		// WDT reset happened, write 1 to clear
#define SYS_RSTSR_WDT_Msk			(0x01 << SYS_RSTSR_WDT_Pos)

#define SYS_RTCWKCR_EN_Pos			0		// RTC wakeup enable
#define SYS_RTCWKCR_EN_Msk			(0x01 << SYS_RTCWKCR_EN_Pos)

#define SYS_RTCWKSR_FLAG_Pos		0		// RTC wakeup flag, write 1 to clear
#define SYS_RTCWKSR_FLAG_Msk		(0x01 << SYS_RTCWKSR_FLAG_Pos)

#define SYS_USBPHYCR_IDEN_Pos		0		// ID PIN sample enalbe
#define SYS_USBPHYCR_IDEN_Msk		(0x01 << SYS_USBPHYCR_IDEN_Pos)
#define SYS_USBPHYCR_OPMODE_Pos		1		// 0 Normal Operation, 1 Non-Driving (Disable Pull-up register)
#define SYS_USBPHYCR_OPMODE_Msk		(0x03 << SYS_USBPHYCR_OPMODE_Pos)
#define SYS_USBPHYCR_XCVR_Pos		4		// Transceiver mode select, 1 FS, 2 LS
#define SYS_USBPHYCR_XCVR_Msk		(0x03 << SYS_USBPHYCR_XCVR_Pos)
#define SYS_USBPHYCR_PLLEN_Pos		7		// built-in PLL switch
#define SYS_USBPHYCR_PLLEN_Msk		(0x01 << SYS_USBPHYCR_PLLEN_Pos)

#define SYS_USBCR_RST48M_Pos		0		// reset for 48MHz clock domain
#define SYS_USBCR_RST48M_Msk		(0x01 << SYS_USBCR_RST48M_Pos)
#define SYS_USBCR_RST12M_Pos		1		// reset for 12MHz clock domain
#define SYS_USBCR_RST12M_Msk		(0x01 << SYS_USBCR_RST12M_Pos)
#define SYS_USBCR_RSTPLL_Pos		2		// reset for PLL clock domain
#define SYS_USBCR_RSTPLL_Msk		(0x01 << SYS_USBCR_RSTPLL_Pos)
#define SYS_USBCR_ROLE_Pos			3		// USB mode: 0 determined by the ID pin, 2 Host, 3 Device
#define SYS_USBCR_ROLE_Msk			(0x03 << SYS_USBCR_ROLE_Pos)
#define SYS_USBCR_VBUS_Pos			5		// 0 determined by the VBUS pin, 1 force high
#define SYS_USBCR_VBUS_Msk			(0x01 << SYS_USBCR_VBUS_Pos)

#define SYS_PRNGCR_CLR_Pos			0		// seed clear, keep at least one LRC clock cycle
#define SYS_PRNGCR_CLR_Msk			(0x01 << SYS_PRNGCR_CLR_Pos)
#define SYS_PRNGCR_MODE_Pos			1		// 0 close, 2 three-clock mode (RCHF, RCLF, XTAH)   3 two-clock mode (RCHF, RCLF)
#define SYS_PRNGCR_MODE_Msk			(0x03 << SYS_PRNGCR_MODE_Pos)
#define SYS_PRNGCR_RDY_Pos			8		// 1 ready, can read PRNGDL and PRNGDH
#define SYS_PRNGCR_RDY_Msk			(0x01 << SYS_PRNGCR_RDY_Pos)

#define SYS_IOFILT_TIM_Pos			0		// filter window time = Tfilter_clk * CLKDIV * 2^TIM
#define SYS_IOFILT_TIM_Msk			(0x0F << SYS_IOFILT_TIM_Pos)
#define SYS_IOFILT_CLKDIV_Pos		4		// 0 div1, 1 div32
#define SYS_IOFILT_CLKDIV_Msk		(0x01 << SYS_IOFILT_CLKDIV_Pos)
#define SYS_IOFILT_IOSEL_Pos		5		// filtered IO select, each IOFILT can filter one of the four IO
#define SYS_IOFILT_IOSEL_Msk		(0x03 << SYS_IOFILT_IOSEL_Pos)

#define SYS_PRSTR0_GPIOA_Pos		0
#define SYS_PRSTR0_GPIOA_Msk		(0x01 << SYS_PRSTR0_GPIOA_Pos)
#define SYS_PRSTR0_GPIOB_Pos		1
#define SYS_PRSTR0_GPIOB_Msk		(0x01 << SYS_PRSTR0_GPIOB_Pos)
#define SYS_PRSTR0_GPIOC_Pos		2
#define SYS_PRSTR0_GPIOC_Msk		(0x01 << SYS_PRSTR0_GPIOC_Pos)
#define SYS_PRSTR0_GPIOD_Pos		3
#define SYS_PRSTR0_GPIOD_Msk		(0x01 << SYS_PRSTR0_GPIOD_Pos)
#define SYS_PRSTR0_GPIOM_Pos		4
#define SYS_PRSTR0_GPIOM_Msk		(0x01 << SYS_PRSTR0_GPIOM_Pos)
#define SYS_PRSTR0_GPION_Pos		5
#define SYS_PRSTR0_GPION_Msk		(0x01 << SYS_PRSTR0_GPION_Pos)
#define SYS_PRSTR0_UART0_Pos		6
#define SYS_PRSTR0_UART0_Msk		(0x01 << SYS_PRSTR0_UART0_Pos)
#define SYS_PRSTR0_UART1_Pos		7
#define SYS_PRSTR0_UART1_Msk		(0x01 << SYS_PRSTR0_UART1_Pos)
#define SYS_PRSTR0_UART2_Pos		8
#define SYS_PRSTR0_UART2_Msk		(0x01 << SYS_PRSTR0_UART2_Pos)
#define SYS_PRSTR0_UART3_Pos		9
#define SYS_PRSTR0_UART3_Msk		(0x01 << SYS_PRSTR0_UART3_Pos)
#define SYS_PRSTR0_WDT_Pos			10
#define SYS_PRSTR0_WDT_Msk			(0x01 << SYS_PRSTR0_WDT_Pos)
#define SYS_PRSTR0_TIMR_Pos			11
#define SYS_PRSTR0_TIMR_Msk			(0x01 << SYS_PRSTR0_TIMR_Pos)
#define SYS_PRSTR0_PWM_Pos			12
#define SYS_PRSTR0_PWM_Msk			(0x01 << SYS_PRSTR0_PWM_Pos)
#define SYS_PRSTR0_SPI0_Pos			13
#define SYS_PRSTR0_SPI0_Msk			(0x01 << SYS_PRSTR0_SPI0_Pos)
#define SYS_PRSTR0_SPI1_Pos			14
#define SYS_PRSTR0_SPI1_Msk			(0x01 << SYS_PRSTR0_SPI1_Pos)
#define SYS_PRSTR0_I2C0_Pos			15
#define SYS_PRSTR0_I2C0_Msk			(0x01 << SYS_PRSTR0_I2C0_Pos)
#define SYS_PRSTR0_I2C1_Pos			16
#define SYS_PRSTR0_I2C1_Msk			(0x01 << SYS_PRSTR0_I2C1_Pos)
#define SYS_PRSTR0_CRC_Pos			19
#define SYS_PRSTR0_CRC_Msk			(0x01 << SYS_PRSTR0_CRC_Pos)
#define SYS_PRSTR0_CORDIC_Pos		20
#define SYS_PRSTR0_CORDIC_Msk		(0x01 << SYS_PRSTR0_CORDIC_Pos)
#define SYS_PRSTR0_DIV_Pos			21
#define SYS_PRSTR0_DIV_Msk			(0x01 << SYS_PRSTR0_DIV_Pos)
#define SYS_PRSTR0_SDIO_Pos			22
#define SYS_PRSTR0_SDIO_Msk			(0x01 << SYS_PRSTR0_SDIO_Pos)
#define SYS_PRSTR0_USB_Pos			24
#define SYS_PRSTR0_USB_Msk			(0x01 << SYS_PRSTR0_USB_Pos)
#define SYS_PRSTR0_ANAC_Pos			25
#define SYS_PRSTR0_ANAC_Msk			(0x01 << SYS_PRSTR0_ANAC_Pos)
#define SYS_PRSTR0_ADC0_Pos			26
#define SYS_PRSTR0_ADC0_Msk			(0x01 << SYS_PRSTR0_ADC0_Pos)
#define SYS_PRSTR0_CAN0_Pos			28
#define SYS_PRSTR0_CAN0_Msk			(0x01 << SYS_PRSTR0_CAN0_Pos)
#define SYS_PRSTR0_DMA2D_Pos		29
#define SYS_PRSTR0_DMA2D_Msk		(0x01 << SYS_PRSTR0_DMA2D_Pos)
#define SYS_PRSTR0_LCD_Pos			30
#define SYS_PRSTR0_LCD_Msk			(0x01 << SYS_PRSTR0_LCD_Pos)

#define SYS_PRSTR1_GPIOE_Pos		0
#define SYS_PRSTR1_GPIOE_Msk		(0x01 << SYS_PRSTR1_GPIOE_Pos)
#define SYS_PRSTR1_SDRAM_Pos		12
#define SYS_PRSTR1_SDRAM_Msk		(0x01 << SYS_PRSTR1_SDRAM_Pos)
#define SYS_PRSTR1_SFC_Pos			13
#define SYS_PRSTR1_SFC_Msk			(0x01 << SYS_PRSTR1_SFC_Pos)
#define SYS_PRSTR1_ADC1_Pos			16
#define SYS_PRSTR1_ADC1_Msk			(0x01 << SYS_PRSTR1_ADC1_Pos)
#define SYS_PRSTR1_CAN1_Pos			17
#define SYS_PRSTR1_CAN1_Msk			(0x01 << SYS_PRSTR1_CAN1_Pos)
#define SYS_PRSTR1_RTC_Pos			19
#define SYS_PRSTR1_RTC_Msk			(0x01 << SYS_PRSTR1_RTC_Pos)
#define SYS_PRSTR1_IOFILT_Pos		20
#define SYS_PRSTR1_IOFILT_Msk		(0x01 << SYS_PRSTR1_IOFILT_Pos)
#define SYS_PRSTR1_BTIMR_Pos		22
#define SYS_PRSTR1_BTIMR_Msk		(0x01 << SYS_PRSTR1_BTIMR_Pos)
#define SYS_PRSTR1_JPEG_Pos			25
#define SYS_PRSTR1_JPEG_Msk			(0x01 << SYS_PRSTR1_JPEG_Pos)
#define SYS_PRSTR1_DAC_Pos			26
#define SYS_PRSTR1_DAC_Msk			(0x01 << SYS_PRSTR1_DAC_Pos)
#define SYS_PRSTR1_QEI_Pos			27
#define SYS_PRSTR1_QEI_Msk			(0x01 << SYS_PRSTR1_QEI_Pos)

#define SYS_HRCCR_ON_Pos			0		// High speed RC ON
#define SYS_HRCCR_ON_Msk			(0x01 << SYS_HRCCR_ON_Pos)
#define SYS_HRCCR_DBL_Pos		    1		// Double Frequency, 0 20MHz, 1 40MHz
#define SYS_HRCCR_DBL_Msk		    (0x01 << SYS_HRCCR_DBL_Pos)

#define SYS_BODCR_IE_Pos		    1		// Interrupt Enable
#define SYS_BODCR_IE_Msk		    (0x01 << SYS_BODCR_IE_Pos)
#define SYS_BODCR_INTLVL_Pos		4		// BOD interrupt trigger level
#define SYS_BODCR_INTLVL_Msk		(0x07 << SYS_BODCR_INTLVL_Pos)
#define SYS_BODCR_RSTLVL_Pos		7		// BOD reset level
#define SYS_BODCR_RSTLVL_Msk		(0x07 << SYS_BODCR_RSTLVL_Pos)

#define SYS_BODSR_IF_Pos			0		// interrupt flag, write 1 to clear
#define SYS_BODSR_IF_Msk			(0x01 << SYS_BODSR_IF_Pos)
#define SYS_BODSR_ST_Pos			1		// BOD Status
#define SYS_BODSR_ST_Msk			(0x01 << SYS_BODSR_ST_Pos)

#define SYS_ADCCR_0IVREN_Pos		0		// ADC0 Internal Vref Enable
#define SYS_ADCCR_0IVREN_Msk		(0x01 << SYS_ADCCR_0IVREN_Pos)
#define SYS_ADCCR_1IVREN_Pos		1		// ADC1 Internal Vref Enable
#define SYS_ADCCR_1IVREN_Msk		(0x01 << SYS_ADCCR_1IVREN_Pos)
#define SYS_ADCCR_0IVRSEL_Pos		2		// ADC0 Internal Vref Select, 0 reference current multiplied by resistance, 1 VDD33 resistive subdivision
#define SYS_ADCCR_0IVRSEL_Msk		(0x01 << SYS_ADCCR_0IVRSEL_Pos)
#define SYS_ADCCR_1IVRSEL_Pos		3		// ADC1 Internal Vref Select, 0 reference current multiplied by resistance, 1 VDD33 resistive subdivision
#define SYS_ADCCR_1IVRSEL_Msk		(0x01 << SYS_ADCCR_1IVRSEL_Pos)
#define SYS_ADCCR_IVRFLT_Pos		4		// Internal Vref Filter, 0 high-pass filter, 1 low-pass filter
#define SYS_ADCCR_IVRFLT_Msk		(0x01 << SYS_ADCCR_IVRFLT_Pos)
#define SYS_ADCCR_IVROuA_Pos		5		// Internal Vref Output uA, 0 50uA, 1 150uA
#define SYS_ADCCR_IVROuA_Msk		(0x01 << SYS_ADCCR_IVROuA_Pos)
#define SYS_ADCCR_IVRLVL_Pos		6		// Internal Vref Output Level, X00b 1.2V, X01 1.6V, X10 2.0V, X11 2.5V
#define SYS_ADCCR_IVRLVL_Msk		(0x07 << SYS_ADCCR_IVRLVL_Pos)
#define SYS_ADCCR_IVRTRIM_Pos		9		// Internal Vref Output Level Trimming, 000 +0V, 001 +0.04V, 010 +0.8V, 011 +0.12V, 100 -0.04V, 101 -0.08V, 110 -0.12V, 111 -0.16V
#define SYS_ADCCR_IVRTRIM_Msk		(0x07 << SYS_ADCCR_IVRTRIM_Pos)

#define SYS_XTALCR_32KON_Pos		0		// XTAL_32K On
#define SYS_XTALCR_32KON_Msk		(0x01 << SYS_XTALCR_32KON_Pos)
#define SYS_XTALCR_ON_Pos			1		// XTAL On
#define SYS_XTALCR_ON_Msk			(0x01 << SYS_XTALCR_ON_Pos)
#define SYS_XTALCR_32KDET_Pos		4		// XTAL_32K Stop Detect
#define SYS_XTALCR_32KDET_Msk		(0x01 << SYS_XTALCR_32KDET_Pos)
#define SYS_XTALCR_DET_Pos			5		// XTAL Stop Detect
#define SYS_XTALCR_DET_Msk			(0x01 << SYS_XTALCR_DET_Pos)
#define SYS_XTALCR_32KDRV_Pos		8		// XTAL_32K drive capability
#define SYS_XTALCR_32KDRV_Msk		(0x0F << SYS_XTALCR_32KDRV_Pos)
#define SYS_XTALCR_DRV_Pos			16		// XTAL drive capability
#define SYS_XTALCR_DRV_Msk			(0x1F << SYS_XTALCR_DRV_Pos)

#define SYS_XTALSR_32KSTOP_Pos		0		// XTAL_32K Stop, write 1 to clear
#define SYS_XTALSR_32KSTOP_Msk		(0x01 << SYS_XTALSR_32KSTOP_Pos)
#define SYS_XTALSR_STOP_Pos			1		// XTAL Stop, write 1 to clear
#define SYS_XTALSR_STOP_Msk			(0x01 << SYS_XTALSR_STOP_Pos)

#define SYS_PLLCR_OUTEN_Pos		    0       // can only be set after LOCK
#define SYS_PLLCR_OUTEN_Msk		    (0x01 << SYS_PLLCR_OUTEN_Pos)
#define SYS_PLLCR_INSEL_Pos		    1       // 0 XTAL, 1 HRC
#define SYS_PLLCR_INSEL_Msk		    (0x01 << SYS_PLLCR_INSEL_Pos)
#define SYS_PLLCR_OFF_Pos		    2
#define SYS_PLLCR_OFF_Msk		    (0x01 << SYS_PLLCR_OFF_Pos)
#define SYS_PLLCR_RST_Pos			3
#define SYS_PLLCR_RST_Msk			(0x01 << SYS_PLLCR_RST_Pos)

#define SYS_PLLDIV_FBDIV_Pos		0       // PLL FeedBack divider
#define SYS_PLLDIV_FBDIV_Msk		(0x1FF << SYS_PLLDIV_FBDIV_Pos)
#define SYS_PLLDIV_INDIV_Pos		16      // PLL input clock divider
#define SYS_PLLDIV_INDIV_Msk		(0x1F << SYS_PLLDIV_INDIV_Pos)
#define SYS_PLLDIV_OUTDIV_Pos		24      // PLL output clock divider, 0 div8, 1 div4, 0 div2
#define SYS_PLLDIV_OUTDIV_Msk		(0x03 << SYS_PLLDIV_OUTDIV_Pos)

#define SYS_LRCCR_ON_Pos			0		// Low Speed RC On
#define SYS_LRCCR_ON_Msk			(0x01 << SYS_LRCCR_ON_Pos)

#define SYS_OPACR_OPA0ON_Pos		0		// OPA0 on
#define SYS_OPACR_OPA0ON_Msk		(0x01 << SYS_OPACR_OPA0ON_Pos)
#define SYS_OPACR_OPA1ON_Pos		1
#define SYS_OPACR_OPA1ON_Msk		(0x01 << SYS_OPACR_OPA1ON_Pos)
#define SYS_OPACR_OPA2ON_Pos		2
#define SYS_OPACR_OPA2ON_Msk		(0x01 << SYS_OPACR_OPA2ON_Pos)
#define SYS_OPACR_OPA3ON_Pos		3
#define SYS_OPACR_OPA3ON_Msk		(0x01 << SYS_OPACR_OPA3ON_Pos)

#define SYS_ACMPCR_CMP0ON_Pos		0		// CMP0 on
#define SYS_ACMPCR_CMP0ON_Msk		(0x01 << SYS_ACMPCR_CMP0ON_Pos)
#define SYS_ACMPCR_CMP1ON_Pos		1
#define SYS_ACMPCR_CMP1ON_Msk		(0x01 << SYS_ACMPCR_CMP1ON_Pos)
#define SYS_ACMPCR_CMP2ON_Pos		2
#define SYS_ACMPCR_CMP2ON_Msk		(0x01 << SYS_ACMPCR_CMP2ON_Pos)
#define SYS_ACMPCR_CMP0HYS_Pos		8		// CMP0 hysteretic on
#define SYS_ACMPCR_CMP0HYS_Msk		(0x01 << SYS_ACMPCR_CMP0HYS_Pos)
#define SYS_ACMPCR_CMP1HYS_Pos		9
#define SYS_ACMPCR_CMP1HYS_Msk		(0x01 << SYS_ACMPCR_CMP1HYS_Pos)
#define SYS_ACMPCR_CMP2HYS_Pos		10
#define SYS_ACMPCR_CMP2HYS_Msk		(0x01 << SYS_ACMPCR_CMP2HYS_Pos)
#define SYS_ACMPCR_0NVREF_Pos		16		// 1 ACMP0 N input connect to VREF
#define SYS_ACMPCR_0NVREF_Msk		(0x01 << SYS_ACMPCR_0NVREF_Pos)
#define SYS_ACMPCR_1NVREF_Pos		17
#define SYS_ACMPCR_1NVREF_Msk		(0x01 << SYS_ACMPCR_1NVREF_Pos)
#define SYS_ACMPCR_2NVREF_Pos		18
#define SYS_ACMPCR_2NVREF_Msk		(0x01 << SYS_ACMPCR_2NVREF_Pos)
#define SYS_ACMPCR_CMP0IE_Pos		24		// CMP0 interrupt enable
#define SYS_ACMPCR_CMP0IE_Msk		(0x01 << SYS_ACMPCR_CMP0IE_Pos)
#define SYS_ACMPCR_CMP1IE_Pos		25
#define SYS_ACMPCR_CMP1IE_Msk		(0x01 << SYS_ACMPCR_CMP1IE_Pos)
#define SYS_ACMPCR_CMP2IE_Pos		26
#define SYS_ACMPCR_CMP2IE_Msk		(0x01 << SYS_ACMPCR_CMP2IE_Pos)

#define SYS_ACMPSR_CMP0OUT_Pos		0		// 0 N > P, 1 P > N
#define SYS_ACMPSR_CMP0OUT_Msk		(0x01 << SYS_ACMPSR_CMP0OUT_Pos)
#define SYS_ACMPSR_CMP1OUT_Pos		1
#define SYS_ACMPSR_CMP1OUT_Msk		(0x01 << SYS_ACMPSR_CMP1OUT_Pos)
#define SYS_ACMPSR_CMP2OUT_Pos		2
#define SYS_ACMPSR_CMP2OUT_Msk		(0x01 << SYS_ACMPSR_CMP2OUT_Pos)
#define SYS_ACMPSR_CMP0IF_Pos		8		// interrupt flag, write 1 to clear
#define SYS_ACMPSR_CMP0IF_Msk		(0x01 << SYS_ACMPSR_CMP0IF_Pos)
#define SYS_ACMPSR_CMP1IF_Pos		9
#define SYS_ACMPSR_CMP1IF_Msk		(0x01 << SYS_ACMPSR_CMP1IF_Pos)
#define SYS_ACMPSR_CMP2IF_Pos		10
#define SYS_ACMPSR_CMP2IF_Msk		(0x01 << SYS_ACMPSR_CMP2IF_Pos)

#define SYS_ACMPCR2_BRK0_Pos		0		// 1 ACMP0 output used as PWM_BRK0
#define SYS_ACMPCR2_BRK0_Msk		(0x01 << SYS_ACMPCR2_BRK0_Pos)
#define SYS_ACMPCR2_BRK1_Pos		1
#define SYS_ACMPCR2_BRK1_Msk		(0x01 << SYS_ACMPCR2_BRK1_Pos)
#define SYS_ACMPCR2_BRK2_Pos		2
#define SYS_ACMPCR2_BRK2_Msk		(0x01 << SYS_ACMPCR2_BRK2_Pos)
#define SYS_ACMPCR2_VREF_Pos		3		// ACMP internal refrence voltage = 0.3 + 0.02*VREF, VREF range from 0 to 47
#define SYS_ACMPCR2_VREF_Msk		(0x3F << SYS_ACMPCR2_VREF_Pos)

#define SYS_DACCR_VRADJ_Pos			0		// Vref Adjust
#define SYS_DACCR_VRADJ_Msk			(0x1F << SYS_DACCR_VRADJ_Pos)

#define SYS_TEMPCR_EN_Pos			0
#define SYS_TEMPCR_EN_Msk			(0x01 << SYS_TEMPCR_EN_Pos)




typedef struct {
	__IO uint32_t FUNC0;					// pin function select
	
	__IO uint32_t FUNC1;
	
		 uint32_t RESERVED[62];
	
    __IO uint32_t PULLU;              		// pull-up enable
    
         uint32_t RESERVED2[63];
    
    __IO uint32_t PULLD;	              	// pull-down enable
    
         uint32_t RESERVED3[63];
    
    __IO uint32_t INEN;               		// input enable
    
         uint32_t RESERVED4[63];

	__IO uint32_t OPEND;              		// open-drain enable
} PORT_TypeDef;




typedef struct {
	__IO uint32_t ODR;
#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6
#define PIN7    7
#define PIN8    8
#define PIN9    9
#define PIN10   10
#define PIN11   11
#define PIN12   12
#define PIN13   13
#define PIN14   14
#define PIN15   15

	__IO uint32_t DIR;					    // 0 input, 1 output

	__IO uint32_t INTLVLTRG;				// Interrupt Level Trigger, 1 level trigger, 0 edge trigger

	__IO uint32_t INTBE;					// Both Edge

	__IO uint32_t INTRISEEN;				// Interrupt Rise Edge Enable, 1 rise-edge or high-level trigger, 0 fall-edge or low-level trigger

	__IO uint32_t INTEN;					// interrupt enable

	__I  uint32_t INTRAWSTAT;			    // interrupt raw state

	__I  uint32_t INTSTAT;				    // INTSTAT.PIN0 = INTRAWSTAT.PIN0 & INTEN.PIN0

	__O  uint32_t INTCLR;				    // interrupt flag clear
		
		 uint32_t RESERVED[3];
	
	__I  uint32_t IDR;
	
		 uint32_t RESERVED2[3];
	
	__IO uint32_t DATAPIN0;
	__IO uint32_t DATAPIN1;
	__IO uint32_t DATAPIN2;
	__IO uint32_t DATAPIN3;
	__IO uint32_t DATAPIN4;
	__IO uint32_t DATAPIN5;
	__IO uint32_t DATAPIN6;
	__IO uint32_t DATAPIN7;
	__IO uint32_t DATAPIN8;
	__IO uint32_t DATAPIN9;
	__IO uint32_t DATAPIN10;
	__IO uint32_t DATAPIN11;
	__IO uint32_t DATAPIN12;
	__IO uint32_t DATAPIN13;
	__IO uint32_t DATAPIN14;
	__IO uint32_t DATAPIN15;
} GPIO_TypeDef;




typedef struct {
	__IO uint32_t LOAD;						// reload value

	__I  uint32_t VALUE;					// current value, counting down from LOAD

	__IO uint32_t CR;
	
		 uint32_t RESERVED;
	
	__IO uint32_t IE;
		
	__IO uint32_t IF;
	
	__IO uint32_t HALT;						// 1 halt counting
	
	__IO uint32_t OCCR;
	__IO uint32_t OCMAT;
	
	__IO uint32_t RESERVED2;
	
	__IO uint32_t ICLOW;
	__IO uint32_t ICHIGH;
	
	__IO uint32_t PREDIV;					// prescale, 8-bit width
} TIMR_TypeDef;


#define TIMR_LOAD_VALUE_Pos			0
#define TIMR_LOAD_VALUE_Msk			(0xFFFFFF << TIMR_LOAD_VALUE_Pos)
#define TIMR_LOAD_RELOAD_Pos		24		// reload VALUE to TIMR's internal Counter immediately. only for BTIMRx, not for TIMRx.
#define TIMR_LOAD_RELOAD_Msk		(0x01 << TIMR_LOAD_RELOAD_Pos)

#define TIMR_CR_CLKSRC_Pos			0		// clock source, 0 system clock, 2 external pin pulse
#define TIMR_CR_CLKSRC_Msk			(0x03 << TIMR_CR_CLKSRC_Pos)
#define TIMR_CR_MODE_Pos			2		// 0 timer, 1 inpurt capture, 2 output compare
#define TIMR_CR_MODE_Msk			(0x03 << TIMR_CR_MODE_Pos)

#define TIMR_IE_TO_Pos				0		// Time out
#define TIMR_IE_TO_Msk				(0x01 << TIMR_IE_TO_Pos)
#define TIMR_IE_OC0_Pos				1		// Output compare inversion point zero
#define TIMR_IE_OC0_Msk				(0x01 << TIMR_IE_OC0_Pos)
#define TIMR_IE_OC1_Pos				2		// Output compare inversion point one
#define TIMR_IE_OC1_Msk				(0x01 << TIMR_IE_OC1_Pos)
#define TIMR_IE_ICR_Pos				3		// Input capture, rising edge interrupt
#define TIMR_IE_ICR_Msk				(0x01 << TIMR_IE_ICR_Pos)
#define TIMR_IE_ICF_Pos				4		// Input capture, falling edge interrupt
#define TIMR_IE_ICF_Msk				(0x01 << TIMR_IE_ICF_Pos)

#define TIMR_IF_TO_Pos				0		// write 1 to clear
#define TIMR_IF_TO_Msk				(0x01 << TIMR_IF_TO_Pos)
#define TIMR_IF_OC0_Pos				1
#define TIMR_IF_OC0_Msk				(0x01 << TIMR_IF_OC0_Pos)
#define TIMR_IF_OC1_Pos				2
#define TIMR_IF_OC1_Msk				(0x01 << TIMR_IF_OC1_Pos)
#define TIMR_IF_ICR_Pos				3
#define TIMR_IF_ICR_Msk				(0x01 << TIMR_IF_ICR_Pos)
#define TIMR_IF_ICF_Pos				4
#define TIMR_IF_ICF_Msk				(0x01 << TIMR_IF_ICF_Pos)

#define TIMR_OCCR_FORCELVL_Pos		0		// Force Levle
#define TIMR_OCCR_FORCELVL_Msk		(0x01 << TIMR_OCCR_FORCELVL_Pos)
#define TIMR_OCCR_INITLVL_Pos		1		// Initial Level
#define TIMR_OCCR_INITLVL_Msk		(0x01 << TIMR_OCCR_INITLVL_Pos)
#define TIMR_OCCR_FORCEEN_Pos		2		// Force Level Enable
#define TIMR_OCCR_FORCEEN_Msk		(0x01 << TIMR_OCCR_FORCEEN_Pos)


typedef struct {
	__IO uint32_t HALLIE;					// HALL interrupt enable
	
		 uint32_t RESERVED;
	
	__IO uint32_t HALLIF;
	
	__IO uint32_t HALLEN;					// HALL enable
	
	__IO uint32_t HALL0V;					// HALL for TIMR0 capture value (LOAD - VALUE)
	
	__IO uint32_t HALL3V;
	
		 uint32_t RESERVED2;
	
	__IO uint32_t HALLSR;					// HALL input status
	
		 uint32_t RESERVED3[8];
	
	__IO uint32_t EN;
} TIMRG_TypeDef;


#define TIMRG_HALLIE_HALL0_Pos		0		// HALL for TIMR0
#define TIMRG_HALLIE_HALL0_Msk		(0x01 << TIMRG_HALLIE_HALL0_Pos)
#define TIMRG_HALLIE_HALL3_Pos		1		// HALL for TIMR3
#define TIMRG_HALLIE_HALL3_Msk		(0x01 << TIMRG_HALLIE_HALL3_Pos)

#define TIMRG_HALLIF_H0IN0_Pos		0		// HALL0's input0 interrupt flag
#define TIMRG_HALLIF_H0IN0_Msk		(0x01 << TIMRG_HALLIF_H0IN0_Pos)
#define TIMRG_HALLIF_H0IN1_Pos		1
#define TIMRG_HALLIF_H0IN1_Msk		(0x01 << TIMRG_HALLIF_H0IN1_Pos)
#define TIMRG_HALLIF_H0IN2_Pos		2
#define TIMRG_HALLIF_H0IN2_Msk		(0x01 << TIMRG_HALLIF_H0IN2_Pos)
#define TIMRG_HALLIF_H3IN0_Pos		3		// HALL3's input0 interrupt flag
#define TIMRG_HALLIF_H3IN0_Msk		(0x01 << TIMRG_HALLIF_H3IN0_Pos)
#define TIMRG_HALLIF_H3IN1_Pos		4
#define TIMRG_HALLIF_H3IN1_Msk		(0x01 << TIMRG_HALLIF_H3IN1_Pos)
#define TIMRG_HALLIF_H3IN2_Pos		5
#define TIMRG_HALLIF_H3IN2_Msk		(0x01 << TIMRG_HALLIF_H3IN2_Pos)

#define TIMRG_HALLEN_HALL0_Pos		0		// HALL0 enable
#define TIMRG_HALLEN_HALL0_Msk		(0x01 << TIMRG_HALLEN_HALL0_Pos)
#define TIMRG_HALLEN_HALL3_Pos		1
#define TIMRG_HALLEN_HALL3_Msk		(0x01 << TIMRG_HALLEN_HALL3_Pos)

#define TIMRG_HALLSR_H0IN0_Pos		0		// current status of HALL0's input0
#define TIMRG_HALLSR_H0IN0_Msk		(0x01 << TIMRG_HALLSR_H0IN0_Pos)
#define TIMRG_HALLSR_H0IN1_Pos		1
#define TIMRG_HALLSR_H0IN1_Msk		(0x01 << TIMRG_HALLSR_H0IN1_Pos)
#define TIMRG_HALLSR_H0IN2_Pos		2
#define TIMRG_HALLSR_H0IN2_Msk		(0x01 << TIMRG_HALLSR_H0IN2_Pos)
#define TIMRG_HALLSR_H3IN0_Pos		3
#define TIMRG_HALLSR_H3IN0_Msk		(0x01 << TIMRG_HALLSR_H3IN0_Pos)
#define TIMRG_HALLSR_H3IN1_Pos		4
#define TIMRG_HALLSR_H3IN1_Msk		(0x01 << TIMRG_HALLSR_H3IN1_Pos)
#define TIMRG_HALLSR_H3IN2_Pos		5
#define TIMRG_HALLSR_H3IN2_Msk		(0x01 << TIMRG_HALLSR_H3IN2_Pos)

#define TIMRG_EN_TIMR0_Pos			0
#define TIMRG_EN_TIMR0_Msk			(0x01 << TIMRG_EN_TIMR0_Pos)
#define TIMRG_EN_TIMR1_Pos			1
#define TIMRG_EN_TIMR1_Msk			(0x01 << TIMRG_EN_TIMR1_Pos)
#define TIMRG_EN_TIMR2_Pos			2
#define TIMRG_EN_TIMR2_Msk			(0x01 << TIMRG_EN_TIMR2_Pos)
#define TIMRG_EN_TIMR3_Pos			3
#define TIMRG_EN_TIMR3_Msk			(0x01 << TIMRG_EN_TIMR3_Pos)
#define TIMRG_EN_TIMR4_Pos			4
#define TIMRG_EN_TIMR4_Msk			(0x01 << TIMRG_EN_TIMR4_Pos)
#define TIMRG_EN_TIMR5_Pos			5
#define TIMRG_EN_TIMR5_Msk			(0x01 << TIMRG_EN_TIMR5_Pos)
#define TIMRG_EN_TIMR6_Pos			6
#define TIMRG_EN_TIMR6_Msk			(0x01 << TIMRG_EN_TIMR6_Pos)
#define TIMRG_EN_TIMR7_Pos			7
#define TIMRG_EN_TIMR7_Msk			(0x01 << TIMRG_EN_TIMR7_Pos)
#define TIMRG_EN_TIMR8_Pos			8
#define TIMRG_EN_TIMR8_Msk			(0x01 << TIMRG_EN_TIMR8_Pos)
#define TIMRG_EN_TIMR9_Pos			9
#define TIMRG_EN_TIMR9_Msk			(0x01 << TIMRG_EN_TIMR9_Pos)
#define TIMRG_EN_TIMR10_Pos			10
#define TIMRG_EN_TIMR10_Msk			(0x01 << TIMRG_EN_TIMR10_Pos)
#define TIMRG_EN_TIMR11_Pos			11
#define TIMRG_EN_TIMR11_Msk			(0x01 << TIMRG_EN_TIMR11_Pos)




typedef struct {
	__IO uint32_t DATA;
	
	__IO uint32_t CTRL;
	
	__IO uint32_t BAUD;
	
	__IO uint32_t FIFO;
	
	__IO uint32_t LINCR;
	
	union {
		__IO uint32_t CTSCR;
		
		__IO uint32_t RTSCR;
	};
	
	__IO uint32_t CFG;
	
	__IO uint32_t TOCR;						// Timeout Control Register
} UART_TypeDef;


#define UART_DATA_DATA_Pos			0
#define UART_DATA_DATA_Msk			(0x1FF << UART_DATA_DATA_Pos)
#define UART_DATA_VALID_Pos			9		// data valid, clear by read
#define UART_DATA_VALID_Msk			(0x01 << UART_DATA_VALID_Pos)
#define UART_DATA_PAERR_Pos			10		// Parity Error
#define UART_DATA_PAERR_Msk			(0x01 << UART_DATA_PAERR_Pos)

#define UART_CTRL_TXIDLE_Pos		0		// TX IDLE
#define UART_CTRL_TXIDLE_Msk		(0x01 << UART_CTRL_TXIDLE_Pos)
#define UART_CTRL_TXFF_Pos		    1		// TX FIFO Full
#define UART_CTRL_TXFF_Msk		    (0x01 << UART_CTRL_TXFF_Pos)
#define UART_CTRL_TXIE_Pos			2		// TX interrupt enable, trigger when the number of data in TX FIFO less than specified value
#define UART_CTRL_TXIE_Msk			(0x01 << UART_CTRL_TXIE_Pos)
#define UART_CTRL_RXNE_Pos			3		// RX FIFO Not Empty
#define UART_CTRL_RXNE_Msk			(0x01 << UART_CTRL_RXNE_Pos)
#define UART_CTRL_RXIE_Pos			4		// RX interrupt enable, trigger when the number of data in RX FIFO more than specified value
#define UART_CTRL_RXIE_Msk			(0x01 << UART_CTRL_RXIE_Pos)
#define UART_CTRL_RXOV_Pos			5		// RX FIFO Overflow, write 1 to clar
#define UART_CTRL_RXOV_Msk			(0x01 << UART_CTRL_RXOV_Pos)
#define UART_CTRL_TXDOIE_Pos		6		// TX Done interrupt enable, trigger when TX FIFO and TX shift register both empty
#define UART_CTRL_TXDOIE_Msk		(0x01 << UART_CTRL_TXDOIE_Pos)
#define UART_CTRL_EN_Pos			9
#define UART_CTRL_EN_Msk			(0x01 << UART_CTRL_EN_Pos)
#define UART_CTRL_LOOP_Pos			10
#define UART_CTRL_LOOP_Msk			(0x01 << UART_CTRL_LOOP_Pos)
#define UART_CTRL_TOIE_Pos			14		// TimeOut interrupt enable, trigger when no new data was received within TOTIME/BAUDRAUD second
#define UART_CTRL_TOIE_Msk			(0x01 << UART_CTRL_TOIE_Pos)
#define UART_CTRL_DATA9b_Pos		18		// 1 9-bit data, 0 8-bit data
#define UART_CTRL_DATA9b_Msk		(0x01 << UART_CTRL_DATA9b_Pos)
#define UART_CTRL_PARITY_Pos		19		// 0 no parity, 1 odd parity, 3 even parity, 5 one parity, 7 zero parity
#define UART_CTRL_PARITY_Msk		(0x07 << UART_CTRL_PARITY_Pos)
#define UART_CTRL_STOP2b_Pos		22		// 1 2-bit stop, 0 1-bit stop
#define UART_CTRL_STOP2b_Msk		(0x03 << UART_CTRL_STOP2b_Pos)

#define UART_BAUD_BAUD_Pos			0		// baudrate = SYS_Freq/16/BAUD - 1
#define UART_BAUD_BAUD_Msk			(0x3FFF << UART_BAUD_BAUD_Pos)
#define UART_BAUD_TXD_Pos			14		// TXD pin status
#define UART_BAUD_TXD_Msk			(0x01 << UART_BAUD_TXD_Pos)
#define UART_BAUD_RXD_Pos			15		// RXD pin status
#define UART_BAUD_RXD_Msk			(0x01 << UART_BAUD_RXD_Pos)
#define UART_BAUD_RXTOIF_Pos		16		// = RXIF | TOIF
#define UART_BAUD_RXTOIF_Msk		(0x01 << UART_BAUD_RXTOIF_Pos)
#define UART_BAUD_TXIF_Pos			17		// = TXTHRF & TXIE
#define UART_BAUD_TXIF_Msk			(0x01 << UART_BAUD_TXIF_Pos)
#define UART_BAUD_RXTHRF_Pos		19		// RX FIFO Threshold Flag, set when the number of data in RX FIFO > RXTHR
#define UART_BAUD_RXTHRF_Msk		(0x01 << UART_BAUD_RXTHRF_Pos)
#define UART_BAUD_TXTHRF_Pos		20		// TX FIFO Threshold Flag, set when the number of data in TX FIFO <= TXTHR
#define UART_BAUD_TXTHRF_Msk		(0x01 << UART_BAUD_TXTHRF_Pos)
#define UART_BAUD_TOIF_Pos			21		// TimeOut interrupt flag
#define UART_BAUD_TOIF_Msk			(0x01 << UART_BAUD_TOIF_Pos)
#define UART_BAUD_RXIF_Pos			22		// = RXTHRF & RXIE
#define UART_BAUD_RXIF_Msk			(0x01 << UART_BAUD_RXIF_Pos)
#define UART_BAUD_ABREN_Pos			23		// Auto Baudrate Enable, write 1 starts automatic baudrate calibration and automatically clears after completion
#define UART_BAUD_ABREN_Msk			(0x01 << UART_BAUD_ABREN_Pos)
#define UART_BAUD_ABRBIT_Pos		24
#define UART_BAUD_ABRBIT_Msk		(0x03 << UART_BAUD_ABRBIT_Pos)
#define UART_BAUD_ABRERR_Pos		26		// Auto Baudrate Error
#define UART_BAUD_ABRERR_Msk		(0x01 << UART_BAUD_ABRERR_Pos)
#define UART_BAUD_TXDOIF_Pos		27		// TX Done interrupt flag
#define UART_BAUD_TXDOIF_Msk		(0x01 << UART_BAUD_TXDOIF_Pos)
#define UART_BAUD_FRAC_Pos			28		// Baudrate fractional value
#define UART_BAUD_FRAC_Msk			(0x0Fu << UART_BAUD_FRAC_Pos)

#define UART_FIFO_RXLVL_Pos			0		// RX FIFO Level, the number of data in RX FIFO
#define UART_FIFO_RXLVL_Msk			(0xFF << UART_FIFO_RXLVL_Pos)
#define UART_FIFO_TXLVL_Pos			8		// TX FIFO Level, the number of data in TX FIFO
#define UART_FIFO_TXLVL_Msk			(0xFF << UART_FIFO_TXLVL_Pos)
#define UART_FIFO_RXTHR_Pos			16		// RX FIFO Threshold
#define UART_FIFO_RXTHR_Msk			(0xFF << UART_FIFO_RXTHR_Pos)
#define UART_FIFO_TXTHR_Pos			24		// TX FIFO Threshold
#define UART_FIFO_TXTHR_Msk			(0xFFu<< UART_FIFO_TXTHR_Pos)

#define UART_LINCR_BRKDETIE_Pos		0		// LIN Break detected interrupt enable
#define UART_LINCR_BRKDETIE_Msk		(0x01 << UART_LINCR_BRKDETIE_Pos)
#define UART_LINCR_BRKDETIF_Pos		1		// LIN Break detected interrupt flag
#define UART_LINCR_BRKDETIF_Msk		(0x01 << UART_LINCR_BRKDETIF_Pos)
#define UART_LINCR_GENBRKIE_Pos		2		// LIN Break sent done interrupt enable
#define UART_LINCR_GENBRKIE_Msk		(0x01 << UART_LINCR_GENBRKIE_Pos)
#define UART_LINCR_GENBRKIF_Pos		3		// LIN Break sent done interrupt flag
#define UART_LINCR_GENBRKIF_Msk		(0x01 << UART_LINCR_GENBRKIF_Pos)
#define UART_LINCR_GENBRK_Pos		4		// write 1 to generate LIN Break, automatic zero clearing after sending
#define UART_LINCR_GENBRK_Msk		(0x01 << UART_LINCR_GENBRK_Pos)

#define UART_CTSCR_EN_Pos			0		// CTS flow control enable
#define UART_CTSCR_EN_Msk			(0x01 << UART_CTSCR_EN_Pos)
#define UART_CTSCR_POL_Pos			2		// CTS signal polarity, 0 low valid, when the CTS input is low, data can be sent
#define UART_CTSCR_POL_Msk			(0x01 << UART_CTSCR_POL_Pos)
#define UART_CTSCR_STAT_Pos			7		// CTS signal current level
#define UART_CTSCR_STAT_Msk			(0x01 << UART_CTSCR_STAT_Pos)

#define UART_RTSCR_EN_Pos			1		// RTS flow control enable
#define UART_RTSCR_EN_Msk			(0x01 << UART_RTSCR_EN_Pos)
#define UART_RTSCR_POL_Pos			3		// RTS signal polarity, 0 low valid, when UART can receive data, output low level on the RTS line
#define UART_RTSCR_POL_Msk			(0x01 << UART_RTSCR_POL_Pos)
#define UART_RTSCR_THR_Pos			4		// RTS flow control threshold, 0 1-byte, 1 2-byte, 2 4-byte, 3 6-byte
#define UART_RTSCR_THR_Msk			(0x07 << UART_RTSCR_THR_Pos)
#define UART_RTSCR_STAT_Pos			8		// RTS signal current level
#define UART_RTSCR_STAT_Msk			(0x01 << UART_RTSCR_STAT_Pos)

#define UART_CFG_RXEN_Pos			0		// RX Enable
#define UART_CFG_RXEN_Msk			(0x01 << UART_CFG_RXEN_Pos)
#define UART_CFG_MSBF_Pos			1		// MSB First
#define UART_CFG_MSBF_Msk			(0x01 << UART_CFG_MSBF_Pos)
#define UART_CFG_BRKTXLEN_Pos		2		// TX break length
#define UART_CFG_BRKTXLEN_Msk		(0x0F << UART_CFG_BRKTXLEN_Pos)
#define UART_CFG_BRKRXLEN_Pos		6		// RX break length
#define UART_CFG_BRKRXLEN_Msk		(0x0F << UART_CFG_BRKRXLEN_Pos)
#define UART_CFG_RXINV_Pos			10		// RX level inverse
#define UART_CFG_RXINV_Msk			(0x01 << UART_CFG_RXINV_Pos)
#define UART_CFG_TXINV_Pos			11		// TX level inverse
#define UART_CFG_TXINV_Msk			(0x01 << UART_CFG_TXINV_Pos)

#define UART_TOCR_TIME_Pos			0		// Timeout duration, expressed in 10/BAUDRATE seconds
#define UART_TOCR_TIME_Msk			(0xFFF<< UART_TOCR_TIME_Pos)
#define UART_TOCR_MODE_Pos			12		// 0 trigger only when there is data in RX FIFO, 1 always trigger when the timeout time reaches
#define UART_TOCR_MODE_Msk			(0x01 << UART_TOCR_MODE_Pos)
#define UART_TOCR_IFCLR_Pos			13		// Timeout Interrupt Flag Clear
#define UART_TOCR_IFCLR_Msk			(0x01 << UART_TOCR_IFCLR_Pos)




typedef struct {
	__IO uint32_t CTRL;

	__IO uint32_t DATA;

	__IO uint32_t STAT;

	__IO uint32_t IE;

	__IO uint32_t IF;
	
	__IO uint32_t I2SCR;
	
	__IO uint32_t I2SPR;
} SPI_TypeDef;


#define SPI_CTRL_CLKDIV_Pos			0		// Clock Divider, SPI clock = SYS_Freq/pow(2, CLKDIV+2)
#define SPI_CTRL_CLKDIV_Msk			(0x07 << SPI_CTRL_CLKDIV_Pos)
#define SPI_CTRL_EN_Pos				3
#define SPI_CTRL_EN_Msk				(0x01 << SPI_CTRL_EN_Pos)
#define SPI_CTRL_SIZE_Pos			4		// Data Size Select, 3 4-bit, 4 5-bit, ..., 15 16-bit
#define SPI_CTRL_SIZE_Msk			(0x0F << SPI_CTRL_SIZE_Pos)
#define SPI_CTRL_CPHA_Pos			8		// 0 sample data on the first edge on SCLK, 1 sample data on the second edge on SCLK
#define SPI_CTRL_CPHA_Msk			(0x01 << SPI_CTRL_CPHA_Pos)
#define SPI_CTRL_CPOL_Pos			9		// 0 SCLK is low in idle state, 1 SCLK is high in idle state
#define SPI_CTRL_CPOL_Msk			(0x01 << SPI_CTRL_CPOL_Pos)
#define SPI_CTRL_FFS_Pos			10		// Frame Format Select, 0 SPI, 1 TI SSI, 2 I2S, 3 SPI Flash
#define SPI_CTRL_FFS_Msk			(0x03 << SPI_CTRL_FFS_Pos)
#define SPI_CTRL_MSTR_Pos			12		// Master mode
#define SPI_CTRL_MSTR_Msk			(0x01 << SPI_CTRL_MSTR_Pos)
#define SPI_CTRL_FAST_Pos			13		// 1 SPI clock = SYS_Freq/2, 0 SPI clock set by CLKDIV field
#define SPI_CTRL_FAST_Msk			(0x01 << SPI_CTRL_FAST_Pos)
#define SPI_CTRL_DMATXEN_Pos		14		//
#define SPI_CTRL_DMATXEN_Msk		(0x01 << SPI_CTRL_DMATXEN_Pos)
#define SPI_CTRL_DMARXEN_Pos		15		//
#define SPI_CTRL_DMARXEN_Msk		(0x01 << SPI_CTRL_DMARXEN_Pos)
#define SPI_CTRL_FILTE_Pos			16		// input signal debounce enable
#define SPI_CTRL_FILTE_Msk			(0x01 << SPI_CTRL_FILTE_Pos)
#define SPI_CTRL_SSN_H_Pos			17		// 1 pull the SSN up half a clock cycle between each character
#define SPI_CTRL_SSN_H_Msk			(0x01 << SPI_CTRL_SSN_H_Pos)
#define SPI_CTRL_RFTHR_Pos			18		// RX FIFO Threshold
#define SPI_CTRL_RFTHR_Msk			(0x07 << SPI_CTRL_RFTHR_Pos)
#define SPI_CTRL_TFTHR_Pos			21		// TX FIFO Threshold
#define SPI_CTRL_TFTHR_Msk			(0x07 << SPI_CTRL_TFTHR_Pos)
#define SPI_CTRL_RFCLR_Pos			24		// RX FIFO Clear
#define SPI_CTRL_RFCLR_Msk			(0x01 << SPI_CTRL_RFCLR_Pos)
#define SPI_CTRL_TFCLR_Pos			25		// TX FIFO Clear
#define SPI_CTRL_TFCLR_Msk			(0x01 << SPI_CTRL_TFCLR_Pos)
#define SPI_CTRL_LSBF_Pos			28		// LSB Fisrt
#define SPI_CTRL_LSBF_Msk			(0x01 << SPI_CTRL_LSBF_Pos)
#define SPI_CTRL_NSYNC_Pos			29
#define SPI_CTRL_NSYNC_Msk			(0x01 << SPI_CTRL_NSYNC_Pos)

#define SPI_STAT_WTC_Pos			0		// Word Transmit Complete
#define SPI_STAT_WTC_Msk			(0x01 << SPI_STAT_WTC_Pos)
#define SPI_STAT_TFE_Pos			1		// TX FIFO Empty
#define SPI_STAT_TFE_Msk			(0x01 << SPI_STAT_TFE_Pos)
#define SPI_STAT_TFNF_Pos			2		// TX FIFO Not Full
#define SPI_STAT_TFNF_Msk			(0x01 << SPI_STAT_TFNF_Pos)
#define SPI_STAT_RFNE_Pos			3		// RX FIFO Not Empty
#define SPI_STAT_RFNE_Msk			(0x01 << SPI_STAT_RFNE_Pos)
#define SPI_STAT_RFF_Pos			4		// RX FIFO Full
#define SPI_STAT_RFF_Msk			(0x01 << SPI_STAT_RFF_Pos)
#define SPI_STAT_RFOV_Pos			5		// RX FIFO Overflow
#define SPI_STAT_RFOV_Msk			(0x01 << SPI_STAT_RFOV_Pos)
#define SPI_STAT_TFLVL_Pos			6		// the number of data in TX FIFO, 0 8 data when TFNF=0, 0 data when TFNF=1,	1--7 1--7 data
#define SPI_STAT_TFLVL_Msk			(0x07 << SPI_STAT_TFLVL_Pos)
#define SPI_STAT_RFLVL_Pos			9		// the number of data in RX FIFO, 0 8 data when RFF =1, 0 data when RFF =0, 1--7 1--7 data
#define SPI_STAT_RFLVL_Msk			(0x07 << SPI_STAT_RFLVL_Pos)
#define SPI_STAT_BUSY_Pos			15
#define SPI_STAT_BUSY_Msk			(0x01 << SPI_STAT_BUSY_Pos)

#define SPI_IE_RFOV_Pos				0
#define SPI_IE_RFOV_Msk				(0x01 << SPI_IE_RFOV_Pos)
#define SPI_IE_RFF_Pos				1
#define SPI_IE_RFF_Msk				(0x01 << SPI_IE_RFF_Pos)
#define SPI_IE_RFHF_Pos				2
#define SPI_IE_RFHF_Msk				(0x01 << SPI_IE_RFHF_Pos)
#define SPI_IE_TFE_Pos				3
#define SPI_IE_TFE_Msk				(0x01 << SPI_IE_TFE_Pos)
#define SPI_IE_TFHF_Pos				4		// the number of data in TX FIFO is greater than 4
#define SPI_IE_TFHF_Msk				(0x01 << SPI_IE_TFHF_Pos)
#define SPI_IE_RFTHR_Pos			5		// 1 trigger interrupt when STAT.RFLVL > CTRL.RFTHR
#define SPI_IE_RFTHR_Msk			(0x01 << SPI_IE_RFTHR_Pos)
#define SPI_IE_TFTHR_Pos			6		// 1 trigger interrupt when STAT.TXLVL < CTRL.TFTHR
#define SPI_IE_TFTHR_Msk			(0x01 << SPI_IE_TFTHR_Pos)
#define SPI_IE_WTC_Pos				8		// Word Transmit Complete
#define SPI_IE_WTC_Msk				(0x01 << SPI_IE_WTC_Pos)
#define SPI_IE_FTC_Pos				9		// Frame Transmit Complete
#define SPI_IE_FTC_Msk				(0x01 << SPI_IE_FTC_Pos)
#define SPI_IE_SSFALL_Pos			10		// Slave Select Fall Edge
#define SPI_IE_SSFALL_Msk			(0x01 << SPI_IE_SSFALL_Pos)
#define SPI_IE_SSRISE_Pos			11		// Slave Select Rise Edge
#define SPI_IE_SSRISE_Msk			(0x01 << SPI_IE_SSRISE_Pos)

#define SPI_IF_RFOV_Pos				0
#define SPI_IF_RFOV_Msk				(0x01 << SPI_IF_RFOV_Pos)
#define SPI_IF_RFF_Pos				1
#define SPI_IF_RFF_Msk				(0x01 << SPI_IF_RFF_Pos)
#define SPI_IF_RFHF_Pos				2
#define SPI_IF_RFHF_Msk				(0x01 << SPI_IF_RFHF_Pos)
#define SPI_IF_TFE_Pos				3
#define SPI_IF_TFE_Msk				(0x01 << SPI_IF_TFE_Pos)
#define SPI_IF_TFHF_Pos				4
#define SPI_IF_TFHF_Msk				(0x01 << SPI_IF_TFHF_Pos)
#define SPI_IF_RFTHR_Pos			5
#define SPI_IF_RFTHR_Msk			(0x01 << SPI_IF_RFTHR_Pos)
#define SPI_IF_TFTHR_Pos			6
#define SPI_IF_TFTHR_Msk			(0x01 << SPI_IF_TFTHR_Pos)
#define SPI_IF_WTC_Pos				8
#define SPI_IF_WTC_Msk				(0x01 << SPI_IF_WTC_Pos)
#define SPI_IF_FTC_Pos				9
#define SPI_IF_FTC_Msk				(0x01 << SPI_IF_FTC_Pos)
#define SPI_IF_SSFALL_Pos			10
#define SPI_IF_SSFALL_Msk			(0x01 << SPI_IF_SSFALL_Pos)
#define SPI_IF_SSRISE_Pos			11
#define SPI_IF_SSRISE_Msk			(0x01 << SPI_IF_SSRISE_Pos)

#define SPI_I2SCR_DIEN_Pos			0		// Data Input Enable
#define SPI_I2SCR_DIEN_Msk			(0x01 << SPI_I2SCR_DIEN_Pos)
#define SPI_I2SCR_DOEN_Pos			1		// Data Output Enable
#define SPI_I2SCR_DOEN_Msk			(0x01 << SPI_I2SCR_DOEN_Pos)
#define SPI_I2SCR_MSTR_Pos			2		// Master Mode
#define SPI_I2SCR_MSTR_Msk			(0x01 << SPI_I2SCR_MSTR_Pos)
#define SPI_I2SCR_EN_Pos			3
#define SPI_I2SCR_EN_Msk			(0x01 << SPI_I2SCR_EN_Pos)
#define SPI_I2SCR_FFMT_Pos			4		// I2S Frame Format, 0 I2S philips, 1 MSB justified, 2 PCM Short, 3 PCM Long
#define SPI_I2SCR_FFMT_Msk			(0x03 << SPI_I2SCR_FFMT_Pos)
#define SPI_I2SCR_DLEN_Pos			6		// I2S Data Length,  0 8-bit, 1 16-bit 2 24-bit 3 32-bit
#define SPI_I2SCR_DLEN_Msk			(0x03 << SPI_I2SCR_DLEN_Pos)
#define SPI_I2SCR_PCMSYNW_Pos		8		// I2S PCM Long Mode Sync Width, 0 1 SCLK period, 1 1 Data Length
#define SPI_I2SCR_PCMSYNW_Msk		(0x01 << SPI_I2SCR_PCMSYNW_Pos)
#define SPI_I2SCR_MCLKOE_Pos		9		// MCLK Output Enable
#define SPI_I2SCR_MCLKOE_Msk		(0x01 << SPI_I2SCR_MCLKOE_Pos)
#define SPI_I2SCR_CHLEN_Pos			10		// Channel width, 0 16-bit, 1 32-bit
#define SPI_I2SCR_CHLEN_Msk			(0x01 << SPI_I2SCR_CHLEN_Pos)
#define SPI_I2SCR_CHRIGHT_Pos		16		// 1 Right Channel, 0 Left Channel
#define SPI_I2SCR_CHRIGHT_Msk		(0x01 << SPI_I2SCR_CHRIGHT_Pos)

#define SPI_I2SPR_MCLKDIV_Pos		0		// Fmclk = Fpclk / (2 * (MCLKDIV + 1))
#define SPI_I2SPR_MCLKDIV_Msk		(0x3F << SPI_I2SPR_MCLKDIV_Pos)
#define SPI_I2SPR_SCLKDIV_Pos		8		// Fsclk = Fpclk / (2 * (SCLKDIV + 1))
#define SPI_I2SPR_SCLKDIV_Msk		(0xFFF<< SPI_I2SPR_SCLKDIV_Pos)




typedef struct {
	__IO uint32_t CR;

	__IO uint32_t SR;

	__IO uint32_t TR;						// Transfer Register

	__IO uint32_t RXDATA;
	
	__IO uint32_t TXDATA;
	
	__IO uint32_t IF;
	
	__IO uint32_t IE;
	
		 uint32_t RESERVED1;
	
	__IO uint32_t MCR;						// Master Control Register
	
	__IO uint32_t CLK;
	
		 uint32_t RESERVED2[2];
	
	__IO uint32_t SCR;						// Slave Control Register
	
	__IO uint32_t SADDR;
} I2C_TypeDef;


#define I2C_CR_EN_Pos				0	
#define I2C_CR_EN_Msk				(0x01 << I2C_CR_EN_Pos)
#define I2C_CR_MASTER_Pos			1		// Master mode
#define I2C_CR_MASTER_Msk			(0x01 << I2C_CR_MASTER_Pos)
#define I2C_CR_HS_Pos				2		// 1 High-Speed mode, 0 Standard-mode or Fast-mode
#define I2C_CR_HS_Msk				(0x01 << I2C_CR_HS_Pos)
#define I2C_CR_DNF_Pos				3		// Digital Noise Filter, pulse narrower than DNF+1 are considered glitch
#define I2C_CR_DNF_Msk				(0x0F << I2C_CR_DNF_Pos)

#define I2C_SR_BUSY_Pos				0
#define I2C_SR_BUSY_Msk				(0x01 << I2C_SR_BUSY_Pos)
#define I2C_SR_SCL_Pos				1		// SCL Line Level
#define I2C_SR_SCL_Msk				(0x01 << I2C_SR_SCL_Pos)
#define I2C_SR_SDA_Pos				2		// SDA Line Level
#define I2C_SR_SDA_Msk				(0x01 << I2C_SR_SDA_Pos)

#define I2C_TR_TXACK_Pos			0		// when as receiver, ACK level to send
#define I2C_TR_TXACK_Msk			(0x01 << I2C_TR_TXACK_Pos)
#define I2C_TR_RXACK_Pos			1		// when as tramsmiter, ACK value received
#define I2C_TR_RXACK_Msk			(0x01 << I2C_TR_RXACK_Pos)
#define I2C_TR_TXCLR_Pos			2		// TX Data Clear, auto returning to zero
#define I2C_TR_TXCLR_Msk			(0x01 << I2C_TR_TXCLR_Pos)
#define I2C_TR_SLVACT_Pos			8		// Slave Active, set when selected by host
#define I2C_TR_SLVACT_Msk			(0x01 << I2C_TR_SLVACT_Pos)
#define I2C_TR_SLVRD_Pos			9		// Slave Read mode, set when receive read request
#define I2C_TR_SLVRD_Msk			(0x01 << I2C_TR_SLVRD_Pos)
#define I2C_TR_SLVWR_Pos			10		// Slave Write mode, set when receive write request
#define I2C_TR_SLVWR_Msk			(0x01 << I2C_TR_SLVWR_Pos)
#define I2C_TR_SLVSTR_Pos			11		// Slave clock stretching
#define I2C_TR_SLVSTR_Msk			(0x01 << I2C_TR_SLVSTR_Pos)
#define I2C_TR_SLVRDS_Pos			12		// Slave RXDATA Status, 0 empty, 1 receive address, 2 receive data, 3 receive Master Code
#define I2C_TR_SLVRDS_Msk			(0x03 << I2C_TR_SLVRDS_Pos)

#define I2C_IF_TXE_Pos				0		// TX Empty, clear by write TXDATA
#define I2C_IF_TXE_Msk				(0x01 << I2C_IF_TXE_Pos)
#define I2C_IF_RXNE_Pos				1		// RX Not Empty, clear by read RXDATA
#define I2C_IF_RXNE_Msk				(0x01 << I2C_IF_RXNE_Pos)
#define I2C_IF_RXOV_Pos				2		// RX Overflow, clear by read
#define I2C_IF_RXOV_Msk				(0x01 << I2C_IF_RXOV_Pos)
#define I2C_IF_TXDONE_Pos			3		// TX Done, write 1 to clear
#define I2C_IF_TXDONE_Msk			(0x01 << I2C_IF_TXDONE_Pos)
#define I2C_IF_RXDONE_Pos			4		// RX Done, write 1 to clear
#define I2C_IF_RXDONE_Msk			(0x01 << I2C_IF_RXDONE_Pos)
#define I2C_IF_RXSTA_Pos			8		// slave received start bit, write 1 to clear
#define I2C_IF_RXSTA_Msk			(0x01 << I2C_IF_RXSTA_Pos)
#define I2C_IF_RXSTO_Pos			9		// slave received stop bit, write 1 to clear
#define I2C_IF_RXSTO_Msk			(0x01 << I2C_IF_RXSTO_Pos)
#define I2C_IF_AL_Pos				16		// host lost arbitration, write 1 to clear
#define I2C_IF_AL_Msk				(0x01 << I2C_IF_AL_Pos)
#define I2C_IF_MLTO_Pos				17		// Master SCL Low Timeout, write 1 to clear
#define I2C_IF_MLTO_Msk				(0x01 << I2C_IF_MLTO_Pos)

#define I2C_IE_TXE_Pos				0
#define I2C_IE_TXE_Msk				(0x01 << I2C_IE_TXE_Pos)
#define I2C_IE_RXNE_Pos				1
#define I2C_IE_RXNE_Msk				(0x01 << I2C_IE_RXNE_Pos)
#define I2C_IE_RXOV_Pos				2
#define I2C_IE_RXOV_Msk				(0x01 << I2C_IE_RXOV_Pos)
#define I2C_IE_TXDONE_Pos			3
#define I2C_IE_TXDONE_Msk			(0x01 << I2C_IE_TXDONE_Pos)
#define I2C_IE_RXDONE_Pos			4
#define I2C_IE_RXDONE_Msk			(0x01 << I2C_IE_RXDONE_Pos)
#define I2C_IE_RXSTA_Pos			8
#define I2C_IE_RXSTA_Msk			(0x01 << I2C_IE_RXSTA_Pos)
#define I2C_IE_RXSTO_Pos			9
#define I2C_IE_RXSTO_Msk			(0x01 << I2C_IE_RXSTO_Pos)
#define I2C_IE_AL_Pos				16
#define I2C_IE_AL_Msk				(0x01 << I2C_IE_AL_Pos)
#define I2C_IE_MLTO_Pos				17
#define I2C_IE_MLTO_Msk				(0x01 << I2C_IE_MLTO_Pos)

#define I2C_MCR_STA_Pos				0		// Writing 1 generate start bit and automatically clear to zero after completion
#define I2C_MCR_STA_Msk				(0x01 << I2C_MCR_STA_Pos)
#define I2C_MCR_RD_Pos				1
#define I2C_MCR_RD_Msk				(0x01 << I2C_MCR_RD_Pos)
#define I2C_MCR_WR_Pos				2
#define I2C_MCR_WR_Msk				(0x01 << I2C_MCR_WR_Pos)
#define I2C_MCR_STO_Pos				3		// Writing 1 generate stop bit and automatically clear to zero after completion
#define I2C_MCR_STO_Msk				(0x01 << I2C_MCR_STO_Pos)

#define I2C_CLK_SCLL_Pos			0		// SCL Low Time
#define I2C_CLK_SCLL_Msk			(0xFF << I2C_CLK_SCLL_Pos)
#define I2C_CLK_SCLH_Pos			8		// SCL High Time
#define I2C_CLK_SCLH_Msk			(0xFF << I2C_CLK_SCLH_Pos)
#define I2C_CLK_DIV_Pos				16
#define I2C_CLK_DIV_Msk				(0xFF << I2C_CLK_DIV_Pos)
#define I2C_CLK_SDAH_Pos			24		// SDA Hold Time
#define I2C_CLK_SDAH_Msk			(0x0F << I2C_CLK_SDAH_Pos)

#define I2C_SCR_ADDR10_Pos			0		// 10-bit address enable
#define I2C_SCR_ADDR10_Msk			(0x01 << I2C_SCR_ADDR10_Pos)
#define I2C_SCR_MCDE_Pos			1		// Master Code Detect Enable
#define I2C_SCR_MCDE_Msk			(0x01 << I2C_SCR_MCDE_Pos)
#define I2C_SCR_STRE_Pos			2		// Clock Stretching Enable
#define I2C_SCR_STRE_Msk			(0x01 << I2C_SCR_STRE_Pos)
#define I2C_SCR_ASDS_Pos			3		// Adaptive Stretching Data Setup
#define I2C_SCR_ASDS_Msk			(0x01 << I2C_SCR_ASDS_Pos)

#define I2C_SADDR_ADDR7_Pos			1		// address for 7-bit address mode
#define I2C_SADDR_ADDR7_Msk			(0x7F << I2C_SADDR_ADDR7_Pos)
#define I2C_SADDR_ADDR10_Pos		0		// address for 10-bit address mode
#define I2C_SADDR_ADDR10_Msk		(0x3FF<< I2C_SADDR_ADDR10_Pos)
#define I2C_SADDR_MASK7_Pos			17		// address mask for 7-bit address mode, ADDR7 & (~MASK7) compare with received address
#define I2C_SADDR_MASK7_Msk			(0x7F << I2C_SADDR_MASK7_Pos)
#define I2C_SADDR_MASK10_Pos		16		// address mask for 10-bit address mode, only mask the lower 8-bits
#define I2C_SADDR_MASK10_Msk		(0xFF << I2C_SADDR_MASK10_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__IO uint32_t GO;
	
	__IO uint32_t IE;
	
	__IO uint32_t IF;
	
	struct {
		__IO uint32_t SR;
		
		__IO uint32_t DR;
		
		__IO uint32_t CMP;
		
			 uint32_t RESERVED;
	} SEQ[4];
	
	     uint32_t RESERVED[15];
    
	__IO uint32_t SEQCHN0;
	
	__IO uint32_t SEQCHN1;					// Sequencer channel select
    
	__IO uint32_t SEQTRG;					// Sequencer trigger select
	
	__IO uint32_t SEQCOV;					// Sequencer conversion times
	
	__IO uint32_t SEQSMP;					// Sequencer sample time
	
	     uint32_t RESERVED2[4];
	
	__IO uint32_t CR2;
	
		 uint32_t RESERVED3[15];
	
	__IO uint32_t CALIB;
} ADC_TypeDef;


#define ADC_CR_EN_Pos				0
#define ADC_CR_EN_Msk				(0x01 << ADC_CR_EN_Pos)
#define ADC_CR_AVG_Pos				1
#define ADC_CR_AVG_Msk				(0x03 << ADC_CR_AVG_Pos)
#define ADC_CR_RESET_Pos			3
#define ADC_CR_RESET_Msk			(0x01 << ADC_CR_RESET_Pos)
#define ADC_CR_DMAEN_Pos			4		// Sequencer DMA enable, 1-bit per Sequencer
#define ADC_CR_DMAEN_Msk			(0x0F << ADC_CR_DMAEN_Pos)
#define ADC_CR_FFCLR_Pos			8		// FIFO Clear, 1-bit per Sequencer
#define ADC_CR_FFCLR_Msk			(0x0F << ADC_CR_FFCLR_Pos)

#define ADC_GO_SEQ0_Pos				0
#define ADC_GO_SEQ0_Msk				(0x01 << ADC_GO_SEQ0_Pos)
#define ADC_GO_SEQ1_Pos				1
#define ADC_GO_SEQ1_Msk				(0x01 << ADC_GO_SEQ1_Pos)
#define ADC_GO_SEQ2_Pos				2
#define ADC_GO_SEQ2_Msk				(0x01 << ADC_GO_SEQ2_Pos)
#define ADC_GO_SEQ3_Pos				3
#define ADC_GO_SEQ3_Msk				(0x01 << ADC_GO_SEQ3_Pos)
#define ADC_GO_BUSY_Pos				4
#define ADC_GO_BUSY_Msk				(0x01 << ADC_GO_BUSY_Pos)

#define ADC_IE_SEQ0EOC_Pos			0		// Sequencer 0 End Of Convertion interrupt enable
#define ADC_IE_SEQ0EOC_Msk			(0x01 << ADC_IE_SEQ0EOC_Pos)
#define ADC_IE_SEQ0OVF_Pos			1		// Sequencer 0 FIFO Overflow interrupt enable
#define ADC_IE_SEQ0OVF_Msk			(0x01 << ADC_IE_SEQ0OVF_Pos)
#define ADC_IE_SEQ0HALF_Pos			2       // Sequencer 0 FIFO Half Full interrupt enable
#define ADC_IE_SEQ0HALF_Msk			(0x01 << ADC_IE_SEQ0HALF_Pos)
#define ADC_IE_SEQ0FULL_Pos			3       // Sequencer 0 FIFO Full interrupt enable
#define ADC_IE_SEQ0FULL_Msk			(0x01 << ADC_IE_SEQ0FULL_Pos)
#define ADC_IE_SEQ0CMPMAX_Pos		4		// Sequencer 0 conversion result > COMP.MAX interrupt enable
#define ADC_IE_SEQ0CMPMAX_Msk		(0x01 << ADC_IE_SEQ0CMPMAX_Pos)
#define ADC_IE_SEQ0CMPMIN_Pos		5		// Sequencer 0 conversion result < COMP.MIN interrupt enable
#define ADC_IE_SEQ0CMPMIN_Msk		(0x01 << ADC_IE_SEQ0CMPMIN_Pos)
#define ADC_IE_SEQ1EOC_Pos			8
#define ADC_IE_SEQ1EOC_Msk			(0x01 << ADC_IE_SEQ1EOC_Pos)
#define ADC_IE_SEQ1OVF_Pos			9
#define ADC_IE_SEQ1OVF_Msk			(0x01 << ADC_IE_SEQ1OVF_Pos)
#define ADC_IE_SEQ1HALF_Pos			10
#define ADC_IE_SEQ1HALF_Msk			(0x01 << ADC_IE_SEQ1HALF_Pos)
#define ADC_IE_SEQ1FULL_Pos			11
#define ADC_IE_SEQ1FULL_Msk			(0x01 << ADC_IE_SEQ1FULL_Pos)
#define ADC_IE_SEQ1CMPMAX_Pos		12
#define ADC_IE_SEQ1CMPMAX_Msk		(0x01 << ADC_IE_SEQ1CMPMAX_Pos)
#define ADC_IE_SEQ1CMPMIN_Pos		13
#define ADC_IE_SEQ1CMPMIN_Msk		(0x01 << ADC_IE_SEQ1CMPMIN_Pos)
#define ADC_IE_SEQ2EOC_Pos			16
#define ADC_IE_SEQ2EOC_Msk			(0x01 << ADC_IE_SEQ2EOC_Pos)
#define ADC_IE_SEQ2OVF_Pos			17
#define ADC_IE_SEQ2OVF_Msk			(0x01 << ADC_IE_SEQ2OVF_Pos)
#define ADC_IE_SEQ2HALF_Pos			18
#define ADC_IE_SEQ2HALF_Msk			(0x01 << ADC_IE_SEQ2HALF_Pos)
#define ADC_IE_SEQ2FULL_Pos			19
#define ADC_IE_SEQ2FULL_Msk			(0x01 << ADC_IE_SEQ2FULL_Pos)
#define ADC_IE_SEQ2CMPMAX_Pos		20
#define ADC_IE_SEQ2CMPMAX_Msk		(0x01 << ADC_IE_SEQ2CMPMAX_Pos)
#define ADC_IE_SEQ2CMPMIN_Pos		21
#define ADC_IE_SEQ2CMPMIN_Msk		(0x01 << ADC_IE_SEQ2CMPMIN_Pos)
#define ADC_IE_SEQ3EOC_Pos			24
#define ADC_IE_SEQ3EOC_Msk			(0x01 << ADC_IE_SEQ3EOC_Pos)
#define ADC_IE_SEQ3OVF_Pos			25
#define ADC_IE_SEQ3OVF_Msk			(0x01 << ADC_IE_SEQ3OVF_Pos)
#define ADC_IE_SEQ3HALF_Pos			26
#define ADC_IE_SEQ3HALF_Msk			(0x01 << ADC_IE_SEQ3HALF_Pos)
#define ADC_IE_SEQ3FULL_Pos			27
#define ADC_IE_SEQ3FULL_Msk			(0x01 << ADC_IE_SEQ3FULL_Pos)
#define ADC_IE_SEQ3CMPMAX_Pos		28
#define ADC_IE_SEQ3CMPMAX_Msk		(0x01 << ADC_IE_SEQ3CMPMAX_Pos)
#define ADC_IE_SEQ3CMPMIN_Pos		29
#define ADC_IE_SEQ3CMPMIN_Msk		(0x01 << ADC_IE_SEQ3CMPMIN_Pos)

#define ADC_IF_SEQ0EOC_Pos			0		// Sequencer 0 End Of Convertion interrupt flag
#define ADC_IF_SEQ0EOC_Msk			(0x01 << ADC_IF_SEQ0EOC_Pos)
#define ADC_IF_SEQ0OVF_Pos			1
#define ADC_IF_SEQ0OVF_Msk			(0x01 << ADC_IF_SEQ0OVF_Pos)
#define ADC_IF_SEQ0HALF_Pos			2
#define ADC_IF_SEQ0HALF_Msk			(0x01 << ADC_IF_SEQ0HALF_Pos)
#define ADC_IF_SEQ0FULL_Pos			3
#define ADC_IF_SEQ0FULL_Msk			(0x01 << ADC_IF_SEQ0FULL_Pos)
#define ADC_IF_SEQ0CMPMAX_Pos		4
#define ADC_IF_SEQ0CMPMAX_Msk		(0x01 << ADC_IF_SEQ0CMPMAX_Pos)
#define ADC_IF_SEQ0CMPMIN_Pos		5
#define ADC_IF_SEQ0CMPMIN_Msk		(0x01 << ADC_IF_SEQ0CMPMIN_Pos)
#define ADC_IF_SEQ1EOC_Pos			8
#define ADC_IF_SEQ1EOC_Msk			(0x01 << ADC_IF_SEQ1EOC_Pos)
#define ADC_IF_SEQ1OVF_Pos			9
#define ADC_IF_SEQ1OVF_Msk			(0x01 << ADC_IF_SEQ1OVF_Pos)
#define ADC_IF_SEQ1HALF_Pos			10
#define ADC_IF_SEQ1HALF_Msk			(0x01 << ADC_IF_SEQ1HALF_Pos)
#define ADC_IF_SEQ1FULL_Pos			11
#define ADC_IF_SEQ1FULL_Msk			(0x01 << ADC_IF_SEQ1FULL_Pos)
#define ADC_IF_SEQ1CMPMAX_Pos		12
#define ADC_IF_SEQ1CMPMAX_Msk		(0x01 << ADC_IF_SEQ1CMPMAX_Pos)
#define ADC_IF_SEQ1CMPMIN_Pos		13
#define ADC_IF_SEQ1CMPMIN_Msk		(0x01 << ADC_IF_SEQ1CMPMIN_Pos)
#define ADC_IF_SEQ2EOC_Pos			16
#define ADC_IF_SEQ2EOC_Msk			(0x01 << ADC_IF_SEQ2EOC_Pos)
#define ADC_IF_SEQ2OVF_Pos			17
#define ADC_IF_SEQ2OVF_Msk			(0x01 << ADC_IF_SEQ2OVF_Pos)
#define ADC_IF_SEQ2HALF_Pos			18
#define ADC_IF_SEQ2HALF_Msk			(0x01 << ADC_IF_SEQ2HALF_Pos)
#define ADC_IF_SEQ2FULL_Pos			19
#define ADC_IF_SEQ2FULL_Msk			(0x01 << ADC_IF_SEQ2FULL_Pos)
#define ADC_IF_SEQ2CMPMAX_Pos		20
#define ADC_IF_SEQ2CMPMAX_Msk		(0x01 << ADC_IF_SEQ2CMPMAX_Pos)
#define ADC_IF_SEQ2CMPMIN_Pos		21
#define ADC_IF_SEQ2CMPMIN_Msk		(0x01 << ADC_IF_SEQ2CMPMIN_Pos)
#define ADC_IF_SEQ3EOC_Pos			24
#define ADC_IF_SEQ3EOC_Msk			(0x01 << ADC_IF_SEQ3EOC_Pos)
#define ADC_IF_SEQ3OVF_Pos			25
#define ADC_IF_SEQ3OVF_Msk			(0x01 << ADC_IF_SEQ3OVF_Pos)
#define ADC_IF_SEQ3HALF_Pos			26
#define ADC_IF_SEQ3HALF_Msk			(0x01 << ADC_IF_SEQ3HALF_Pos)
#define ADC_IF_SEQ3FULL_Pos			27
#define ADC_IF_SEQ3FULL_Msk			(0x01 << ADC_IF_SEQ3FULL_Pos)
#define ADC_IF_SEQ3CMPMAX_Pos		28
#define ADC_IF_SEQ3CMPMAX_Msk		(0x01 << ADC_IF_SEQ3CMPMAX_Pos)
#define ADC_IF_SEQ3CMPMIN_Pos		29
#define ADC_IF_SEQ3CMPMIN_Msk		(0x01 << ADC_IF_SEQ3CMPMIN_Pos)

#define ADC_SR_EOC_Pos				0		// End Of Conversion, automatically clear when next conersion start
#define ADC_SR_EOC_Msk				(0x01 << ADC_SR_EOC_Pos)
#define ADC_SR_OVF_Pos				1		// FIFO Overflow
#define ADC_SR_OVF_Msk				(0x01 << ADC_SR_OVF_Pos)
#define ADC_SR_HALF_Pos				2		// FIFO Half Full
#define ADC_SR_HALF_Msk				(0x01 << ADC_SR_HALF_Pos)
#define ADC_SR_FULL_Pos				3		// FIFO Full
#define ADC_SR_FULL_Msk				(0x01 << ADC_SR_FULL_Pos)
#define ADC_SR_EMPTY_Pos			4		// FIFO Empty
#define ADC_SR_EMPTY_Msk			(0x01 << ADC_SR_EMPTY_Pos)
#define ADC_SR_LEVEL_Pos			5		// the number of data in FIFO, value 0 indicates 0 or 8 data
#define ADC_SR_LEVEL_Msk			(0x07 << ADC_SR_LEVEL_Pos)

#define ADC_DR_VALUE_Pos			0
#define ADC_DR_VALUE_Msk			(0xFFF<< ADC_DR_VALUE_Pos)
#define ADC_DR_CHNUM_Pos			12
#define ADC_DR_CHNUM_Msk			(0x0F << ADC_DR_CHNUM_Pos)

#define ADC_CMP_MAX_Pos				0
#define ADC_CMP_MAX_Msk				(0xFFF<< ADC_CMP_MAX_Pos)
#define ADC_CMP_MIN_Pos				16
#define ADC_CMP_MIN_Msk				(0xFFF<< ADC_CMP_MIN_Pos)

#define ADC_SEQCHN0_SEQ0_Pos		0		// Sequencer 0 channel select, one bit per channel
#define ADC_SEQCHN0_SEQ0_Msk		(0xFFF << ADC_SEQCHN0_SEQ0_Pos)
#define ADC_SEQCHN0_SEQ1_Pos		16
#define ADC_SEQCHN0_SEQ1_Msk		(0xFFF << ADC_SEQCHN0_SEQ1_Pos)

#define ADC_SEQCHN1_SEQ2_Pos		0
#define ADC_SEQCHN1_SEQ2_Msk		(0xFFF << ADC_SEQCHN1_SEQ2_Pos)
#define ADC_SEQCHN1_SEQ3_Pos		16
#define ADC_SEQCHN1_SEQ3_Msk		(0xFFF << ADC_SEQCHN1_SEQ3_Pos)

#define ADC_SEQTRG_SEQ0_Pos			0		// Sequencer 0 trigger select, 0x01 CPU, 0x02 TIMR2, 0x03 TIMR3, 0x04-07 ADC_TRIG0-3 pin, 0x10-1F PWM0A-PWM7B
#define ADC_SEQTRG_SEQ0_Msk			(0x1F << ADC_SEQTRG_SEQ0_Pos)
#define ADC_SEQTRG_SEQ1_Pos			8
#define ADC_SEQTRG_SEQ1_Msk			(0x1F << ADC_SEQTRG_SEQ1_Pos)
#define ADC_SEQTRG_SEQ2_Pos			16
#define ADC_SEQTRG_SEQ2_Msk			(0x1F << ADC_SEQTRG_SEQ2_Pos)
#define ADC_SEQTRG_SEQ3_Pos			24
#define ADC_SEQTRG_SEQ3_Msk			(0x1F << ADC_SEQTRG_SEQ3_Pos)

#define ADC_SEQCOV_SEQ0_Pos			0		// Sequencer 0 conversion times, 0 indicates 1 time
#define ADC_SEQCOV_SEQ0_Msk			(0xFF << ADC_SEQCOV_SEQ0_Pos)
#define ADC_SEQCOV_SEQ1_Pos			8
#define ADC_SEQCOV_SEQ1_Msk			(0xFF << ADC_SEQCOV_SEQ1_Pos)
#define ADC_SEQCOV_SEQ2_Pos			16
#define ADC_SEQCOV_SEQ2_Msk			(0xFF << ADC_SEQCOV_SEQ2_Pos)
#define ADC_SEQCOV_SEQ3_Pos			24
#define ADC_SEQCOV_SEQ3_Msk			(0xFF << ADC_SEQCOV_SEQ3_Pos)

#define ADC_SEQSMP_SEQ0_Pos			0		// Sequencer 0 sample time, 0/1/2/3 indicates 1/2/4/8 sample clock period respectively
#define ADC_SEQSMP_SEQ0_Msk			(0x07 << ADC_SEQSMP_SEQ0_Pos)
#define ADC_SEQSMP_SEQ1_Pos			4
#define ADC_SEQSMP_SEQ1_Msk			(0x07 << ADC_SEQSMP_SEQ1_Pos)
#define ADC_SEQSMP_SEQ2_Pos			8
#define ADC_SEQSMP_SEQ2_Msk			(0x07 << ADC_SEQSMP_SEQ2_Pos)
#define ADC_SEQSMP_SEQ3_Pos			12
#define ADC_SEQSMP_SEQ3_Msk			(0x07 << ADC_SEQSMP_SEQ3_Pos)

#define ADC_CR2_ENLDO_Pos			2		// LDO enable
#define ADC_CR2_ENLDO_Msk			(0x01 << ADC_CR2_ENLDO_Pos)
#define ADC_CR2_BITS_Pos			6		// the number of bits of ADC result, 0 6-bit, 1 8-bit, 2 10-bit, 3 12-bit
#define ADC_CR2_BITS_Msk			(0x03 << ADC_CR2_BITS_Pos)

#define ADC_CALIB_RESET_Pos			0		// calibration block reset, keep at least 2 sampling cycles
#define ADC_CALIB_RESET_Msk			(0x01 << ADC_CALIB_RESET_Pos)
#define ADC_CALIB_START_Pos			1		// write 1 starts calibration, automatically clear when calibration done
#define ADC_CALIB_START_Msk			(0x01 << ADC_CALIB_START_Pos)
#define ADC_CALIB_BUSY_Pos			2		// 1 calibration in process
#define ADC_CALIB_BUSY_Msk			(0x01 << ADC_CALIB_BUSY_Pos)
#define ADC_CALIB_LOAD_Pos			3		// write 1 to load calibration result to ADC, automatically clear
#define ADC_CALIB_LOAD_Msk			(0x01 << ADC_CALIB_LOAD_Pos)
#define ADC_CALIB_BYPASS_Pos		4		// 1 calibration block bypass
#define ADC_CALIB_BYPASS_Msk		(0x01 << ADC_CALIB_BYPASS_Pos)
#define ADC_CALIB_RESULT_Pos		8
#define ADC_CALIB_RESULT_Msk		(0x7F << ADC_CALIB_RESULT_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__IO uint32_t OCR;
	
	__IO uint32_t BRKCR;
	
	__IO uint32_t BRKIN;
	
		 uint32_t RESERVED[4];
	
	__IO uint32_t PERIOD;                   // [15:0] period
	
	__IO uint32_t CMPA;                   	// [15:0] high level duration, for PWM_EDGE_ALIGNED mode, PWM_CENTER_ALIGNED mode and the first half cycle in PWM_ASYM_CENTER_ALIGNED mode
	
	__IO uint32_t CMPB;						// [15:0] same as CMPA but for channel B
	
	__IO uint32_t DZA;                      // [9:0] deadzone
	
	__IO uint32_t DZB;
	
	__IO uint32_t CMPA2;					// only for PWM_ASYM_CENTER_ALIGNED mode, high level duration for the second half cycle
	
	__IO uint32_t CMPB2;					// same as CMPA2 but for channel B
	
		 uint32_t RESERVED2[5];
	
	__IO uint32_t OVFTRG;
	
	__IO uint32_t CMPTRG;
	
		 uint32_t RESERVED3[2];
	
	__IO uint32_t EVMUX;
	
    __IO uint32_t EVMSK;
	
		 uint32_t RESERVED4[2];
	
	__IO uint32_t IE;
	
	__IO uint32_t IF;
	
	__IO uint32_t VALUE;
	
	__IO uint32_t SR;
} PWM_TypeDef;


#define PWM_CR_MODE_Pos				0		// 0 edge alignment mode, 1 center alignment mode, 2 asymmetric center alignment mode
#define PWM_CR_MODE_Msk				(0x03 << PWM_CR_MODE_Pos)
#define PWM_CR_MULT_Pos				2		// 0 one-shot mode
#define PWM_CR_MULT_Msk				(0x01 << PWM_CR_MULT_Pos)
#define PWM_CR_DIR_Pos				3		// 0 counting up, 1 counting down
#define PWM_CR_DIR_Msk				(0x01 << PWM_CR_DIR_Pos)
#define PWM_CR_CLKSRC_Pos			4		// clock source, 0 system clock, 1 PWM_PULSE0 input, 2 PWM_PULSE1 input
#define PWM_CR_CLKSRC_Msk			(0x03 << PWM_CR_CLKSRC_Pos)
#define PWM_CR_CLKDIV_Pos			6		// clock divider, 0 div1, 1 div2, ..., 1023 div1024
#define PWM_CR_CLKDIV_Msk			(0x3FF<< PWM_CR_CLKDIV_Pos)
#define PWM_CR_RPTNUM_Pos			16		// when the counter overflows RPTNUM+1 times, the register reloads once
#define PWM_CR_RPTNUM_Msk			(0xFF << PWM_CR_RPTNUM_Pos)

#define PWM_OCR_IDLEA_Pos			0		// PWM channel A output level when stop state
#define PWM_OCR_IDLEA_Msk			(0x01 << PWM_OCR_IDLEA_Pos)
#define PWM_OCR_IDLEB_Pos			1
#define PWM_OCR_IDLEB_Msk			(0x01 << PWM_OCR_IDLEB_Pos)
#define PWM_OCR_IDLEAN_Pos			2
#define PWM_OCR_IDLEAN_Msk			(0x01 << PWM_OCR_IDLEAN_Pos)
#define PWM_OCR_IDLEBN_Pos			3
#define PWM_OCR_IDLEBN_Msk			(0x01 << PWM_OCR_IDLEBN_Pos)
#define PWM_OCR_INVA_Pos			4		// whether PWM channel A output inverse
#define PWM_OCR_INVA_Msk			(0x01 << PWM_OCR_INVA_Pos)
#define PWM_OCR_INVB_Pos			5
#define PWM_OCR_INVB_Msk			(0x01 << PWM_OCR_INVB_Pos)
#define PWM_OCR_INVAN_Pos			6
#define PWM_OCR_INVAN_Msk			(0x01 << PWM_OCR_INVAN_Pos)
#define PWM_OCR_INVBN_Pos			7
#define PWM_OCR_INVBN_Msk			(0x01 << PWM_OCR_INVBN_Pos)

#define PWM_BRKCR_OUTA_Pos			0		// PWM channel A output level when brake state
#define PWM_BRKCR_OUTA_Msk			(0x01 << PWM_BRKCR_OUTA_Pos)
#define PWM_BRKCR_OFFA_Pos			1		// PWM channel A output when brake signal disappear: 0 immediately recover, 1 stay unchange until overflow
#define PWM_BRKCR_OFFA_Msk			(0x01 << PWM_BRKCR_OFFA_Pos)
#define PWM_BRKCR_OUTB_Pos			4
#define PWM_BRKCR_OUTB_Msk			(0x01 << PWM_BRKCR_OUTB_Pos)
#define PWM_BRKCR_OFFB_Pos			5
#define PWM_BRKCR_OFFB_Msk			(0x01 << PWM_BRKCR_OFFB_Pos)
#define PWM_BRKCR_OUTAN_Pos			8
#define PWM_BRKCR_OUTAN_Msk			(0x01 << PWM_BRKCR_OUTAN_Pos)
#define PWM_BRKCR_OUTBN_Pos			9
#define PWM_BRKCR_OUTBN_Msk			(0x01 << PWM_BRKCR_OUTBN_Pos)
#define PWM_BRKCR_STPCNT_Pos		10		// PWM counter behave when brake signal active: 0 continue counting, 1 stop counting
#define PWM_BRKCR_STPCNT_Msk		(0x01 << PWM_BRKCR_STPCNT_Pos)
#define PWM_BRKCR_ACTIVE_Pos		17		// whether it is currently in the brake state
#define PWM_BRKCR_ACTIVE_Msk		(0x01 << PWM_BRKCR_ACTIVE_Pos)

#define PWM_BRKIN_BRK0A_Pos			0		// whether PWM channel A is affected by brake input PWM_BRK0
#define PWM_BRKIN_BRK0A_Msk			(0x01 << PWM_BRKIN_BRK0A_Pos)
#define PWM_BRKIN_BRK1A_Pos			1
#define PWM_BRKIN_BRK1A_Msk			(0x01 << PWM_BRKIN_BRK1A_Pos)
#define PWM_BRKIN_BRK2A_Pos			2
#define PWM_BRKIN_BRK2A_Msk			(0x01 << PWM_BRKIN_BRK2A_Pos)
#define PWM_BRKIN_BRK0B_Pos			4
#define PWM_BRKIN_BRK0B_Msk			(0x01 << PWM_BRKIN_BRK0B_Pos)
#define PWM_BRKIN_BRK1B_Pos			5
#define PWM_BRKIN_BRK1B_Msk			(0x01 << PWM_BRKIN_BRK1B_Pos)
#define PWM_BRKIN_BRK2B_Pos			6
#define PWM_BRKIN_BRK2B_Msk			(0x01 << PWM_BRKIN_BRK2B_Pos)

#define PWM_OVFTRG_UPEN_Pos			0		// overflow trigger enable when counting-up
#define PWM_OVFTRG_UPEN_Msk			(0x01 << PWM_OVFTRG_UPEN_Pos)
#define PWM_OVFTRG_DNEN_Pos			1		// overflow trigger enable when counting-down
#define PWM_OVFTRG_DNEN_Msk			(0x01 << PWM_OVFTRG_DNEN_Pos)
#define PWM_OVFTRG_MUX_Pos			2		// select which the generated trigger signal will be used as: 0 PWM_TRG_0, 1 PWM_TRG_1, ..., or 7 PWM_TRG_7
#define PWM_OVFTRG_MUX_Msk			(0x07 << PWM_OVFTRG_MUX_Pos)

#define PWM_CMPTRG_CMP_Pos			0		// when PWM counter == CMP, PWM will generate a trigger signal
#define PWM_CMPTRG_CMP_Msk			(0xFFFF<<PWM_CMPTRG_CMP_Pos)
#define PWM_CMPTRG_EN_Pos			16
#define PWM_CMPTRG_EN_Msk			(0x01 << PWM_CMPTRG_EN_Pos)
#define PWM_CMPTRG_MUX_Pos			17		// select which the generated trigger signal will be used as: 0 PWM_TRG_0, 1 PWM_TRG_1, ..., or 7 PWM_TRG_7
#define PWM_CMPTRG_MUX_Msk			(0x07 << PWM_CMPTRG_MUX_Pos)
#define PWM_CMPTRG_WIDTH_Pos		20		// the trigger signal width =  WIDTH * 4 clock cycle, so no trigger signal output when width == 0
#define PWM_CMPTRG_WIDTH_Msk		(0x3F << PWM_CMPTRG_WIDTH_Pos)
#define PWM_CMPTRG_DIR_Pos			28		// 0: when counting up, do the compare; 1: when counting down, do the compare
#define PWM_CMPTRG_DIR_Msk			(0x01 << PWM_CMPTRG_DIR_Pos)
#define PWM_CMPTRG_ATP_Pos			29
#define PWM_CMPTRG_ATP_Msk			(0x07u<< PWM_CMPTRG_ATP_Pos)

#define PWM_EVMUX_START_Pos			0
#define PWM_EVMUX_START_Msk			(0x07 << PWM_EVMUX_START_Pos)
#define PWM_EVMUX_STOP_Pos			4
#define PWM_EVMUX_STOP_Msk			(0x07 << PWM_EVMUX_STOP_Pos)
#define PWM_EVMUX_PAUSE_Pos			8
#define PWM_EVMUX_PAUSE_Msk			(0x07 << PWM_EVMUX_PAUSE_Pos)
#define PWM_EVMUX_RELOAD_Pos		12
#define PWM_EVMUX_RELOAD_Msk		(0x07 << PWM_EVMUX_RELOAD_Pos)
#define PWM_EVMUX_MASKA_Pos			16
#define PWM_EVMUX_MASKA_Msk			(0x07 << PWM_EVMUX_MASKA_Pos)
#define PWM_EVMUX_MASKB_Pos			20
#define PWM_EVMUX_MASKB_Msk			(0x07 << PWM_EVMUX_MASKB_Pos)
#define PWM_EVMUX_MASKAN_Pos		24
#define PWM_EVMUX_MASKAN_Msk		(0x07 << PWM_EVMUX_MASKAN_Pos)
#define PWM_EVMUX_MASKBN_Pos		28
#define PWM_EVMUX_MASKBN_Msk		(0x07 << PWM_EVMUX_MASKBN_Pos)

#define PWM_EVMSK_OUTA_Pos			0
#define PWM_EVMSK_OUTA_Msk			(0x01 << PWM_EVMSK_OUTA_Pos)
#define PWM_EVMSK_OUTB_Pos			1
#define PWM_EVMSK_OUTB_Msk			(0x01 << PWM_EVMSK_OUTB_Pos)
#define PWM_EVMSK_OUTAN_Pos			2
#define PWM_EVMSK_OUTAN_Msk			(0x01 << PWM_EVMSK_OUTAN_Pos)
#define PWM_EVMSK_OUTBN_Pos			3
#define PWM_EVMSK_OUTBN_Msk			(0x01 << PWM_EVMSK_OUTBN_Pos)
#define PWM_EVMSK_IMME_Pos			4		// 1 MASK take effect immediately, 0 MASK take effect when the counter overflows
#define PWM_EVMSK_IMME_Msk			(0x01 << PWM_EVMSK_IMME_Pos)
#define PWM_EVMSK_STPCLR_Pos		8		// whether the counter clears when an external event causes it to stop
#define PWM_EVMSK_STPCLR_Msk		(0x01 << PWM_EVMSK_STPCLR_Pos)

#define PWM_IE_UPOVF_Pos			0		// counting up overflow interrupt enable
#define PWM_IE_UPOVF_Msk			(0x01 << PWM_IE_UPOVF_Pos)
#define PWM_IE_DNOVF_Pos			1		// counting down overflow interrupt enable
#define PWM_IE_DNOVF_Msk			(0x01 << PWM_IE_DNOVF_Pos)
#define PWM_IE_UPCMPA_Pos			2		// when counting up, the counter match CMPA interrupt enabled
#define PWM_IE_UPCMPA_Msk			(0x01 << PWM_IE_UPCMPA_Pos)
#define PWM_IE_UPCMPB_Pos			3		// when counting up, the counter match CMPB interrupt enabled
#define PWM_IE_UPCMPB_Msk			(0x01 << PWM_IE_UPCMPB_Pos)
#define PWM_IE_DNCMPA_Pos			4		// when counting down, the counter match CMPA/CMPA2 interrupt enabled
#define PWM_IE_DNCMPA_Msk			(0x01 << PWM_IE_DNCMPA_Pos)
#define PWM_IE_DNCMPB_Pos			5		// when counting down, the counter match CMPB/CMPB2 interrupt enabled
#define PWM_IE_DNCMPB_Msk			(0x01 << PWM_IE_DNCMPB_Pos)

#define PWM_IF_UPOVF_Pos			0
#define PWM_IF_UPOVF_Msk			(0x01 << PWM_IF_UPOVF_Pos)
#define PWM_IF_DNOVF_Pos			1
#define PWM_IF_DNOVF_Msk			(0x01 << PWM_IF_DNOVF_Pos)
#define PWM_IF_UPCMPA_Pos			2
#define PWM_IF_UPCMPA_Msk			(0x01 << PWM_IF_UPCMPA_Pos)
#define PWM_IF_UPCMPB_Pos			3
#define PWM_IF_UPCMPB_Msk			(0x01 << PWM_IF_UPCMPB_Pos)
#define PWM_IF_DNCMPA_Pos			4
#define PWM_IF_DNCMPA_Msk			(0x01 << PWM_IF_DNCMPA_Pos)
#define PWM_IF_DNCMPB_Pos			5
#define PWM_IF_DNCMPB_Msk			(0x01 << PWM_IF_DNCMPB_Pos)

#define PWM_SR_STAT_Pos				0		// 0 IDLE, 1 ACTIVE, 2 PAUSE
#define PWM_SR_STAT_Msk				(0x03 << PWM_SR_STAT_Pos)
#define PWM_SR_DIR_Pos				4		// 0 counting up, 1 counting down
#define PWM_SR_DIR_Msk				(0x01 << PWM_SR_DIR_Pos)
#define PWM_SR_OUTA_Pos				5
#define PWM_SR_OUTA_Msk				(0x01 << PWM_SR_OUTA_Pos)
#define PWM_SR_OUTB_Pos				6
#define PWM_SR_OUTB_Msk				(0x01 << PWM_SR_OUTB_Pos)
#define PWM_SR_OUTAN_Pos			7
#define PWM_SR_OUTAN_Msk			(0x01 << PWM_SR_OUTAN_Pos)
#define PWM_SR_OUTBN_Pos			8
#define PWM_SR_OUTBN_Msk			(0x01 << PWM_SR_OUTBN_Pos)


typedef struct {
	__IO uint32_t START;
	
	__IO uint32_t SWBRK;					// Software Brake
    
    __IO uint32_t RESET;
	
	union {
		__IO uint32_t RELOADEN;
		
		__IO uint32_t RESTART;
	};
	
    __IO uint32_t PULSE;
	
    __IO uint32_t FILTER;					// external signal filter, 0 no filter, 1 4 PCLK cycle, 2 8 PCLK cycle, 3 16 PCLK cycle
	
    __IO uint32_t BRKPOL;					// brake signal polarity
	
    __IO uint32_t BRKIE;
    
	union {
		__IO uint32_t BRKIF;
		
		__IO uint32_t BRKSR;
	};
	
	__IO uint32_t EVSR;
} PWMG_TypeDef;


#define PWMG_START_PWM0_Pos			0
#define PWMG_START_PWM0_Msk			(0x01 << PWMG_START_PWM0_Pos)
#define PWMG_START_PWM1_Pos			1
#define PWMG_START_PWM1_Msk			(0x01 << PWMG_START_PWM1_Pos)
#define PWMG_START_PWM2_Pos			2
#define PWMG_START_PWM2_Msk			(0x01 << PWMG_START_PWM2_Pos)
#define PWMG_START_PWM3_Pos			3
#define PWMG_START_PWM3_Msk			(0x01 << PWMG_START_PWM3_Pos)
#define PWMG_START_PWM4_Pos			4
#define PWMG_START_PWM4_Msk			(0x01 << PWMG_START_PWM4_Pos)

#define PWMG_SWBRK_PWM0A_Pos		0
#define PWMG_SWBRK_PWM0A_Msk		(0x01 << PWMG_SWBRK_PWM0A_Pos)
#define PWMG_SWBRK_PWM1A_Pos		1
#define PWMG_SWBRK_PWM1A_Msk		(0x01 << PWMG_SWBRK_PWM1A_Pos)
#define PWMG_SWBRK_PWM2A_Pos		2
#define PWMG_SWBRK_PWM2A_Msk		(0x01 << PWMG_SWBRK_PWM2A_Pos)
#define PWMG_SWBRK_PWM3A_Pos		3
#define PWMG_SWBRK_PWM3A_Msk		(0x01 << PWMG_SWBRK_PWM3A_Pos)
#define PWMG_SWBRK_PWM4A_Pos		4
#define PWMG_SWBRK_PWM4A_Msk		(0x01 << PWMG_SWBRK_PWM4A_Pos)
#define PWMG_SWBRK_PWM0B_Pos		8
#define PWMG_SWBRK_PWM0B_Msk		(0x01 << PWMG_SWBRK_PWM0B_Pos)
#define PWMG_SWBRK_PWM1B_Pos		9
#define PWMG_SWBRK_PWM1B_Msk		(0x01 << PWMG_SWBRK_PWM1B_Pos)
#define PWMG_SWBRK_PWM2B_Pos		10
#define PWMG_SWBRK_PWM2B_Msk		(0x01 << PWMG_SWBRK_PWM2B_Pos)
#define PWMG_SWBRK_PWM3B_Pos		11
#define PWMG_SWBRK_PWM3B_Msk		(0x01 << PWMG_SWBRK_PWM3B_Pos)
#define PWMG_SWBRK_PWM4B_Pos		12
#define PWMG_SWBRK_PWM4B_Msk		(0x01 << PWMG_SWBRK_PWM4B_Pos)

#define PWMG_RESET_PWM0_Pos			0
#define PWMG_RESET_PWM0_Msk			(0x01 << PWMG_RESET_PWM0_Pos)
#define PWMG_RESET_PWM1_Pos			1
#define PWMG_RESET_PWM1_Msk			(0x01 << PWMG_RESET_PWM1_Pos)
#define PWMG_RESET_PWM2_Pos			2
#define PWMG_RESET_PWM2_Msk			(0x01 << PWMG_RESET_PWM2_Pos)
#define PWMG_RESET_PWM3_Pos			3
#define PWMG_RESET_PWM3_Msk			(0x01 << PWMG_RESET_PWM3_Pos)
#define PWMG_RESET_PWM4_Pos			4
#define PWMG_RESET_PWM4_Msk			(0x01 << PWMG_RESET_PWM4_Pos)

#define PWMG_RELOADEN_PWM0_Pos		0
#define PWMG_RELOADEN_PWM0_Msk		(0x01 << PWMG_RELOADEN_PWM0_Pos)
#define PWMG_RELOADEN_PWM1_Pos		1
#define PWMG_RELOADEN_PWM1_Msk		(0x01 << PWMG_RELOADEN_PWM1_Pos)
#define PWMG_RELOADEN_PWM2_Pos		2
#define PWMG_RELOADEN_PWM2_Msk		(0x01 << PWMG_RELOADEN_PWM2_Pos)
#define PWMG_RELOADEN_PWM3_Pos		3
#define PWMG_RELOADEN_PWM3_Msk		(0x01 << PWMG_RELOADEN_PWM3_Pos)
#define PWMG_RELOADEN_PWM4_Pos		4
#define PWMG_RELOADEN_PWM4_Msk		(0x01 << PWMG_RELOADEN_PWM4_Pos)

#define PWMG_RESTART_PWM0_Pos		8
#define PWMG_RESTART_PWM0_Msk		(0x01 << PWMG_RESTART_PWM0_Pos)
#define PWMG_RESTART_PWM1_Pos		9
#define PWMG_RESTART_PWM1_Msk		(0x01 << PWMG_RESTART_PWM1_Pos)
#define PWMG_RESTART_PWM2_Pos		10
#define PWMG_RESTART_PWM2_Msk		(0x01 << PWMG_RESTART_PWM2_Pos)
#define PWMG_RESTART_PWM3_Pos		11
#define PWMG_RESTART_PWM3_Msk		(0x01 << PWMG_RESTART_PWM3_Pos)
#define PWMG_RESTART_PWM4_Pos		12
#define PWMG_RESTART_PWM4_Msk		(0x01 << PWMG_RESTART_PWM4_Pos)

#define PWMG_PULSE_EDGE0_Pos		0		// PWM_PULSE0 counting edge, 0 rising edge, 1 falling edge
#define PWMG_PULSE_EDGE0_Msk		(0x01 << PWMG_PULSE_EDGE0_Pos)
#define PWMG_PULSE_EDGE1_Pos		1
#define PWMG_PULSE_EDGE1_Msk		(0x01 << PWMG_PULSE_EDGE1_Pos)

#define PWMG_BRKPOL_BRK0_Pos		0		// PWMG_BRK0 brake signal polarity, 0 low level brake, 1 high level brake
#define PWMG_BRKPOL_BRK0_Msk		(0x01 << PWMG_BRKPOL_BRK0_Pos)
#define PWMG_BRKPOL_BRK1_Pos		1
#define PWMG_BRKPOL_BRK1_Msk		(0x01 << PWMG_BRKPOL_BRK1_Pos)
#define PWMG_BRKPOL_BRK2_Pos		2
#define PWMG_BRKPOL_BRK2_Msk		(0x01 << PWMG_BRKPOL_BRK2_Pos)

#define PWMG_BRKIE_BRK0_Pos			0
#define PWMG_BRKIE_BRK0_Msk			(0x01 << PWMG_BRKIE_BRK0_Pos)
#define PWMG_BRKIE_BRK1_Pos			1
#define PWMG_BRKIE_BRK1_Msk			(0x01 << PWMG_BRKIE_BRK1_Pos)
#define PWMG_BRKIE_BRK2_Pos			2
#define PWMG_BRKIE_BRK2_Msk			(0x01 << PWMG_BRKIE_BRK2_Pos)

#define PWMG_BRKIF_BRK0_Pos			0
#define PWMG_BRKIF_BRK0_Msk			(0x01 << PWMG_BRKIF_BRK0_Pos)
#define PWMG_BRKIF_BRK1_Pos			1
#define PWMG_BRKIF_BRK1_Msk			(0x01 << PWMG_BRKIF_BRK1_Pos)
#define PWMG_BRKIF_BRK2_Pos			2
#define PWMG_BRKIF_BRK2_Msk			(0x01 << PWMG_BRKIF_BRK2_Pos)

#define PWMG_BRKSR_BRK0_Pos			4		// brake pin current level
#define PWMG_BRKSR_BRK0_Msk			(0x01 << PWMG_BRKSR_BRK0_Pos)
#define PWMG_BRKSR_BRK1_Pos			5
#define PWMG_BRKSR_BRK1_Msk			(0x01 << PWMG_BRKSR_BRK1_Pos)
#define PWMG_BRKSR_BRK2_Pos			6
#define PWMG_BRKSR_BRK2_Msk			(0x01 << PWMG_BRKSR_BRK2_Pos)

#define PWMG_EVSR_EV0_Pos			0		// external event signal level value
#define PWMG_EVSR_EV0_Msk			(0x01 << PWMG_EVSR_EV0_Pos)
#define PWMG_EVSR_EV1_Pos			1
#define PWMG_EVSR_EV1_Msk			(0x01 << PWMG_EVSR_EV1_Pos)
#define PWMG_EVSR_EV2_Pos			2
#define PWMG_EVSR_EV2_Msk			(0x01 << PWMG_EVSR_EV2_Pos)
#define PWMG_EVSR_EV3_Pos			3
#define PWMG_EVSR_EV3_Msk			(0x01 << PWMG_EVSR_EV3_Pos)
#define PWMG_EVSR_EV4_Pos			4
#define PWMG_EVSR_EV4_Msk			(0x01 << PWMG_EVSR_EV4_Pos)
#define PWMG_EVSR_EV5_Pos			5
#define PWMG_EVSR_EV5_Msk			(0x01 << PWMG_EVSR_EV5_Pos)
#define PWMG_EVSR_EV6_Pos			6
#define PWMG_EVSR_EV6_Msk			(0x01 << PWMG_EVSR_EV6_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__IO uint32_t POSCNT;					//[15:0] position counter
	__IO uint32_t MAXCNT;					//[15:0] maximum count value
		
		 uint32_t RESERVED[5];
	
	__IO uint32_t IE;						// Interrupt Enable
	
	__IO uint32_t IM;						// Interrupt Mask, QEI_IRQn = IM & IF
	
	__O  uint32_t IC;						// Interrupt Clear
	
	__I  uint32_t IF;						// Interrupt Flag, IF = IE & raw interrupt request
	
	__IO uint32_t IFOV;						// Interrupt Overrun
} QEI_TypeDef;


#define QEI_CR_ENA_Pos   			0
#define QEI_CR_ENA_Msk				(0x01 << QEI_CR_ENA_Pos)
#define QEI_CR_ABSWAP_Pos  			4		// 1 swap pin A and B
#define QEI_CR_ABSWAP_Msk			(0x01 << QEI_CR_ABSWAP_Pos)
#define QEI_CR_X2X4_Pos  			5		// 0 X2 counting mode, 1 X4 counting mode
#define QEI_CR_X2X4_Msk				(0x01 << QEI_CR_X2X4_Pos)
#define QEI_CR_RSTSRC_Pos  			6		// Reset Source, 0 reset when counting match, 1 reset by index signal
#define QEI_CR_RSTSRC_Msk			(0x01 << QEI_CR_RSTSRC_Pos)
#define QEI_CR_MODE_Pos  			7		// working mode, 1 QEI mode
#define QEI_CR_MODE_Msk				(0x01 << QEI_CR_MODE_Pos)
#define QEI_CR_INDEX_Pos 			9		// 0 index pin is low level, 1 index pin is high level
#define QEI_CR_INDEX_Msk			(0x01 << QEI_CR_INDEX_Pos)
#define QEI_CR_PAUSE_Pos 			10
#define QEI_CR_PAUSE_Msk			(0x01 << QEI_CR_PAUSE_Pos)

#define QEI_IE_INDEX_Pos 			0		// Index pulse detected interrupt enable
#define QEI_IE_INDEX_Msk			(0x01 << QEI_IE_INDEX_Pos)
#define QEI_IE_MATCH_Pos 			1		// POSCNT increments to equal MAXCNT, or POSCNT decrement from MAXCNT to 0 interrupt enable
#define QEI_IE_MATCH_Msk			(0x01 << QEI_IE_MATCH_Pos)
#define QEI_IE_CNTOV_Pos 			2		// Counter Overrun interrupt enable
#define QEI_IE_CNTOV_Msk			(0x01 << QEI_IE_CNTOV_Pos)
#define QEI_IE_ERROR_Pos 			3		// Counter Error interrupt enable
#define QEI_IE_ERROR_Msk			(0x01 << QEI_IE_ERROR_Pos)

#define QEI_IM_INDEX_Pos 			0
#define QEI_IM_INDEX_Msk			(0x01 << QEI_IM_INDEX_Pos)
#define QEI_IM_MATCH_Pos 			1
#define QEI_IM_MATCH_Msk			(0x01 << QEI_IM_MATCH_Pos)
#define QEI_IM_CNTOV_Pos 			2
#define QEI_IM_CNTOV_Msk			(0x01 << QEI_IM_CNTOV_Pos)
#define QEI_IM_ERROR_Pos 			3
#define QEI_IM_ERROR_Msk			(0x01 << QEI_IM_ERROR_Pos)

#define QEI_IC_INDEX_Pos 			0
#define QEI_IC_INDEX_Msk			(0x01 << QEI_IC_INDEX_Pos)
#define QEI_IC_MATCH_Pos 			1
#define QEI_IC_MATCH_Msk			(0x01 << QEI_IC_MATCH_Pos)
#define QEI_IC_CNTOV_Pos 			2
#define QEI_IC_CNTOV_Msk			(0x01 << QEI_IC_CNTOV_Pos)
#define QEI_IC_ERROR_Pos 			3
#define QEI_IC_ERROR_Msk			(0x01 << QEI_IC_ERROR_Pos)

#define QEI_IF_INDEX_Pos 			0
#define QEI_IF_INDEX_Msk			(0x01 << QEI_IF_INDEX_Pos)
#define QEI_IF_MATCH_Pos 			1
#define QEI_IF_MATCH_Msk			(0x01 << QEI_IF_MATCH_Pos)
#define QEI_IF_CNTOV_Pos 			2
#define QEI_IF_CNTOV_Msk			(0x01 << QEI_IF_CNTOV_Pos)
#define QEI_IF_ERROR_Pos 			3
#define QEI_IF_ERROR_Msk			(0x01 << QEI_IF_ERROR_Pos)

#define QEI_IFOV_INDEX_Pos 			0
#define QEI_IFOV_INDEX_Msk			(0x01 << QEI_IFOV_INDEX_Pos)
#define QEI_IFOV_MATCH_Pos 			1
#define QEI_IFOV_MATCH_Msk			(0x01 << QEI_IFOV_MATCH_Pos)
#define QEI_IFOV_CNTOV_Pos 			2
#define QEI_IFOV_CNTOV_Msk			(0x01 << QEI_IFOV_CNTOV_Pos)
#define QEI_IFOV_ERROR_Pos 			3
#define QEI_IFOV_ERROR_Msk			(0x01 << QEI_IFOV_ERROR_Pos)




typedef struct {
	__IO uint32_t EN;                       // [0] ENABLE
    
	__IO uint32_t IE;                       // Interrupt Enable
    
	__IO uint32_t IM;                       // Interrupt Mask, DMA_IRQn = IM & IF
    
	__IO uint32_t IF;                       // Interrupt Flag, IF = IE & raw interrupt request
	
	__IO uint32_t DSTSGIE;					// Only used in Scatter Gather mode
	
	__IO uint32_t DSTSGIM;					// Only used in Scatter Gather mode
	
	__IO uint32_t DSTSGIF;					// Only used in Scatter Gather mode
	
	__IO uint32_t SRCSGIE;					// Only used in Scatter Gather mode
	
	__IO uint32_t SRCSGIM;					// Only used in Scatter Gather mode
	
	__IO uint32_t SRCSGIF;					// Only used in Scatter Gather mode
	
		 uint32_t RESERVED[5];
	
	__IO uint32_t PRI;						// priority, 1 high priority, 0 low priority
	
	struct {
		__IO uint32_t CR;
		
		__IO uint32_t AM;                   // Adress Mode
		
		__IO uint32_t DST;
		
		__IO uint32_t DSTSGADDR1;			// Only used in Scatter Gather mode
		
		__IO uint32_t DSTSGADDR2;			// Only used in Scatter Gather mode
		
		__IO uint32_t DSTSGADDR3;			// Only used in Scatter Gather mode
		
		__IO uint32_t MUX;
		
		__IO uint32_t SRC;
		
		__IO uint32_t SRCSGADDR1;			// Only used in Scatter Gather mode
		
		__IO uint32_t SRCSGADDR2;			// Only used in Scatter Gather mode
		
		__IO uint32_t SRCSGADDR3;			// Only used in Scatter Gather mode
		
		__I  uint32_t DSTSR;
		
		__I  uint32_t SRCSR;
		
			 uint32_t RESERVED[3];
	} CH[4];
} DMA_TypeDef;


#define DMA_IE_CH0_Pos			    0		
#define DMA_IE_CH0_Msk			    (0x01 << DMA_IE_CH0_Pos)
#define DMA_IE_CH1_Pos			    1		
#define DMA_IE_CH1_Msk			    (0x01 << DMA_IE_CH1_Pos)
#define DMA_IE_CH2_Pos			    2		
#define DMA_IE_CH2_Msk			    (0x01 << DMA_IE_CH2_Pos)
#define DMA_IE_CH3_Pos			    3		
#define DMA_IE_CH3_Msk			    (0x01 << DMA_IE_CH3_Pos)

#define DMA_IM_CH0_Pos			    0		
#define DMA_IM_CH0_Msk			    (0x01 << DMA_IM_CH0_Pos)
#define DMA_IM_CH1_Pos			    1		
#define DMA_IM_CH1_Msk			    (0x01 << DMA_IM_CH1_Pos)
#define DMA_IM_CH2_Pos			    2		
#define DMA_IM_CH2_Msk			    (0x01 << DMA_IM_CH2_Pos)
#define DMA_IM_CH3_Pos			    3		
#define DMA_IM_CH3_Msk			    (0x01 << DMA_IM_CH3_Pos)

#define DMA_IF_CH0_Pos			    0		
#define DMA_IF_CH0_Msk			    (0x01 << DMA_IF_CH0_Pos)
#define DMA_IF_CH1_Pos			    1		
#define DMA_IF_CH1_Msk			    (0x01 << DMA_IF_CH1_Pos)
#define DMA_IF_CH2_Pos			    2		
#define DMA_IF_CH2_Msk			    (0x01 << DMA_IF_CH2_Pos)
#define DMA_IF_CH3_Pos			    3		
#define DMA_IF_CH3_Msk			    (0x01 << DMA_IF_CH3_Pos)

#define DMA_CR_LEN_Pos				0       // length, number of transmitted data
#define DMA_CR_LEN_Msk				(0xFFFFF<< DMA_CR_LEN_Pos)
#define DMA_CR_RXEN_Pos				24		// start transmission by software, direction is SRC-->DST
#define DMA_CR_RXEN_Msk				(0x01 << DMA_CR_RXEN_Pos)
#define DMA_CR_TXEN_Pos				25		// start transmission by software, direction is DST-->SRC
#define DMA_CR_TXEN_Msk				(0x01 << DMA_CR_TXEN_Pos)
#define DMA_CR_AUTORE_Pos			26      // Auto Restart enable, i.e. circle mode
#define DMA_CR_AUTORE_Msk			(0x01 << DMA_CR_AUTORE_Pos)
#define DMA_CR_STEPOP_Pos			27		// Step Operation, trigger once to transmit 1 data
#define DMA_CR_STEPOP_Msk			(0x01 << DMA_CR_STEPOP_Pos)

#define DMA_AM_DSTAM_Pos			0       // Address Mode	0 Fixed address, 1 Address increment, 2 scatter gather mode
#define DMA_AM_DSTAM_Msk			(0x03 << DMA_AM_DSTAM_Pos)
#define DMA_AM_DSTBIT_Pos			2		// Transmission bit width, 0 byte, 1 half-word, 2 word
#define DMA_AM_DSTBIT_Msk			(0x03 << DMA_AM_DSTBIT_Pos)
#define DMA_AM_DSTBURST_Pos			4		// 0 Single, 1 Burst (INC4)
#define DMA_AM_DSTBURST_Msk			(0x01 << DMA_AM_DSTBURST_Pos)
#define DMA_AM_SRCAM_Pos			8
#define DMA_AM_SRCAM_Msk			(0x03 << DMA_AM_SRCAM_Pos)
#define DMA_AM_SRCBIT_Pos			10
#define DMA_AM_SRCBIT_Msk			(0x03 << DMA_AM_SRCBIT_Pos)
#define DMA_AM_SRCBURST_Pos			12
#define DMA_AM_SRCBURST_Msk			(0x01 << DMA_AM_SRCBURST_Pos)

#define DMA_MUX_DSTHSSIG_Pos		0		// Target side handshake signal
#define DMA_MUX_DSTHSSIG_Msk		(0x03 << DMA_MUX_DSTHSSIG_Pos)
#define DMA_MUX_DSTHSEN_Pos			2		// Target side handshake enable
#define DMA_MUX_DSTHSEN_Msk			(0x01 << DMA_MUX_DSTHSEN_Pos)
#define DMA_MUX_SRCHSSIG_Pos		8		// Source side handshake signal
#define DMA_MUX_SRCHSSIG_Msk		(0x03 << DMA_MUX_SRCHSSIG_Pos)
#define DMA_MUX_SRCHSEN_Pos			10		// Source side handshake enable
#define DMA_MUX_SRCHSEN_Msk			(0x01 << DMA_MUX_SRCHSEN_Pos)
#define DMA_MUX_EXTHSSIG_Pos		16		// External handshake signal, 0 TIMR0, 1 TIMR1, 2 TIMR2, 3 TIMR3, 4 TIMR4, 5 DMA_TRIG0, 6 DMA_TRIG1
#define DMA_MUX_EXTHSSIG_Msk		(0x07 << DMA_MUX_EXTHSSIG_Pos)
#define DMA_MUX_EXTHSEN_Pos			19		// External handshake enable
#define DMA_MUX_EXTHSEN_Msk			(0x01 << DMA_MUX_EXTHSEN_Pos)

#define DMA_DSTSR_LEN_Pos			0		// Residual number of transmitted data
#define DMA_DSTSR_LEN_Msk			(0xFFFFF<<DMA_DSTSR_LEN_Pos)
#define DMA_DSTSR_ERR_Pos			31		// Length configuration error
#define DMA_DSTSR_ERR_Msk			(0x01u<< DMA_DSTSR_ERR_Pos)

#define DMA_SRCSR_LEN_Pos			0
#define DMA_SRCSR_LEN_Msk			(0xFFFFF<<DMA_SRCSR_LEN_Pos)
#define DMA_SRCSR_ERR_Pos			31
#define DMA_SRCSR_ERR_Msk			(0x01u<< DMA_SRCSR_ERR_Pos)




typedef struct {
	__IO uint32_t CR;						// Control Register
	
	__O  uint32_t CMD;						// Command Register
	
	__I  uint32_t SR;						// Status Register
	
	__IO uint32_t IF;						// Interrupt Flag, clear by read
	
	__IO uint32_t IE;						// Interrupt Enable
	
	__IO uint32_t BT2;
	
	__IO uint32_t BT0;						// Bit Time Register 0
	
	__IO uint32_t BT1;						// Bit Time Register 1
	
	     uint32_t RESERVED;
	
	__IO uint32_t AFM;						// Acceptance Filter Mode
	
	__IO uint32_t AFE;						// Acceptance Filter Enable
	
	__I  uint32_t ALC;						// Arbitration Lost Capture
	
	__I  uint32_t ECC;						// Error code capture
	
	__IO uint32_t EWLIM;					// Error Warning Limit
	
	__IO uint32_t RXERR;					// RX error count
	
	__IO uint32_t TXERR;					// TX error count
	
	struct {
		__IO uint32_t INFO;
	
		__IO uint32_t DATA[12];
	} FRAME;
	
	__I  uint32_t RMCNT;					// Receive Message Count
	
		 uint32_t RESERVED2[162];
	
	__IO uint32_t ACR[16];					// Acceptance Check Register
	
		 uint32_t RESERVED3[16];
	
	__IO uint32_t AMR[16];					// Acceptance Mask Register
} CAN_TypeDef;


#define CAN_CR_RST_Pos				0
#define CAN_CR_RST_Msk				(0x01 << CAN_CR_RST_Pos)
#define CAN_CR_LOM_Pos				1		// Listen Only Mode
#define CAN_CR_LOM_Msk				(0x01 << CAN_CR_LOM_Pos)
#define CAN_CR_STM_Pos				2		// Self Test Mode, In this mode, the CAN controller can successfully send even if there is no response
#define CAN_CR_STM_Msk				(0x01 << CAN_CR_STM_Pos)
#define CAN_CR_SLEEP_Pos			4		// Write 1 into sleep mode, wake up when there is bus activity or interruption and automatically clear this bit
#define CAN_CR_SLEEP_Msk			(0x01 << CAN_CR_SLEEP_Pos)

#define CAN_CMD_TXREQ_Pos			0		// Transmission Request
#define CAN_CMD_TXREQ_Msk			(0x01 << CAN_CMD_TXREQ_Pos)
#define CAN_CMD_ABTTX_Pos			1		// Abort Transmission
#define CAN_CMD_ABTTX_Msk			(0x01 << CAN_CMD_ABTTX_Pos)
#define CAN_CMD_RRB_Pos				2		// Release Receive Buffer
#define CAN_CMD_RRB_Msk				(0x01 << CAN_CMD_RRB_Pos)
#define CAN_CMD_CLROV_Pos			3		// Clear Data Overrun
#define CAN_CMD_CLROV_Msk			(0x01 << CAN_CMD_CLROV_Pos)
#define CAN_CMD_SRR_Pos				4		// Self Reception Request
#define CAN_CMD_SRR_Msk				(0x01 << CAN_CMD_SRR_Pos)

#define CAN_SR_RXDA_Pos				0		// Receive Data Available
#define CAN_SR_RXDA_Msk				(0x01 << CAN_SR_RXDA_Pos)
#define CAN_SR_RXOV_Pos				1		// Receive FIFO Overrun
#define CAN_SR_RXOV_Msk				(0x01 << CAN_SR_RXOV_Pos)
#define CAN_SR_TXBR_Pos				2		// Transmit Buffer Release
#define CAN_SR_TXBR_Msk				(0x01 << CAN_SR_TXBR_Pos)
#define CAN_SR_TXOK_Pos				3		// Transmit OK, successfully completed
#define CAN_SR_TXOK_Msk				(0x01 << CAN_SR_TXOK_Pos)
#define CAN_SR_RXBUSY_Pos			4		// Receive Busy
#define CAN_SR_RXBUSY_Msk			(0x01 << CAN_SR_RXBUSY_Pos)
#define CAN_SR_TXBUSY_Pos			5		// Transmit Busy
#define CAN_SR_TXBUSY_Msk			(0x01 << CAN_SR_TXBUSY_Pos)
#define CAN_SR_ERRWARN_Pos			6		// RXERR/TXERR > Warning Limit
#define CAN_SR_ERRWARN_Msk			(0x01 << CAN_SR_ERRWARN_Pos)
#define CAN_SR_BUSOFF_Pos			7
#define CAN_SR_BUSOFF_Msk			(0x01 << CAN_SR_BUSOFF_Pos)

#define CAN_IF_RXDA_Pos				0		// IF.RXDA = SR.RXDA & IE.RXDA
#define CAN_IF_RXDA_Msk				(0x01 << CAN_IF_RXDA_Pos)
#define CAN_IF_TXBR_Pos				1		// IF.TXBR = SR.TXBR & IE.TXBR
#define CAN_IF_TXBR_Msk				(0x01 << CAN_IF_TXBR_Pos)
#define CAN_IF_ERRWARN_Pos			2		// IF.ERRWARN = (SR.ERRWARN | SR.BUSOFF 0-to-1/1-to-0) & IE.ERRWARN
#define CAN_IF_ERRWARN_Msk			(0x01 << CAN_IF_ERRWARN_Pos)
#define CAN_IF_RXOV_Pos				3		// IF.RXOV = SR.RXOV & IE.RXOV
#define CAN_IF_RXOV_Msk				(0x01 << CAN_IF_RXOV_Pos)
#define CAN_IF_WKUP_Pos				4		// IF.WKUP = IE.WKUP & CAN controller in sleep mode detects bus activity
#define CAN_IF_WKUP_Msk				(0x01 << CAN_IF_WKUP_Pos)
#define CAN_IF_ERRPASS_Pos			5		// IF.ERRPASS = IE.ERRPASS & ( RXERR/TXERR > 127)
#define CAN_IF_ERRPASS_Msk			(0x01 << CAN_IF_ERRPASS_Pos)
#define CAN_IF_ARBLOST_Pos			6
#define CAN_IF_ARBLOST_Msk			(0x01 << CAN_IF_ARBLOST_Pos)
#define CAN_IF_BUSERR_Pos			7
#define CAN_IF_BUSERR_Msk			(0x01 << CAN_IF_BUSERR_Pos)

#define CAN_IE_RXDA_Pos				0
#define CAN_IE_RXDA_Msk				(0x01 << CAN_IE_RXDA_Pos)
#define CAN_IE_TXBR_Pos				1
#define CAN_IE_TXBR_Msk				(0x01 << CAN_IE_TXBR_Pos)
#define CAN_IE_ERRWARN_Pos			2
#define CAN_IE_ERRWARN_Msk			(0x01 << CAN_IE_ERRWARN_Pos)
#define CAN_IE_RXOV_Pos				3
#define CAN_IE_RXOV_Msk				(0x01 << CAN_IE_RXOV_Pos)
#define CAN_IE_WKUP_Pos				4
#define CAN_IE_WKUP_Msk				(0x01 << CAN_IE_WKUP_Pos)
#define CAN_IE_ERRPASS_Pos			5
#define CAN_IE_ERRPASS_Msk			(0x01 << CAN_IE_ERRPASS_Pos)
#define CAN_IE_ARBLOST_Pos			6
#define CAN_IE_ARBLOST_Msk			(0x01 << CAN_IE_ARBLOST_Pos)
#define CAN_IE_BUSERR_Pos			7
#define CAN_IE_BUSERR_Msk			(0x01 << CAN_IE_BUSERR_Pos)

#define CAN_BT2_BRP_Pos				0
#define CAN_BT2_BRP_Msk				(0x0F << CAN_BT2_BRP_Pos)

#define CAN_BT0_BRP_Pos				0		// Baud Rate Prescaler, CAN_TQ = 2*Tsysclk*((BT2.BRP<<6) + BT0.BRP + 1)
#define CAN_BT0_BRP_Msk				(0x3F << CAN_BT0_BRP_Pos)
#define CAN_BT0_SJW_Pos				6		// Synchronization Jump Width
#define CAN_BT0_SJW_Msk				(0x03 << CAN_BT0_SJW_Pos)

#define CAN_BT1_TSEG1_Pos			0		// t_tseg1 = CAN_TQ * (TSEG1+1)
#define CAN_BT1_TSEG1_Msk			(0x0F << CAN_BT1_TSEG1_Pos)
#define CAN_BT1_TSEG2_Pos			4		// t_tseg2 = CAN_TQ * (TSEG2+1)
#define CAN_BT1_TSEG2_Msk			(0x07 << CAN_BT1_TSEG2_Pos)
#define CAN_BT1_SAM_Pos				7		// sample times, 0: sampled once, 1: sampled three times
#define CAN_BT1_SAM_Msk				(0x01 << CAN_BT1_SAM_Pos)

#define CAN_ECC_SEGCODE_Pos			0		// Segment Code
#define CAN_ECC_SEGCODE_Msk			(0x1F << CAN_ECC_SEGCODE_Pos)
#define CAN_ECC_DIR_Pos				5		// 0 error occurred during transmission, 1 during reception
#define CAN_ECC_DIR_Msk				(0x01 << CAN_ECC_DIR_Pos)
#define CAN_ECC_ERRCODE_Pos			6		// Error Code: 0 Bit error, 1 Form error, 2 Stuff error, 3 other error
#define CAN_ECC_ERRCODE_Msk			(0x03 << CAN_ECC_ERRCODE_Pos)

#define CAN_INFO_DLC_Pos			0		// Data Length Control
#define CAN_INFO_DLC_Msk			(0x0F << CAN_INFO_DLC_Pos)
#define CAN_INFO_RTR_Pos			6		// 1 remote frame, 0 data frame
#define CAN_INFO_RTR_Msk			(0x01 << CAN_INFO_RTR_Pos)
#define CAN_INFO_FF_Pos				7		// Frame Format, 0 standard frame, 1 extended frame
#define CAN_INFO_FF_Msk				(0x01 << CAN_INFO_FF_Pos)




typedef struct {
    __IO uint32_t DMA_MEM_ADDR;
    
    __IO uint32_t BLK;                      // Block Size and Count
    
    __IO uint32_t ARG;                      // Argument
    
    __IO uint32_t CMD;                      // Command
    
    __IO uint32_t RESP[4];                  // Response
    
    __IO uint32_t DATA;
    
    __IO uint32_t STAT;
    
    __IO uint32_t CR1;
    
    __IO uint32_t CR2;
    
    __IO uint32_t IF;
    
    __IO uint32_t IM;						// Interrupt Mask (Interrupt Flag Enable)
    
    __IO uint32_t IE;						// Interrupt Enalbe
    
    __IO uint32_t CMD12ERR;					// Auto CMD12 error status
} SDIO_TypeDef;


#define SDIO_BLK_SIZE_Pos			0		// block size, 0x200 512 bytes, 0x400 1024 bytes
#define SDIO_BLK_SIZE_Msk			(0xFFF << SDIO_BLK_SIZE_Pos)
#define SDIO_BLK_COUNT_Pos			16		// block count, 0 Stop Transfer, 1 1 block, 2 2 block, ...
#define SDIO_BLK_COUNT_Msk			(0xFFF << SDIO_BLK_COUNT_Pos)

#define SDIO_CMD_DMAEN_Pos			0
#define SDIO_CMD_DMAEN_Msk			(0x01 << SDIO_CMD_DMAEN_Pos)
#define SDIO_CMD_BLKCNTEN_Pos		1
#define SDIO_CMD_BLKCNTEN_Msk		(0x01 << SDIO_CMD_BLKCNTEN_Pos)
#define SDIO_CMD_AUTOCMD12_Pos		2
#define SDIO_CMD_AUTOCMD12_Msk		(0x01 << SDIO_CMD_AUTOCMD12_Pos)
#define SDIO_CMD_DIRREAD_Pos		4       // 0 Write, Host to Card, 1 Read, Card to Host
#define SDIO_CMD_DIRREAD_Msk		(0x01 << SDIO_CMD_DIRREAD_Pos)
#define SDIO_CMD_MULTBLK_Pos		5       // 0 Single Block, 1 Multiple Block
#define SDIO_CMD_MULTBLK_Msk		(0x01 << SDIO_CMD_MULTBLK_Pos)
#define SDIO_CMD_RESPTYPE_Pos		16		// response type, 0 no response, 1 136-bit response, 2 48-bit response, 3 48-bit response, Busy after response
#define SDIO_CMD_RESPTYPE_Msk		(0x03 << SDIO_CMD_RESPTYPE_Pos)
#define SDIO_CMD_CRCCHECK_Pos		19		// Command CRC Check Enable
#define SDIO_CMD_CRCCHECK_Msk		(0x01 << SDIO_CMD_CRCCHECK_Pos)
#define SDIO_CMD_IDXCHECK_Pos		20		// Command Index Check Enable
#define SDIO_CMD_IDXCHECK_Msk		(0x01 << SDIO_CMD_IDXCHECK_Pos)
#define SDIO_CMD_HAVEDATA_Pos		21		// 0 No Data Present, 1 Data Present
#define SDIO_CMD_HAVEDATA_Msk		(0x01 << SDIO_CMD_HAVEDATA_Pos)
#define SDIO_CMD_CMDTYPE_Pos		22		// 0 NORMAL, 1 SUSPEND, 2 RESUME, 3 ABORT
#define SDIO_CMD_CMDTYPE_Msk		(0x03 << SDIO_CMD_CMDTYPE_Pos)
#define SDIO_CMD_CMDINDX_Pos		24		// Command Index, CMD0-63, ACMD0-63
#define SDIO_CMD_CMDINDX_Msk		(0x3F << SDIO_CMD_CMDINDX_Pos)

#define SDIO_CR1_4BIT_Pos			1		// 1 4 bit mode, 0 1 bit mode
#define SDIO_CR1_4BIT_Msk			(0x01 << SDIO_CR1_4BIT_Pos)
#define SDIO_CR1_8BIT_Pos			5		// 1 8 bit mode is selected, 0 8 bit mode is not selected
#define SDIO_CR1_8BIT_Msk			(0x01 << SDIO_CR1_8BIT_Pos)
#define SDIO_CR1_CDBIT_Pos			6		// 0 No Card, 1 Card Inserted
#define SDIO_CR1_CDBIT_Msk			(0x01 << SDIO_CR1_CDBIT_Pos)
#define SDIO_CR1_CDSRC_Pos			7		// Card Detect Source, 1 CR1.CDBIT bit, 0 SD_Detect pin
#define SDIO_CR1_CDSRC_Msk			(0x01 << SDIO_CR1_CDSRC_Pos)
#define SDIO_CR1_PWRON_Pos			8		// 1 Power on, 0 Power off
#define SDIO_CR1_PWRON_Msk			(0x01 << SDIO_CR1_PWRON_Pos)
#define SDIO_CR1_VOLT_Pos			9		// 7 3.3V, 6 3.0V, 5 1.8V
#define SDIO_CR1_VOLT_Msk			(0x07 << SDIO_CR1_VOLT_Pos)

#define SDIO_CR2_CLKEN_Pos			0		// Internal Clock Enable
#define SDIO_CR2_CLKEN_Msk			(0x01 << SDIO_CR2_CLKEN_Pos)
#define SDIO_CR2_CLKRDY_Pos			1		// Internal Clock Stable/Ready
#define SDIO_CR2_CLKRDY_Msk			(0x01 << SDIO_CR2_CLKRDY_Pos)
#define SDIO_CR2_SDCLKEN_Pos		2		// SDCLK Enable
#define SDIO_CR2_SDCLKEN_Msk		(0x01 << SDIO_CR2_SDCLKEN_Pos)
#define SDIO_CR2_SDCLKDIV_Pos		8		// SDCLK Frequency Div, 0x00 div1, 0x01 div2, 0x02 div4, 0x04 div8, 0x08, div16, ..., 0x80 div256
#define SDIO_CR2_SDCLKDIV_Msk		(0xFF << SDIO_CR2_SDCLKDIV_Pos)
#define SDIO_CR2_TIMEOUT_Pos		16		// 0 TMCLK*2^13, 1 TMCLK*2^14, ..., 14 TMCLK*2^27
#define SDIO_CR2_TIMEOUT_Msk		(0x0F << SDIO_CR2_TIMEOUT_Pos)
#define SDIO_CR2_RSTALL_Pos			24		// Software Reset for All
#define SDIO_CR2_RSTALL_Msk			(0x01 << SDIO_CR2_RSTALL_Pos)
#define SDIO_CR2_RSTCMD_Pos			25		// Software Reset for CMD Line
#define SDIO_CR2_RSTCMD_Msk			(0x01 << SDIO_CR2_RSTCMD_Pos)
#define SDIO_CR2_RSTDAT_Pos			26		// Software Reset for DAT Line
#define SDIO_CR2_RSTDAT_Msk			(0x01 << SDIO_CR2_RSTDAT_Pos)

#define SDIO_IF_CMDDONE_Pos			0
#define SDIO_IF_CMDDONE_Msk			(0x01 << SDIO_IF_CMDDONE_Pos)
#define SDIO_IF_TRXDONE_Pos			1
#define SDIO_IF_TRXDONE_Msk			(0x01 << SDIO_IF_TRXDONE_Pos)
#define SDIO_IF_BLKGAP_Pos			2
#define SDIO_IF_BLKGAP_Msk			(0x01 << SDIO_IF_BLKGAP_Pos)
#define SDIO_IF_DMADONE_Pos			3
#define SDIO_IF_DMADONE_Msk			(0x01 << SDIO_IF_DMADONE_Pos)
#define SDIO_IF_BUFWRRDY_Pos		4
#define SDIO_IF_BUFWRRDY_Msk		(0x01 << SDIO_IF_BUFWRRDY_Pos)
#define SDIO_IF_BUFRDRDY_Pos		5
#define SDIO_IF_BUFRDRDY_Msk		(0x01 << SDIO_IF_BUFRDRDY_Pos)
#define SDIO_IF_CARDINSR_Pos		6
#define SDIO_IF_CARDINSR_Msk		(0x01 << SDIO_IF_CARDINSR_Pos)
#define SDIO_IF_CARDRMOV_Pos		7
#define SDIO_IF_CARDRMOV_Msk		(0x01 << SDIO_IF_CARDRMOV_Pos)
#define SDIO_IF_CARD_Pos			8
#define SDIO_IF_CARD_Msk			(0x01 << SDIO_IF_CARD_Pos)
#define SDIO_IF_ERROR_Pos			15
#define SDIO_IF_ERROR_Msk			(0x01 << SDIO_IF_ERROR_Pos)
#define SDIO_IF_CMDTIMEOUT_Pos		16
#define SDIO_IF_CMDTIMEOUT_Msk		(0x01 << SDIO_IF_CMDTIMEOUT_Pos)
#define SDIO_IF_CMDCRCERR_Pos		17
#define SDIO_IF_CMDCRCERR_Msk		(0x01 << SDIO_IF_CMDCRCERR_Pos)
#define SDIO_IF_CMDENDERR_Pos		18
#define SDIO_IF_CMDENDERR_Msk		(0x01 << SDIO_IF_CMDENDCERR_Pos)
#define SDIO_IF_CMDIDXERR_Pos		19
#define SDIO_IF_CMDIDXERR_Msk		(0x01 << SDIO_IF_CMDIDXCERR_Pos)
#define SDIO_IF_DATTIMEOUT_Pos		20
#define SDIO_IF_DATTIMEOUT_Msk		(0x01 << SDIO_IF_DATTIMEOUT_Pos)
#define SDIO_IF_DATCRCERR_Pos		21
#define SDIO_IF_DATCRCERR_Msk		(0x01 << SDIO_IF_DATCRCERR_Pos)
#define SDIO_IF_DATENDERR_Pos		22
#define SDIO_IF_DATENDERR_Msk		(0x01 << SDIO_IF_DATENDCERR_Pos)
#define SDIO_IF_CURLIMERR_Pos		23
#define SDIO_IF_CURLIMERR_Msk		(0x01 << SDIO_IF_CURLIMERR_Pos)
#define SDIO_IF_CMD12ERR_Pos		24
#define SDIO_IF_CMD12ERR_Msk		(0x01 << SDIO_IF_CMD12ERR_Pos)
#define SDIO_IF_DMAERR_Pos			25
#define SDIO_IF_DMAERR_Msk			(0x01 << SDIO_IF_DMAERR_Pos)
#define SDIO_IF_RESPERR_Pos			28
#define SDIO_IF_RESPERR_Msk			(0x01 << SDIO_IF_RESPERR_Pos)

#define SDIO_IE_CMDDONE_Pos			0		// Command Complete Status Enable
#define SDIO_IE_CMDDONE_Msk			(0x01 << SDIO_IE_CMDDONE_Pos)
#define SDIO_IE_TRXDONE_Pos			1		// Transfer Complete Status Enable
#define SDIO_IE_TRXDONE_Msk			(0x01 << SDIO_IE_TRXDONE_Pos)
#define SDIO_IE_BLKGAP_Pos			2		// Block Gap Event Status Enable
#define SDIO_IE_BLKGAP_Msk			(0x01 << SDIO_IE_BLKGAP_Pos)
#define SDIO_IE_DMADONE_Pos			3		// DMA Interrupt Status Enable
#define SDIO_IE_DMADONE_Msk			(0x01 << SDIO_IE_DMADONE_Pos)
#define SDIO_IE_BUFWRRDY_Pos		4		// Buffer Write Ready Status Enable
#define SDIO_IE_BUFWRRDY_Msk		(0x01 << SDIO_IE_BUFWRRDY_Pos)
#define SDIO_IE_BUFRDRDY_Pos		5		// Buffer Read Ready Status Enable
#define SDIO_IE_BUFRDRDY_Msk		(0x01 << SDIO_IE_BUFRDRDY_Pos)
#define SDIO_IE_CARDINSR_Pos		6		// Card Insertion Status Enable
#define SDIO_IE_CARDINSR_Msk		(0x01 << SDIO_IE_CARDINSR_Pos)
#define SDIO_IE_CARDRMOV_Pos		7		// Card Removal Status Enable
#define SDIO_IE_CARDRMOV_Msk		(0x01 << SDIO_IE_CARDRMOV_Pos)
#define SDIO_IE_CARD_Pos			8
#define SDIO_IE_CARD_Msk			(0x01 << SDIO_IE_CARD_Pos)
#define SDIO_IE_CMDTIMEOUT_Pos		16		// Command Timeout Error Status Enable
#define SDIO_IE_CMDTIMEOUT_Msk		(0x01 << SDIO_IE_CMDTIMEOUT_Pos)
#define SDIO_IE_CMDCRCERR_Pos		17		// Command CRC Error Status Enable
#define SDIO_IE_CMDCRCERR_Msk		(0x01 << SDIO_IE_CMDCRCERR_Pos)
#define SDIO_IE_CMDENDERR_Pos		18		// Command End Bit Error Status Enable
#define SDIO_IE_CMDENDERR_Msk		(0x01 << SDIO_IE_CMDENDCERR_Pos)
#define SDIO_IE_CMDIDXERR_Pos		19		// Command Index Error Status Enable
#define SDIO_IE_CMDIDXERR_Msk		(0x01 << SDIO_IE_CMDIDXCERR_Pos)
#define SDIO_IE_DATTIMEOUT_Pos		20		// Data Timeout Error Status Enable
#define SDIO_IE_DATTIMEOUT_Msk		(0x01 << SDIO_IE_DATTIMEOUT_Pos)
#define SDIO_IE_DATCRCERR_Pos		21		// Data CRC Error Status Enable
#define SDIO_IE_DATCRCERR_Msk		(0x01 << SDIO_IE_DATCRCERR_Pos)
#define SDIO_IE_DATENDERR_Pos		22		// Data End Bit Error Status Enable
#define SDIO_IE_DATENDERR_Msk		(0x01 << SDIO_IE_DATENDCERR_Pos)
#define SDIO_IE_CURLIMERR_Pos		23		// Current Limit Error Status Enable
#define SDIO_IE_CURLIMERR_Msk		(0x01 << SDIO_IE_CURLIMERR_Pos)
#define SDIO_IE_CMD12ERR_Pos		24		// Auto CMD12 Error Status Enable
#define SDIO_IE_CMD12ERR_Msk		(0x01 << SDIO_IE_CMD12ERR_Pos)
#define SDIO_IE_DMAERR_Pos			25		// ADMA Error Status Enable
#define SDIO_IE_DMAERR_Msk			(0x01 << SDIO_IE_DMAERR_Pos)
#define SDIO_IE_RESPERR_Pos			28		// Target Response Error Status Enable
#define SDIO_IE_RESPERR_Msk			(0x01 << SDIO_IE_RESPERR_Pos)

#define SDIO_IM_CMDDONE_Pos			0
#define SDIO_IM_CMDDONE_Msk			(0x01 << SDIO_IM_CMDDONE_Pos)
#define SDIO_IM_TRXDONE_Pos			1
#define SDIO_IM_TRXDONE_Msk			(0x01 << SDIO_IM_TRXDONE_Pos)
#define SDIO_IM_BLKGAP_Pos			2
#define SDIO_IM_BLKGAP_Msk			(0x01 << SDIO_IM_BLKGAP_Pos)
#define SDIO_IM_DMADONE_Pos			3
#define SDIO_IM_DMADONE_Msk			(0x01 << SDIO_IM_DMADONE_Pos)
#define SDIO_IM_BUFWRRDY_Pos		4
#define SDIO_IM_BUFWRRDY_Msk		(0x01 << SDIO_IM_BUFWRRDY_Pos)
#define SDIO_IM_BUFRDRDY_Pos		5
#define SDIO_IM_BUFRDRDY_Msk		(0x01 << SDIO_IM_BUFRDRDY_Pos)
#define SDIO_IM_CARDINSR_Pos		6
#define SDIO_IM_CARDINSR_Msk		(0x01 << SDIO_IM_CARDINSR_Pos)
#define SDIO_IM_CARDRMOV_Pos		7
#define SDIO_IM_CARDRMOV_Msk		(0x01 << SDIO_IM_CARDRMOV_Pos)
#define SDIO_IM_CARD_Pos			8
#define SDIO_IM_CARD_Msk			(0x01 << SDIO_IM_CARD_Pos)
#define SDIO_IM_CMDTIMEOUT_Pos		16
#define SDIO_IM_CMDTIMEOUT_Msk		(0x01 << SDIO_IM_CMDTIMEOUT_Pos)
#define SDIO_IM_CMDCRCERR_Pos		17
#define SDIO_IM_CMDCRCERR_Msk		(0x01 << SDIO_IM_CMDCRCERR_Pos)
#define SDIO_IM_CMDENDERR_Pos		18
#define SDIO_IM_CMDENDERR_Msk		(0x01 << SDIO_IM_CMDENDCERR_Pos)
#define SDIO_IM_CMDIDXERR_Pos		19
#define SDIO_IM_CMDIDXERR_Msk		(0x01 << SDIO_IM_CMDIDXCERR_Pos)
#define SDIO_IM_DATTIMEOUT_Pos		20
#define SDIO_IM_DATTIMEOUT_Msk		(0x01 << SDIO_IM_DATTIMEOUT_Pos)
#define SDIO_IM_DATCRCERR_Pos		21
#define SDIO_IM_DATCRCERR_Msk		(0x01 << SDIO_IM_DATCRCERR_Pos)
#define SDIO_IM_DATENDERR_Pos		22
#define SDIO_IM_DATENDERR_Msk		(0x01 << SDIO_IM_DATENDCERR_Pos)
#define SDIO_IM_CURLIMERR_Pos		23
#define SDIO_IM_CURLIMERR_Msk		(0x01 << SDIO_IM_CURLIMERR_Pos)
#define SDIO_IM_CMD12ERR_Pos		24
#define SDIO_IM_CMD12ERR_Msk		(0x01 << SDIO_IM_CMD12ERR_Pos)
#define SDIO_IM_DMAERR_Pos			25
#define SDIO_IM_DMAERR_Msk			(0x01 << SDIO_IM_DMAERR_Pos)
#define SDIO_IM_RESPERR_Pos			28
#define SDIO_IM_RESPERR_Msk			(0x01 << SDIO_IM_RESPERR_Pos)

#define SDIO_CMD12ERR_NE_Pos		0		// Auto CMD12 not Executed
#define SDIO_CMD12ERR_NE_Msk		(0x01 << SDIO_CMD12ERR_NE_Pos)
#define SDIO_CMD12ERR_TO_Pos		1		// Auto CMD12 Timeout Error
#define SDIO_CMD12ERR_TO_Msk		(0x01 << SDIO_CMD12ERR_TO_Pos)
#define SDIO_CMD12ERR_CRC_Pos		2		// Auto CMD12 CRC Error
#define SDIO_CMD12ERR_CRC_Msk		(0x01 << SDIO_CMD12ERR_CRC_Pos)
#define SDIO_CMD12ERR_END_Pos		3		// Auto CMD12 End Bit Error
#define SDIO_CMD12ERR_END_Msk		(0x01 << SDIO_CMD12ERR_END_Pos)
#define SDIO_CMD12ERR_INDEX_Pos		4		// Auto CMD12 Index Error
#define SDIO_CMD12ERR_INDEX_Msk		(0x01 << SDIO_CMD12ERR_INDEX_Pos)




typedef struct {
	__IO uint32_t IF;						// [0] frame transfer done, write 1 to clear
	
	__IO uint32_t IE;

		 uint32_t RESERVED;
    
    __IO uint32_t START;
    
    __IO uint32_t RESERVED2;
    
    __IO uint32_t CR;
	    
    __IO uint32_t CRH;
		
	__IO uint32_t CRV;
	    
		 uint32_t RESERVED3;
	
	__IO uint32_t BGC;						// Background color
			
		 uint32_t RESERVED4[6];
	
	struct {
		__IO uint32_t LCR;					// Layer Control Register
		
		__IO uint32_t WHP;					// Window Horizon Position
		
		__IO uint32_t WVP;					// Window Vertical Position
		
		__IO uint32_t ADDR;					// display data Address
		
		__IO uint32_t LLEN;					// display data Line Length
		
		__IO uint32_t CK;					// Color Key
		
			 uint32_t RESERVED5[10];
	} L[2];									// Layer
	
		 uint32_t RESERVED5[16];
	
		__IO uint32_t MPUCR;
	
		__IO uint32_t MPUIR;
	
		__IO uint32_t MPUDR;
	
		__IO uint32_t MPUAR;
	
		__IO uint32_t MPULEN;
} LCD_TypeDef;


#define LCD_START_GO_Pos			1		// Write 1 Starts data transmission and automatically clears after transmission ends
#define LCD_START_GO_Msk			(0x01 << LCD_START_GO_Pos)

#define LCD_CR_CLKDIV_Pos			0      	// clock divider, 0 div2, 1 div3, ...
#define LCD_CR_CLKDIV_Msk			(0x3F << LCD_CR_CLKDIV_Pos)
#define LCD_CR_CLKINV_Pos			6      	// DOTCLK invert, used for LCD that samples at falling edge of DOTCLK
#define LCD_CR_CLKINV_Msk			(0x01 << LCD_CR_CLKINV_Pos)
#define LCD_CR_CLKALW_Pos			7		// DOTCLK Always, 0 DOTCLK keep at high level when idle
#define LCD_CR_CLKALW_Msk			(0x01 << LCD_CR_CLKALW_Pos)
#define LCD_CR_BURSTEN_Pos			8		// Burst Enable
#define LCD_CR_BURSTEN_Msk			(0x01 << LCD_CR_BURSTEN_Pos)
#define LCD_CR_BURSTLEN_Pos			9		// Burst Length, 0 Burst INCR4, 1 Burst INCR8
#define LCD_CR_BURSTLEN_Msk			(0x01 << LCD_CR_BURSTLEN_Pos)
#define LCD_CR_AUTORESTA_Pos		13		// Auto Restart
#define LCD_CR_AUTORESTA_Msk		(0x01 << LCD_CR_AUTORESTA_Pos)
#define LCD_CR_IMMRELOAD_Pos		14		// Immediate Reload, Immediately loads the value of the layer configuration register into the layer working register
#define LCD_CR_IMMRELOAD_Msk		(0x01 << LCD_CR_IMMRELOAD_Pos)
#define LCD_CR_VBPRELOAD_Pos		15		// VBP Period Relaod
#define LCD_CR_VBPRELOAD_Msk		(0x01 << LCD_CR_VBPRELOAD_Pos)
#define LCD_CR_FORMAT_Pos			16		// 0 RGB565, 1 RGB888
#define LCD_CR_FORMAT_Msk			(0x01 << LCD_CR_FORMAT_Pos)
#define LCD_CR_SEREN_Pos			17		// Serial RGB Enable
#define LCD_CR_SEREN_Msk			(0x01 << LCD_CR_SEREN_Pos)
#define LCD_CR_MPUEN_Pos			18		// MPU Interface Enable
#define LCD_CR_MPUEN_Msk			(0x01 << LCD_CR_MPUEN_Pos)
#define LCD_CR_VSYNCINV_Pos			19		// 1 VSYNC invert
#define LCD_CR_VSYNCINV_Msk			(0x01 << LCD_CR_VSYNCINV_Pos)
#define LCD_CR_HSYNCINV_Pos			20		// 1 HSYNC invert
#define LCD_CR_HSYNCINV_Msk			(0x01 << LCD_CR_HSYNCINV_Pos)

#define LCD_CRH_HSW_Pos				0		// Hsync Width, Output HSYNC low for how many DOTCLK cycles. 0 indicates 1 cycle
#define LCD_CRH_HSW_Msk				(0xFF << LCD_CRH_HSW_Pos)
#define LCD_CRH_HBP_Pos			    8		// 0 indicates one DOTCLK period
#define LCD_CRH_HBP_Msk			    (0xFF << LCD_CRH_HBP_Pos)
#define LCD_CRH_PIX_Pos				16		// number of horizontal pixels. 0 indicates 1 and the maximum value is 1023
#define LCD_CRH_PIX_Msk				(0x3FF<< LCD_CRH_PIX_Pos)
#define LCD_CRH_HFP_Pos			    26		// 0 indicates one DOTCLK period
#define LCD_CRH_HFP_Msk			    (0x3Fu<< LCD_CRH_HFP_Pos)

#define LCD_CRV_VSW_Pos				0		//Vsync Width, Output VSYNC low level for how many line cycles, 0 indicates 1 cycle
#define LCD_CRV_VSW_Msk				(0xFF << LCD_CRV_VSW_Pos)
#define LCD_CRV_VBP_Pos			    8		// 0 indicates 1 horizontal row cycle
#define LCD_CRV_VBP_Msk			    (0xFF << LCD_CRV_VBP_Pos)
#define LCD_CRV_PIX_Pos				16		// number of vertical pixels. 0 indicates 1, and the maximum value is 1023
#define LCD_CRV_PIX_Msk				(0x3FF<< LCD_CRV_PIX_Pos)
#define LCD_CRV_VFP_Pos			    26		// 0 indicates 1 horizontal row cycle
#define LCD_CRV_VFP_Msk			    (0x3Fu<< LCD_CRV_VFP_Pos)

#define LCD_LCR_ALPHA_Pos			0		// Alpha value of this layer
#define LCD_LCR_ALPHA_Msk			(0xFF << LCD_LCR_ALPHA_Pos)
#define LCD_LCR_EN_Pos				8		// Layer Enable
#define LCD_LCR_EN_Msk				(0x01 << LCD_LCR_EN_Pos)
#define LCD_LCR_CKEN_Pos			9		// Layer Color Key Enable
#define LCD_LCR_CKEN_Msk			(0x01 << LCD_LCR_CKEN_Pos)

#define LCD_WHP_STA_Pos				0		// Layer Window Horizontal start point
#define LCD_WHP_STA_Msk				(0x3FF<< LCD_WHP_STA_Pos)
#define LCD_WHP_STP_Pos				16		// Layer Window Horizontal end point
#define LCD_WHP_STP_Msk				(0x3FF<< LCD_WHP_STP_Pos)

#define LCD_WVP_STA_Pos				0		// Layer Window Vertical start point
#define LCD_WVP_STA_Msk				(0x3FF<< LCD_WVP_STA_Pos)
#define LCD_WVP_STP_Pos				16		// Layer Window Vertical end point
#define LCD_WVP_STP_Msk				(0x3FF<< LCD_WVP_STP_Pos)

#define LCD_MPUCR_RCS1_0_Pos		0		// for read, LCD_CS rising edge to falling edge delay
#define LCD_MPUCR_RCS1_0_Msk		(0x1F << LCD_MPUCR_RCS1_0_Pos)
#define LCD_MPUCR_RDHOLD_Pos		5		// LCD_RD Low level holding time
#define LCD_MPUCR_RDHOLD_Msk		(0x1F << LCD_MPUCR_RDHOLD_Pos)
#define LCD_MPUCR_WCS1_0_Pos		10		// for write, LCD_CS rising edge to falling edge delay
#define LCD_MPUCR_WCS1_0_Msk		(0x0F << LCD_MPUCR_WCS1_0_Pos)
#define LCD_MPUCR_WR1CS1_Pos		14		// LCD_WR rising edge to LCD_CS rising edge delay
#define LCD_MPUCR_WR1CS1_Msk		(0x03 << LCD_MPUCR_WR1CS1_Pos)
#define LCD_MPUCR_WRHOLD_Pos		16		// LCD_WR Low level holding time
#define LCD_MPUCR_WRHOLD_Msk		(0x0F << LCD_MPUCR_WRHOLD_Pos)
#define LCD_MPUCR_CS0WR0_Pos		20		// LCD_CS falling edge to LCD_WR falling edge delay
#define LCD_MPUCR_CS0WR0_Msk		(0x03 << LCD_MPUCR_CS0WR0_Pos)

#define LCD_MPULEN_VPIX_Pos			0		// number of lines. 0 indicates 1 line
#define LCD_MPULEN_VPIX_Msk			(0x3FF<< LCD_MPULEN_VPIX_Pos)
#define LCD_MPULEN_HPIX_Pos			16		// number of pixels per line, 0 represents 1 pixel
#define LCD_MPULEN_HPIX_Msk			(0x3FF<< LCD_MPULEN_HPIX_Pos)




typedef struct {
	__IO uint32_t IF;
	
	__IO uint32_t IE;
	
	__IO uint32_t CR;
	
		 uint32_t RESERVED;
	
	struct {
		__IO uint32_t MAR;					// Memory Address Register, word alignment
		
		__IO uint32_t OR;					// Offset Register, added at the end of each line to determine the starting address of the next line
		
		__IO uint32_t PFCCR;				// Pixel Format Converter Control Register
		
		__IO uint32_t COLOR;
	} L[3];									// Layer: 0 Foreground, 1 Background, 2 Output
	
	__IO uint32_t NLR;						// Number of Line Register
} DMA2D_TypeDef;


#define DMA2D_IF_DONE_Pos			1		// transfer done, wirte 1 to clear
#define DMA2D_IF_DONE_Msk			(0x01 << DMA2D_IF_DONE_Pos)

#define DMA2D_IE_DONE_Pos			1
#define DMA2D_IE_DONE_Msk			(0x01 << DMA2D_IE_DONE_Pos)

#define DMA2D_CR_START_Pos			0		// start transfer
#define DMA2D_CR_START_Msk			(0x01 << DMA2D_CR_START_Pos)
#define DMA2D_CR_MODE_Pos			8		// 0 memory to memory, 1 memory to memory and do PFC, 2 memory to memory and do blend, 3 register to memory
#define DMA2D_CR_MODE_Msk			(0x03 << DMA2D_CR_MODE_Pos)
#define DMA2D_CR_WAIT_Pos			22		// data block (64 word) transfer interval in unit of HCLK period
#define DMA2D_CR_WAIT_Msk			(0x3FFu<<DMA2D_CR_WAIT_Pos)

#define DMA2D_PFCCR_CFMT_Pos		0		// Color Format, 0 ARGB8888, 1 RGB8888, 2 RGB565
#define DMA2D_PFCCR_CFMT_Msk		(0x07 << DMA2D_CFMT_FORMAT_Pos)
#define DMA2D_PFCCR_AINV_Pos		3		// Alpha Invert
#define DMA2D_PFCCR_AINV_Msk		(0x01 << DMA2D_PFCCR_AINV_Pos)
#define DMA2D_PFCCR_RBSWAP_Pos		4		// RB Swap, 0 RGB, 1 BGR
#define DMA2D_PFCCR_RBSWAP_Msk		(0x01 << DMA2D_PFCCR_RBSWAP_Pos)
#define DMA2D_PFCCR_AMODE_Pos		8		// Alpha Mode, 0 use the pixel's own Alpha value, 1 use PFCCR.ALPHA, 2 use the product of the pixel's own Alpha value and PFCCR.ALPHA
#define DMA2D_PFCCR_AMODE_Msk		(0x03 << DMA2D_PFCCR_AMODE_Pos)
#define DMA2D_PFCCR_ALPHA_Pos		24
#define DMA2D_PFCCR_ALPHA_Msk		(0xFFu<< DMA2D_PFCCR_ALPHA_Pos)

#define DMA2D_NLR_NLINE_Pos			0		// Number of Line
#define DMA2D_NLR_NLINE_Msk			(0xFFFF<<DMA2D_NLR_NLINE_Pos)
#define DMA2D_NLR_NPIXEL_Pos		16		// Number of Pixel per line
#define DMA2D_NLR_NPIXEL_Msk		(0x3FFF<<DMA2D_NLR_NPIXEL_Pos)




typedef struct {
    __IO uint32_t TIM;						// SDRAM timing
    
    __IO uint32_t CFG;
    
    __IO uint32_t T64;						// 64ms refresh cycle configuration
    
    __IO uint32_t CR;
    
    __IO uint32_t SR;						// 0 Idle, 1 Init, 2 Self-refresh, 4 Operating, 5 Auto-refresh
} SDRAMC_TypeDef;


#define SDRAMC_TIM_TRCD_Pos		    0		// Row to column delay, Ie. Activate to Command delay
#define SDRAMC_TIM_TRCD_Msk		    (0x03 << SDRAMC_TIM_TRCD_Pos)
#define SDRAMC_TIM_TRC_Pos		    2		// Activate to Activate on same bank
#define SDRAMC_TIM_TRC_Msk		    (0x0F << SDRAMC_TIM_TRC_Pos)
#define SDRAMC_TIM_TRP_Pos		    6		// Row precharge time,  Ie. Precharge to Activate delay
#define SDRAMC_TIM_TRP_Msk		    (0x03 << SDRAMC_TIM_TRP_Pos)
#define SDRAMC_TIM_T100US_Pos		8
#define SDRAMC_TIM_T100US_Msk		(0x7FFF<<SDRAMC_TIM_T100US_Pos)

#define SDRAMC_CFG_CLKDIV_Pos		0		// 0 SYSCLK/1, 1 SYSCLK/2
#define SDRAMC_CFG_CLKDIV_Msk		(0x01 << SDRAMC_CFG_CLKDIV_Pos)
#define SDRAMC_CFG_CASDELAY_Pos		1		// CAS Latency, 0 2, 1 3
#define SDRAMC_CFG_CASDELAY_Msk		(0x01 << SDRAMC_CFG_CASDELAY_Pos)
#define SDRAMC_CFG_HIGHFREQ_Pos		2		// should set when SDRAM CLK > 66MHz
#define SDRAMC_CFG_HIGHFREQ_Msk		(0x01 << SDRAMC_CFG_HIGHFREQ_Pos)
#define SDRAMC_CFG_SIZE_Pos			3		// 0 8MB, 1 16MB, 2 32MB
#define SDRAMC_CFG_SIZE_Msk			(0x03 << SDRAMC_CFG_SIZE_Pos)

#define SDRAMC_CR_ENTERSRF_Pos		0		// Enter Self-refresh
#define SDRAMC_CR_ENTERSRF_Msk		(0x01 << SDRAMC_CR_ENTERSRF_Pos)
#define SDRAMC_CR_PWRON_Pos			1		// write 1 to start init process, and automatically clear when init done
#define SDRAMC_CR_PWRON_Msk			(0x01 << SDRAMC_CR_PWRON_Pos)




typedef struct {
	__IO uint32_t CMD;
	
	__IO uint32_t INPUT;
	
	__IO uint32_t COS;						// cos result
	
	__IO uint32_t SIN;						// sin result
	
	__IO uint32_t ARCTAN;					// arctan result
	
	__IO uint32_t IF;
	
	__IO uint32_t IE;
	
	__IO uint32_t TANH;						// Write starts the TANH calculation, reads the data after writing, and returns the calculation result
} CORDIC_TypeDef;


#define CORDIC_CMD_START_Pos		0		// write 1 to sart calculate, and automatically clear when done
#define CORDIC_CMD_START_Msk		(0x01 << CORDIC_CMD_START_Pos)
#define CORDIC_CMD_RANGE_Pos		1		// for arctan calculate, input range, 0 (0.05, 0.5], 1 (0.5, 2], 2 >2
#define CORDIC_CMD_RANGE_Msk		(0x03 << CORDIC_CMD_RANGE_Pos)
#define CORDIC_CMD_SINCOS_Pos		3		// 1 sin/cos calculate, 0 arctan calculate
#define CORDIC_CMD_SINCOS_Msk		(0x01 << CORDIC_CMD_SINCOS_Pos)
#define CORDIC_CMD_MULDIV_Pos		4		// 0 sin/cos/arctan calculate, 2 division, 3 multiply
#define CORDIC_CMD_MULDIV_Msk		(0x03 << CORDIC_CMD_MULDIV_Pos)

#define CORDIC_INPUT_F_Pos			0		// fractional part
#define CORDIC_INPUT_F_Msk			(0x3FFF << CORDIC_INPUT_F_Pos)
#define CORDIC_INPUT_I_Pos			14		// integer part
#define CORDIC_INPUT_I_Msk			(0x03 << CORDIC_INPUT_I_Pos)
#define CORDIC_INPUT_F2_Pos			16		// second operand for multiply, dividend for division
#define CORDIC_INPUT_F2_Msk			(0x3FFF << CORDIC_INPUT_F2_Pos)
#define CORDIC_INPUT_I2_Pos			30
#define CORDIC_INPUT_I2_Msk			(0x03u<< CORDIC_INPUT_I2_Pos)

#define CORDIC_COS_F_Pos			0		// cos result, fractional part
#define CORDIC_COS_F_Msk			(0x3FFF << CORDIC_COS_F_Pos)
#define CORDIC_COS_I_Pos			14
#define CORDIC_COS_I_Msk			(0x03 << CORDIC_COS_I_Pos)
#define CORDIC_COS_DONE_Pos			16		// cos done?
#define CORDIC_COS_DONE_Msk			(0x01 << CORDIC_COS_DONE_Pos)

#define CORDIC_SIN_F_Pos			0		// sin result
#define CORDIC_SIN_F_Msk			(0x3FFF << CORDIC_SIN_F_Pos)
#define CORDIC_SIN_I_Pos			14
#define CORDIC_SIN_I_Msk			(0x03 << CORDIC_SIN_I_Pos)
#define CORDIC_SIN_DONE_Pos			16
#define CORDIC_SIN_DONE_Msk			(0x01 << CORDIC_SIN_DONE_Pos)

#define CORDIC_ARCTAN_F_Pos			0		// arctan result
#define CORDIC_ARCTAN_F_Msk			(0x3FFF << CORDIC_ARCTAN_F_Pos)
#define CORDIC_ARCTAN_I_Pos			14
#define CORDIC_ARCTAN_I_Msk			(0x03 << CORDIC_ARCTAN_I_Pos)
#define CORDIC_ARCTAN_DONE_Pos		16
#define CORDIC_ARCTAN_DONE_Msk		(0x01 << CORDIC_ARCTAN_DONE_Pos)

#define CORDIC_IF_DONE_Pos			0		// calculate done, wirte 1 to clear
#define CORDIC_IF_DONE_Msk			(0x01 << CORDIC_IF_DONE_Pos)
#define CORDIC_IF_ERR_Pos			1		// calculate error, write 1 to clear
#define CORDIC_IF_ERR_Msk			(0x01 << CORDIC_IF_ERR_Pos)

#define CORDIC_IE_DONE_Pos			0
#define CORDIC_IE_DONE_Msk			(0x01 << CORDIC_IE_DONE_Pos)
#define CORDIC_IE_ERR_Pos			1
#define CORDIC_IE_ERR_Msk			(0x01 << CORDIC_IE_ERR_Pos)

#define CORDIC_TANH_F_Pos			0		// input and result register for tanh
#define CORDIC_TANH_F_Msk			(0x3FFF << CORDIC_TANH_F_Pos)
#define CORDIC_TANH_I_Pos			14
#define CORDIC_TANH_I_Msk			(0x03 << CORDIC_TANH_I_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__IO uint32_t SR;
	
	     uint32_t RESERVED[2];
	
	__IO uint32_t DIVIDEND;
	
	__IO uint32_t DIVISOR;
	
	__IO uint32_t QUO;						// quotient
	
	__IO uint32_t REMAIN;					// remainder
	
	__IO uint32_t RADICAND;
	
	__IO uint32_t ROOT;						// lower 16 bits are the fractional part, and higher 16 bits are the integer part
} DIV_TypeDef;


#define DIV_CR_DIVGO_Pos			0		// write 1 to start division calculate, and automatically clear when done
#define DIV_CR_DIVGO_Msk			(0x01 << DIV_CR_DIVGO_Pos)
#define DIV_CR_DIVSIGN_Pos			1		// 0 signed division, 1 unsigned division
#define DIV_CR_DIVSIGN_Msk			(0x01 << DIV_CR_DIVSIGN_Pos)
#define DIV_CR_ROOTGO_Pos			8		// write 1 to start root calculate, automatically clear when done
#define DIV_CR_ROOTGO_Msk			(0x01 << DIV_CR_ROOTGO_Pos)
#define DIV_CR_ROOTMOD_Pos			9		// root result mode: 0 only integer, 1 both integer and fractional part
#define DIV_CR_ROOTMOD_Msk			(0x01 << DIV_CR_ROOTMOD_Pos)

#define DIV_SR_DIVEND_Pos			0		// division operation complete flag, write 1 to clear
#define DIV_SR_DIVEND_Msk			(0x01 << DIV_SR_DIVEND_Pos)
#define DIV_SR_DIVBUSY_Pos			1		// division operation busy
#define DIV_SR_DIVBUSY_Msk			(0x01 << DIV_SR_DIVBUSY_Pos)
#define DIV_SR_ROOTENDI_Pos			8		// root operation integer part complete flag, write 1 to clear
#define DIV_SR_ROOTENDI_Msk			(0x01 << DIV_SR_ROOTENDI_Pos)
#define DIV_SR_ROOTENDF_Pos			9		// root operation fractional part complete flag, write 1 to clear
#define DIV_SR_ROOTENDF_Msk			(0x01 << DIV_SR_ROOTENDF_Pos)
#define DIV_SR_ROOTBUSY_Pos			10		// root operation busy
#define DIV_SR_ROOTBUSY_Msk			(0x01 << DIV_SR_ROOTBUSY_Pos)




typedef struct {
	__IO uint32_t DATA;
	
	__IO uint32_t ADDR;
	
	__IO uint32_t ERASE;
	
	__IO uint32_t CACHE;
	
	__IO uint32_t CFG0;
	
	__IO uint32_t CFG1;
	
	__IO uint32_t CFG2;
	
	__IO uint32_t CFG3;
	
	__IO uint32_t CFG4;
	
	__IO uint32_t STAT;
} FMC_TypeDef;


#define FMC_ERASE_ADDR_Pos			0		// 4K bytes per page
#define FMC_ERASE_ADDR_Msk			(0x7FFFF << FMC_ERASE_ADDR_Pos)
#define FMC_ERASE_REQ_Pos			27
#define FMC_ERASE_REQ_Msk			(0x1Fu<< FMC_ERASE_REQ_Pos)

#define FMC_CACHE_PROGEN_Pos		0		// Flash Program Enable
#define FMC_CACHE_PROGEN_Msk		(0x01 << FMC_CACHE_PROGEN_Pos)
#define FMC_CACHE_CEN_Pos			16		// Cache Enable
#define FMC_CACHE_CEN_Msk			(0x01 << FMC_CACHE_CEN_Pos)
#define FMC_CACHE_CPEN_Pos			17		// Cache Predict Enable
#define FMC_CACHE_CPEN_Msk			(0x01 << FMC_CACHE_CPEN_Pos)
#define FMC_CACHE_CCLR_Pos			18		// Cache Clear, automatically clear
#define FMC_CACHE_CCLR_Msk			(0x01 << FMC_CACHE_CCLR_Pos)

#define FMC_STAT_ERASEBUSY_Pos		0
#define FMC_STAT_ERASEBUSY_Msk		(0x01 << FMC_STAT_ERASEBUSY_Pos)
#define FMC_STAT_PROGBUSY_Pos		1
#define FMC_STAT_PROGBUSY_Msk		(0x01 << FMC_STAT_PROGBUSY_Pos)
#define FMC_STAT_READBUSY_Pos		2
#define FMC_STAT_READBUSY_Msk		(0x01 << FMC_STAT_READBUSY_Pos)
#define FMC_STAT_FIFOEMPTY_Pos		3		// Write FIFO Empty
#define FMC_STAT_FIFOEMPTY_Msk		(0x01 << FMC_STAT_FIFOEMPTY_Pos)
#define FMC_STAT_FIFOFULL_Pos		4		// Write FIFO Full
#define FMC_STAT_FIFOFULL_Msk		(0x01 << FMC_STAT_FIFOFULL_Pos)
#define FMC_STAT_IDLE_Pos			31
#define FMC_STAT_IDLE_Msk			(0x01u<< FMC_STAT_IDLE_Pos)




typedef struct {
	__IO uint32_t CFG;
	__IO uint32_t TIM;
	__I  uint32_t SR;
	__IO uint32_t IF;
	__IO uint32_t IE;
	__IO uint32_t GO;						// write 1 to start
	__IO uint32_t ADDR;
	__IO uint32_t DATA;
	__IO uint32_t CMDAHB;					// command code used to read or write the Flash through the AHB bus
	__IO uint32_t CMD;						// command code used to read or write the Flash through the register
} SFC_TypeDef;								// Serial Flash Controller


#define SFC_CFG_CMDTYPE_Pos			0		//0 no address, no data, no WIP, used for write enable, write disable
											//1 no address, have data (1-byte read), no WIP, used for SPI Flash status register read
											//2 no address, have data (3-byte read), no WIP, used for Identification read
											//3 have address, have data (2-byte read), no WIP, used for MID/DID read
											//4 no address, no data, have WIP
											//5 no address, no data, have WIP (automatically send write enable), used for chip erase
											//6 no address, have data (2-byte write), have WIP (automatically send write enable), used for SPI Flash status register write
											//7 have address, no data, have WIP (automatically send write enable), used for sector erase
#define SFC_CFG_CMDTYPE_Msk			(0x0F << SFC_CFG_CMDTYPE_Pos)
#define SFC_CFG_CMDWREN_Pos			5		// SFC->CMD register write enable, then you can write command opcode to CMD register
#define SFC_CFG_CMDWREN_Msk			(0x01 << SFC_CFG_CMDWREN_Pos)
#define SFC_CFG_CLKDIV_Pos			6		// clock divider, 0 div1, 1 div2, 2 div4, 3 div8
#define SFC_CFG_CLKDIV_Msk			(0x03 << SFC_CFG_CLKDIV_Pos)
#define SFC_CFG_ADDR4L_Pos			8		// use 4 data line when address phase
#define SFC_CFG_ADDR4L_Msk			(0x01 << SFC_CFG_ADDR4L_Pos)
#define SFC_CFG_DATA4L_PP_Pos		9		// use 4 data line when data phase for page program
#define SFC_CFG_DATA4L_PP_Msk		(0x01 << SFC_CFG_DATA4L_PP_Pos)
#define SFC_CFG_DATA4L_RD_Pos		10		// use 4 data line when data phase for data read, 0 1 data line, 1 2 data line, 2 4 data line
#define SFC_CFG_DATA4L_RD_Msk		(0x03 << SFC_CFG_DATA4L_RD_Pos)
#define SFC_CFG_WREN_Pos			12		// Flash write enable
#define SFC_CFG_WREN_Msk			(0x01 << SFC_CFG_WREN_Pos)

#define SFC_TIM_WIP_CHK_ITV_Pos		0		// time interval for automatical WIP query
#define SFC_TIM_WIP_CHK_ITV_Msk		(0xFF << SFC_TIM_WIP_CHK_ITV_Pos)
#define SFC_TIM_WIP_CHK_LMT_Pos		8		// count limit for automatical WIP query
#define SFC_TIM_WIP_CHK_LMT_Msk		(0xFF << SFC_TIM_WIP_CHK_LMT_Pos)
#define SFC_TIM_IDLETO_Pos			16		// Idle timeout, release CS after timeout
#define SFC_TIM_IDLETO_Msk			(0x3F << SFC_TIM_IDLETO_Pos)
#define SFC_TIM_CSHIGH_Pos			22		// time duration for CS high, 0 1 SCLK cycle, 1 2 SCLK cycle, 2 3 SCLK cycle, 3 4 SCLK cycle
#define SFC_TIM_CSHIGH_Msk			(0x03 << SFC_TIM_CSHIGH_Pos)

#define SFC_SR_BUSY_Pos				0
#define SFC_SR_BUSY_Msk				(0x01 << SFC_SR_BUSY_Pos)
#define SFC_SR_FIFOLVL_Pos			24		// FIFO Level
#define SFC_SR_FIFOLVL_Msk			(0x07 << SFC_SR_FIFOLVL_Pos)
#define SFC_SR_FIFOEMPTY_Pos		27		// FIFO Empty
#define SFC_SR_FIFOEMPTY_Msk		(0x01 << SFC_SR_FIFOEMPTY_Pos)
#define SFC_SR_FIFOHFULL_Pos		28		// FIFO Half Full
#define SFC_SR_FIFOHFULL_Msk		(0x01 << SFC_SR_FIFOHFULL_Pos)
#define SFC_SR_FIFOFULL_Pos			29		// FIFO Full
#define SFC_SR_FIFOFULL_Msk			(0x01 << SFC_SR_FIFOFULL_Pos)
#define SFC_SR_FIFOOVF_Pos			30		// FIFO Overflow
#define SFC_SR_FIFOOVF_Msk			(0x01 << SFC_SR_FIFOOVF_Pos)
#define SFC_SR_FIFOUVF_Pos			31		// FIFO Underflow
#define SFC_SR_FIFOUVF_Msk			(0x01u<< SFC_SR_FIFOUVF_Pos)

#define SFC_IF_OVER_Pos				0		// operation done, wirte 1 to clear
#define SFC_IF_OVER_Msk				(0x01 << SFC_IF_OVER_Pos)
#define SFC_IF_ERR_Pos				1		// Error
#define SFC_IF_ERR_Msk				(0x01 << SFC_IF_ERR_Pos)
#define SFC_IF_TO_Pos				2		// Timeout
#define SFC_IF_TO_Msk				(0x01 << SFC_IF_TO_Pos)
#define SFC_IF_FIFOEMPTY_Pos		27
#define SFC_IF_FIFOEMPTY_Msk		(0x01 << SFC_IF_FIFOEMPTY_Pos)
#define SFC_IF_FIFOHFULL_Pos		28
#define SFC_IF_FIFOHFULL_Msk		(0x01 << SFC_IF_FIFOHFULL_Pos)
#define SFC_IF_FIFOFULL_Pos			29
#define SFC_IF_FIFOFULL_Msk			(0x01 << SFC_IF_FIFOFULL_Pos)
#define SFC_IF_FIFOOVF_Pos			30
#define SFC_IF_FIFOOVF_Msk			(0x01 << SFC_IF_FIFOOVF_Pos)
#define SFC_IF_FIFOUVF_Pos			31
#define SFC_IF_FIFOUVF_Msk			(0x01u<< SFC_IF_FIFOUVF_Pos)

#define SFC_IE_OVER_Pos				0		// operation done interrupt enable
#define SFC_IE_OVER_Msk				(0x01 << SFC_IE_OVER_Pos)
#define SFC_IE_ERR_Pos				1
#define SFC_IE_ERR_Msk				(0x01 << SFC_IE_ERR_Pos)
#define SFC_IE_TO_Pos				2		// Timeout
#define SFC_IE_TO_Msk 				(0x01 << SFC_IE_TO_Pos)
#define SFC_IE_FIFOEMPTY_Pos		27
#define SFC_IE_FIFOEMPTY_Msk		(0x01 << SFC_IE_FIFOEMPTY_Pos)
#define SFC_IE_FIFOHFULL_Pos		28
#define SFC_IE_FIFOHFULL_Msk		(0x01 << SFC_IE_FIFOHFULL_Pos)
#define SFC_IE_FIFOFULL_Pos			29
#define SFC_IE_FIFOFULL_Msk			(0x01 << SFC_IE_FIFOFULL_Pos)
#define SFC_IE_FIFOOVF_Pos			30
#define SFC_IE_FIFOOVF_Msk			(0x01 << SFC_IE_FIFOOVF_Pos)
#define SFC_IE_FIFOUVF_Pos			31
#define SFC_IE_FIFOUVF_Msk			(0x01u<< SFC_IE_FIFOUVF_Pos)

#define SFC_CMDAHB_WREN_Pos			0		// Flash command opcode for write enable
#define SFC_CMDAHB_WREN_Msk			(0xFF << SFC_CMDAHB_WREN_Pos)
#define SFC_CMDAHB_PP_Pos			8		// Flash command opcode for page program
#define SFC_CMDAHB_PP_Msk			(0xFF << SFC_CMDAHB_PP_Pos)
#define SFC_CMDAHB_RDSRL_Pos		16		// Flash command opcode for read status register
#define SFC_CMDAHB_RDSRL_Msk		(0xFF << SFC_CMDAHB_RDSRL_Pos)
#define SFC_CMDAHB_READ_Pos			24		// Flash command opcode for data read
#define SFC_CMDAHB_READ_Msk			(0xFFu<< SFC_CMDAHB_READ_Pos)




typedef struct {
	__IO uint32_t CR;
	
	__I  uint32_t SR;
	
	__O  uint32_t SWTRG;					// Software trigger
	
	__IO uint32_t DHR;
} DAC_TypeDef;


#define DAC_CR_EN_Pos				0
#define DAC_CR_EN_Msk				(0x01 << DAC_CR_EN_Pos)
#define DAC_CR_DMAEN_Pos			3
#define DAC_CR_DMAEN_Msk			(0x01 << DAC_CR_DMAEN_Pos)
#define DAC_CR_DHRFMT_Pos			9		// DHR Format, 0 12-bit data, DHR[11:0] => DOR[11:0]; 1 12-bit data, DHR[15:4] => DOR[11:0]; 3 8-bit data, DHR[7 :0] => DOR[11:4]
#define DAC_CR_DHRFMT_Msk			(0x03 << DAC_CR_DHRFMT_Pos)

#define DAC_SR_DHRFULL_Pos			0		// 0 DHR not Full, can write now
#define DAC_SR_DHRFULL_Msk			(0x01 << DAC_SR_DHRFULL_Pos)




typedef struct {
    __IO uint32_t CR;
    
	__O  uint32_t DATAIN;
	
    __IO uint32_t INIVAL;					// inital value, be written to RESULT when write 1 to CR.EN
    
    __I  uint32_t RESULT;
} CRC_TypeDef;


#define CRC_CR_EN_Pos			    0
#define CRC_CR_EN_Msk			    (0x01 << CRC_CR_EN_Pos)
#define CRC_CR_IREV_Pos				1       // input data reverse enable, 0 bit order not change, 1 bit order completely reverse, 2 bit order intra-byte reverse, 3 byte order reverse
#define CRC_CR_IREV_Msk				(0x03 << CRC_CR_IREV_Pos)
#define CRC_CR_INOT_Pos				3       // input data inverse enable
#define CRC_CR_INOT_Msk				(0x01 << CRC_CR_INOT_Pos)
#define CRC_CR_OREV_Pos				4       // output data reverse enable
#define CRC_CR_OREV_Msk				(0x03 << CRC_CR_OREV_Pos)
#define CRC_CR_ONOT_Pos				6       // output data inverse enable
#define CRC_CR_ONOT_Msk				(0x01 << CRC_CR_ONOT_Pos)
#define CRC_CR_POLY_Pos				7       // CRC polynomial, 0 x^16+x^12+x^5+1, 1 x^8+x^2+x+1, 2 x^16+x^15+x^2+1, 3 x^32+x^26+x^23+x^22+x^16+x^12+x^11+x^10+x^8+x^7+x^5+x^4+x^2+x+1
#define CRC_CR_POLY_Msk				(0x03 << CRC_CR_POLY_Pos)
#define CRC_CR_IBIT_Pos				9       // input data width, 0 32-bit, 1 16-bit, 2 8-bit
#define CRC_CR_IBIT_Msk				(0x03 << CRC_CR_IBIT_Pos)




typedef struct {
    __IO uint32_t MINSEC;                   // minute, second
    
    __IO uint32_t DATHUR;                   // date, hour
    
    __IO uint32_t MONDAY;                   // month, day
    
    __IO uint32_t YEAR;                     // year, support 1901-2199
    
    __IO uint32_t MINSECAL;                 // minute and second alarm
    
    __IO uint32_t DAYHURAL;                 // day and hour alarm
    
    __IO uint32_t LOAD;                     // load configure register value to RTC internal working register, automatically clear
    
    __IO uint32_t IE;
    
    __IO uint32_t IF;                       // write 1 to clear
    
    __IO uint32_t EN;                       // [0] RTC enalbe
    
    __IO uint32_t CFGABLE;                  // [0] RTC configurable
    
    __IO uint32_t TRIM;
    
    __IO uint32_t TRIMM;
} RTC_TypeDef;


#define RTC_LOAD_TIME_Pos			0
#define RTC_LOAD_TIME_Msk			(0x01 << RTC_LOAD_TIME_Pos)
#define RTC_LOAD_ALARM_Pos			1
#define RTC_LOAD_ALARM_Msk			(0x01 << RTC_LOAD_ALARM_Pos)

#define RTC_MINSEC_SEC_Pos			0       // second, can be 0--59
#define RTC_MINSEC_SEC_Msk		    (0x3F << RTC_MINSEC_SEC_Pos)
#define RTC_MINSEC_MIN_Pos			6       // minute, can be 0--59
#define RTC_MINSEC_MIN_Msk		    (0x3F << RTC_MINSEC_MIN_Pos)

#define RTC_DATHUR_HOUR_Pos			0       // hour, can be 0--23
#define RTC_DATHUR_HOUR_Msk		    (0x1F << RTC_DATHUR_HOUR_Pos)
#define RTC_DATHUR_DATE_Pos			5       // date of month, can be 1--31
#define RTC_DATHUR_DATE_Msk		    (0x1F << RTC_DATHUR_DATE_Pos)

#define RTC_MONDAY_DAY_Pos			0       // day of week, can be 0--6
#define RTC_MONDAY_DAY_Msk		    (0x07 << RTC_MONDAY_DAY_Pos)
#define RTC_MONDAY_MON_Pos			3       // month, can be 1--12
#define RTC_MONDAY_MON_Msk		    (0x0F << RTC_MONDAY_MON_Pos)

#define RTC_MINSECAL_SEC_Pos		0
#define RTC_MINSECAL_SEC_Msk		(0x3F << RTC_MINSECAL_SEC_Pos)
#define RTC_MINSECAL_MIN_Pos	    6
#define RTC_MINSECAL_MIN_Msk		(0x3F << RTC_MINSECAL_MIN_Pos)

#define RTC_DAYHURAL_HOUR_Pos		0
#define RTC_DAYHURAL_HOUR_Msk		(0x1F << RTC_DAYHURAL_HOUR_Pos)
#define RTC_DAYHURAL_SUN_Pos		5
#define RTC_DAYHURAL_SUN_Msk		(0x01 << RTC_DAYHURAL_SUN_Pos)
#define RTC_DAYHURAL_MON_Pos		6
#define RTC_DAYHURAL_MON_Msk		(0x01 << RTC_DAYHURAL_MON_Pos)
#define RTC_DAYHURAL_TUE_Pos		7
#define RTC_DAYHURAL_TUE_Msk		(0x01 << RTC_DAYHURAL_TUE_Pos)
#define RTC_DAYHURAL_WED_Pos		8
#define RTC_DAYHURAL_WED_Msk		(0x01 << RTC_DAYHURAL_WED_Pos)
#define RTC_DAYHURAL_THU_Pos		9
#define RTC_DAYHURAL_THU_Msk		(0x01 << RTC_DAYHURAL_THU_Pos)
#define RTC_DAYHURAL_FRI_Pos		10
#define RTC_DAYHURAL_FRI_Msk		(0x01 << RTC_DAYHURAL_FRI_Pos)
#define RTC_DAYHURAL_SAT_Pos		11
#define RTC_DAYHURAL_SAT_Msk		(0x01 << RTC_DAYHURAL_SAT_Pos)

#define RTC_IE_SEC_Pos		        0       // second interrupt enable
#define RTC_IE_SEC_Msk		        (0x01 << RTC_IE_SEC_Pos)
#define RTC_IE_MIN_Pos		        1
#define RTC_IE_MIN_Msk		        (0x01 << RTC_IE_MIN_Pos)
#define RTC_IE_HOUR_Pos		        2
#define RTC_IE_HOUR_Msk		        (0x01 << RTC_IE_HOUR_Pos)
#define RTC_IE_DATE_Pos		        3
#define RTC_IE_DATE_Msk		        (0x01 << RTC_IE_DATE_Pos)
#define RTC_IE_ALARM_Pos		    4
#define RTC_IE_ALARM_Msk		    (0x01 << RTC_IE_ALARM_Pos)
#define RTC_IE_TRIM_Pos				5
#define RTC_IE_TRIM_Msk				(0x01 << RTC_IE_TRIM_Pos)
#define RTC_IE_HSEC_Pos				6		// half-second interrupt enable
#define RTC_IE_HSEC_Msk				(0x01 << RTC_IE_HSEC_Pos)
#define RTC_IE_QSEC_Pos				7		// quad-second interrupt enable
#define RTC_IE_QSEC_Msk				(0x01 << RTC_IE_QSEC_Pos)

#define RTC_IF_SEC_Pos		        0       // write 1 to clear
#define RTC_IF_SEC_Msk		        (0x01 << RTC_IF_SEC_Pos)
#define RTC_IF_MIN_Pos		        1
#define RTC_IF_MIN_Msk		        (0x01 << RTC_IF_MIN_Pos)
#define RTC_IF_HOUR_Pos		        2
#define RTC_IF_HOUR_Msk		        (0x01 << RTC_IF_HOUR_Pos)
#define RTC_IF_DATE_Pos		        3
#define RTC_IF_DATE_Msk		        (0x01 << RTC_IF_DATE_Pos)
#define RTC_IF_ALARM_Pos		    4
#define RTC_IF_ALARM_Msk		    (0x01 << RTC_IF_ALARM_Pos)
#define RTC_IF_TRIM_Pos				5
#define RTC_IF_TRIM_Msk				(0x01 << RTC_IF_TRIM_Pos)
#define RTC_IF_HSEC_Pos				6
#define RTC_IF_HSEC_Msk				(0x01 << RTC_IF_HSEC_Pos)
#define RTC_IF_QSEC_Pos				7
#define RTC_IF_QSEC_Msk				(0x01 << RTC_IF_QSEC_Pos)

#define RTC_TRIM_ADJ_Pos		    0       //Used to adjust the count period of BASECNT, the default is 32768, if DEC is 1, the count period is adjusted to 32768-ADJ, otherwise 32768+ADJ
#define RTC_TRIM_ADJ_Msk		    (0xFF << RTC_TRIM_ADJ_Pos)
#define RTC_TRIM_DEC_Pos		    8
#define RTC_TRIM_DEC_Msk		    (0x01 << RTC_TRIM_DEC_Pos)

#define RTC_TRIMM_CYCLE_Pos		    0       //For counting period fine-tuning, if INC is 1, the Nth counting period is adjusted to (32768+/-ADJ)+1, otherwise it is adjusted to (32768+/-ADJ)-1
#define RTC_TRIMM_CYCLE_Msk		    (0x07 << RTC_TRIMM_CYCLE_Pos)
#define RTC_TRIMM_INC_Pos		    3
#define RTC_TRIMM_INC_Msk		    (0x01 << RTC_TRIMM_INC_Pos)




typedef struct {
	__IO uint32_t RSTVAL;					// a reset occurs when the counter counts to this value
	
	__IO uint32_t INTVAL;					// an interrupt occurs when the counter counts to this value
	
	__IO uint32_t CR;
	
	__IO uint32_t IF;						// [0] interrupt flag, write 1 to clear
	
	__IO uint32_t FEED;						// write 0x55 to feed dog
} WDT_TypeDef;


#define WDT_CR_EN_Pos				0
#define WDT_CR_EN_Msk				(0x01 << WDT_CR_EN_Pos)
#define WDT_CR_RSTEN_Pos			1
#define WDT_CR_RSTEN_Msk			(0x01 << WDT_CR_RSTEN_Pos)
#define WDT_CR_INTEN_Pos			2
#define WDT_CR_INTEN_Msk			(0x01 << WDT_CR_INTEN_Pos)
#define WDT_CR_WINEN_Pos			3		// window function enable
#define WDT_CR_WINEN_Msk			(0x01 << WDT_CR_WINEN_Pos)
#define WDT_CR_CKDIV_Pos			8		// clock divider, 0 div2, 1 div4, 2 div8, ...
#define WDT_CR_CKDIV_Msk			(0x0F << WDT_CR_CKDIV_Pos)




typedef struct {
	__IO uint32_t TXBUF[16][0x100];			// 0x4000, 16KB
	
	__IO uint32_t RXBUF[0x100];				// 0x4400, 17KB
	
	struct {
		__IO uint32_t TXCR;					// Tx Control
		
		__IO uint32_t TXSR;					// Tx Status
		
			 uint32_t RESERVED;
		
		__IO uint32_t TXTRSZ;				// Tx Transfer Size
	} INEP[16];
	
	struct {
		__IO uint32_t RXCR;					// Rx Control
		
			 uint32_t RESERVED[3];
	} OUTEP[16];							// 0x4600
	
	__IO uint32_t DEVCR;					// Device Control
	
	__I  uint32_t DEVSR;					// Device Status
	
	__IO uint32_t DEVIF;					// Device Interrupt Flag
	
	__IO uint32_t DEVIE;					// Device Interrupt Enable
	
		 uint32_t RESERVED;
	
	__IO uint32_t EPIE;						// Endpoint Interrupt Enable
	
	__IO uint32_t FFTHR;					// FIFO Threshold
	
	__IO uint32_t RXSR;						// Rx Status
	
	__IO uint32_t SETUPSR;					// Setup Status
	
	__IO uint32_t EPIF;						// Endpoint Interrupt Flag
	
	__IO uint32_t FRAMENR;					// Frame Number, set the frame number for isochronous transfer
	
		 uint32_t RESERVED1[(0x4700-0x4628)/4-1];
		 
	__IO uint32_t SETUPD1;
	
	__IO uint32_t SETUPD2;
	
		 uint32_t RESERVED2[(0x4804-0x4704)/4-1];
	
	__IO uint32_t EPCFG[16];
} USBD_TypeDef;


#define USBD_TXCR_SNDSTALL_Pos		0		// Send Stall
#define USBD_TXCR_SNDSTALL_Msk		(0x01 << USBD_TXCR_SNDSTALL_Pos)
#define USBD_TXCR_FLUSHFF_Pos		2		// Flush FIFO
#define USBD_TXCR_FLUSHFF_Msk		(0x01 << USBD_TXCR_FLUSHFF_Pos)
#define USBD_TXCR_FFRDY_Pos			3		// FIFO Ready
#define USBD_TXCR_FFRDY_Msk			(0x01 << USBD_TXCR_FFRDY_Pos)

#define USBD_TXSR_SUCC_Pos			0		// Success
#define USBD_TXSR_SUCC_Msk			(0x01 << USBD_TXSR_SUCC_Pos)
#define USBD_TXSR_DATSNT_Pos		1		// Data Sent
#define USBD_TXSR_DATSNT_Msk		(0x01 << USBD_TXSR_DATSNT_Pos)
#define USBD_TXSR_NAKSNT_Pos		3		// NAK Sent
#define USBD_TXSR_NAKSNT_Msk		(0x01 << USBD_TXSR_NAKSNT_Pos)

#define USBD_RXCR_SNDSTALL_Pos		0		// Send Stall
#define USBD_RXCR_SNDSTALL_Msk		(0x01 << USBD_RXCR_SNDSTALL_Pos)
#define USBD_RXCR_FLUSHFF_Pos		2		// Flush FIFO
#define USBD_RXCR_FLUSHFF_Msk		(0x01 << USBD_RXCR_FLUSHFF_Pos)
#define USBD_RXCR_FFRDY_Pos			3		// FIFO Ready
#define USBD_RXCR_FFRDY_Msk			(0x01 << USBD_RXCR_FFRDY_Pos)

#define USBD_DEVCR_SPEED_Pos		0		// 2 LS, 3 FS
#define USBD_DEVCR_SPEED_Msk		(0x03 << USBD_DEVCR_SPEED_Pos)
#define USBD_DEVCR_REMOTEWKUP_Pos	2
#define USBD_DEVCR_REMOTEWKUP_Msk	(0x01 << USBD_DEVCR_REMOTEWKUP_Pos)
#define USBD_DEVCR_SELFPWRD_Pos		3		// Self Powered
#define USBD_DEVCR_SELFPWRD_Msk		(0x01 << USBD_DEVCR_SELFPWRD_Pos)
#define USBD_DEVCR_SYNCFRAME_Pos	4		// Sync Frame Command Support
#define USBD_DEVCR_SYNCFRAME_Msk	(0x01 << USBD_DEVCR_SYNCFRAME_Pos)
#define USBD_DEVCR_CSRDONE_Pos		6		// CSR Configured Done
#define USBD_DEVCR_CSRDONE_Msk		(0x01 << USBD_DEVCR_CSRDONE_Pos)
#define USBD_DEVCR_SETDESC_Pos		7		// Set Descriptor Command Support
#define USBD_DEVCR_SETDESC_Msk		(0x01 << USBD_DEVCR_SETDESC_Pos)
#define USBD_DEVCR_DEVICE_Pos		8		// 1 Device, 0 Host
#define USBD_DEVCR_DEVICE_Msk		(0x01 << USBD_DEVCR_DEVICE_Pos)

#define USBD_DEVSR_CFG_Pos			0		// value set by Set Configuration command
#define USBD_DEVSR_CFG_Msk			(0x0F << USBD_DEVSR_CFG_Pos)
#define USBD_DEVSR_INTF_Pos			4		// value set by Set Interface command
#define USBD_DEVSR_INTF_Msk			(0x0F << USBD_DEVSR_INTF_Pos)
#define USBD_DEVSR_ALT_Pos			8		// value set by Set Alternate command
#define USBD_DEVSR_ALT_Msk			(0x0F << USBD_DEVSR_ALT_Pos)
#define USBD_DEVSR_SUSP_Pos			12		// set when Suspend condition detected on the USB
#define USBD_DEVSR_SUSP_Msk			(0x01 << USBD_DEVSR_SUSP_Pos)
#define USBD_DEVSR_FRNR_Pos			21		// Frame Number of received SOF
#define USBD_DEVSR_FRNR_Msk			(0x7FF<< USBD_DEVSR_FRNR_Pos)

#define USBD_DEVIF_SETCFG_Pos		0		// Set when receive Set Configuration command
#define USBD_DEVIF_SETCFG_Msk		(0x01 << USBD_DEVIF_SETCFG_Pos)
#define USBD_DEVIF_SETINTF_Pos		1		// Set when receive Set Interface command
#define USBD_DEVIF_SETINTF_Msk		(0x01 << USBD_DEVIF_SETINTF_Pos)
#define USBD_DEVIF_RST_Pos			3		// Set when Reset detected on the USB
#define USBD_DEVIF_RST_Msk			(0x01 << USBD_DEVIF_RST_Pos)
#define USBD_DEVIF_SUSP_Pos			4		// Set when Suspend detected on the USB
#define USBD_DEVIF_SUSP_Msk			(0x01 << USBD_DEVIF_SUSP_Pos)
#define USBD_DEVIF_SOF_Pos			5		// Set when SOF received
#define USBD_DEVIF_SOF_Msk			(0x01 << USBD_DEVIF_SOF_Pos)
#define USBD_DEVIF_SETUP_Pos		6		// Set when Setup received
#define USBD_DEVIF_SETUP_Msk		(0x01 << USBD_DEVIF_SETUP_Pos)
#define USBD_DEVIF_OUT_Pos			7		// Set when OUT received
#define USBD_DEVIF_OUT_Msk			(0x01 << USBD_DEVIF_OUT_Pos)

#define USBD_DEVIE_SETCFG_Pos		0
#define USBD_DEVIE_SETCFG_Msk		(0x01 << USBD_DEVIE_SETCFG_Pos)
#define USBD_DEVIE_SETINTF_Pos		1
#define USBD_DEVIE_SETINTF_Msk		(0x01 << USBD_DEVIE_SETINTF_Pos)
#define USBD_DEVIE_RST_Pos			3
#define USBD_DEVIE_RST_Msk			(0x01 << USBD_DEVIE_RST_Pos)
#define USBD_DEVIE_SUSP_Pos			4
#define USBD_DEVIE_SUSP_Msk			(0x01 << USBD_DEVIE_SUSP_Pos)
#define USBD_DEVIE_SOF_Pos			5
#define USBD_DEVIE_SOF_Msk			(0x01 << USBD_DEVIE_SOF_Pos)
#define USBD_DEVIE_SETUP_Pos		6
#define USBD_DEVIE_SETUP_Msk		(0x01 << USBD_DEVIE_SETUP_Pos)
#define USBD_DEVIE_OUT_Pos			7
#define USBD_DEVIE_OUT_Msk			(0x01 << USBD_DEVIE_OUT_Pos)

#define USBD_EPIE_INEP0_Pos			0
#define USBD_EPIE_INEP0_Msk			(0x01 << USBD_EPIE_INEP0_Pos)
#define USBD_EPIE_INEP1_Pos			1
#define USBD_EPIE_INEP1_Msk			(0x01 << USBD_EPIE_INEP1_Pos)
#define USBD_EPIE_INEP2_Pos			2
#define USBD_EPIE_INEP2_Msk			(0x01 << USBD_EPIE_INEP2_Pos)
#define USBD_EPIE_INEP3_Pos			3
#define USBD_EPIE_INEP3_Msk			(0x01 << USBD_EPIE_INEP3_Pos)
#define USBD_EPIE_INEP4_Pos			4
#define USBD_EPIE_INEP4_Msk			(0x01 << USBD_EPIE_INEP4_Pos)
#define USBD_EPIE_INEP5_Pos			5
#define USBD_EPIE_INEP5_Msk			(0x01 << USBD_EPIE_INEP5_Pos)
#define USBD_EPIE_INEP6_Pos			6
#define USBD_EPIE_INEP6_Msk			(0x01 << USBD_EPIE_INEP6_Pos)
#define USBD_EPIE_INEP7_Pos			7
#define USBD_EPIE_INEP7_Msk			(0x01 << USBD_EPIE_INEP7_Pos)
#define USBD_EPIE_OUTEP0_Pos		16
#define USBD_EPIE_OUTEP0_Msk		(0x01 << USBD_EPIE_OUTEP0_Pos)
#define USBD_EPIE_OUTEP1_Pos		17
#define USBD_EPIE_OUTEP1_Msk		(0x01 << USBD_EPIE_OUTEP1_Pos)
#define USBD_EPIE_OUTEP2_Pos		18
#define USBD_EPIE_OUTEP2_Msk		(0x01 << USBD_EPIE_OUTEP2_Pos)
#define USBD_EPIE_OUTEP3_Pos		19
#define USBD_EPIE_OUTEP3_Msk		(0x01 << USBD_EPIE_OUTEP3_Pos)
#define USBD_EPIE_OUTEP4_Pos		20
#define USBD_EPIE_OUTEP4_Msk		(0x01 << USBD_EPIE_OUTEP4_Pos)
#define USBD_EPIE_OUTEP5_Pos		21
#define USBD_EPIE_OUTEP5_Msk		(0x01 << USBD_EPIE_OUTEP5_Pos)
#define USBD_EPIE_OUTEP6_Pos		22
#define USBD_EPIE_OUTEP6_Msk		(0x01 << USBD_EPIE_OUTEP6_Pos)
#define USBD_EPIE_OUTEP7_Pos		23
#define USBD_EPIE_OUTEP7_Msk		(0x01 << USBD_EPIE_OUTEP7_Pos)

#define USBD_RXSR_SUCC_Pos			0		// Success
#define USBD_RXSR_SUCC_Msk			(0x01 << USBD_RXSR_SUCC_Pos)
#define USBD_RXSR_DONE_Pos			1		// 1 Transfer completed
#define USBD_RXSR_DONE_Msk			(0x01 << USBD_RXSR_DONE_Pos)
#define USBD_RXSR_EPNR_Pos			16		// Endpoint Number
#define USBD_RXSR_EPNR_Msk			(0x0F << USBD_RXSR_EPNR_Pos)
#define USBD_RXSR_TRSZ_Pos			22		// Transfer Size in byte
#define USBD_RXSR_TRSZ_Msk			(0x3FFu<<USBD_RXSR_TRSZ_Pos)

#define USBD_SETUPSR_SUCC_Pos		0		// Success
#define USBD_SETUPSR_SUCC_Msk		(0x01 << USBD_SETUPSR_SUCC_Pos)
#define USBD_SETUPSR_DONE_Pos		1		// 1 Transfer completed
#define USBD_SETUPSR_DONE_Msk		(0x01 << USBD_SETUPSR_DONE_Pos)
#define USBD_SETUPSR_EPNR_Pos		16
#define USBD_SETUPSR_EPNR_Msk		(0x0F << USBD_SETUPSR_EPNR_Pos)

#define USBD_EPIF_INEP0_Pos			0		// data sent or NAK sent
#define USBD_EPIF_INEP0_Msk			(0x01 << USBD_EPIF_INEP0_Pos)
#define USBD_EPIF_INEP1_Pos			1
#define USBD_EPIF_INEP1_Msk			(0x01 << USBD_EPIF_INEP1_Pos)
#define USBD_EPIF_INEP2_Pos			2
#define USBD_EPIF_INEP2_Msk			(0x01 << USBD_EPIF_INEP2_Pos)
#define USBD_EPIF_INEP3_Pos			3
#define USBD_EPIF_INEP3_Msk			(0x01 << USBD_EPIF_INEP3_Pos)
#define USBD_EPIF_INEP4_Pos			4
#define USBD_EPIF_INEP4_Msk			(0x01 << USBD_EPIF_INEP4_Pos)
#define USBD_EPIF_INEP5_Pos			5
#define USBD_EPIF_INEP5_Msk			(0x01 << USBD_EPIF_INEP5_Pos)
#define USBD_EPIF_INEP6_Pos			6
#define USBD_EPIF_INEP6_Msk			(0x01 << USBD_EPIF_INEP6_Pos)
#define USBD_EPIF_INEP7_Pos			7
#define USBD_EPIF_INEP7_Msk			(0x01 << USBD_EPIF_INEP7_Pos)
#define USBD_EPIF_OUTEP0_Pos		16		//data available
#define USBD_EPIF_OUTEP0_Msk		(0x01 << USBD_EPIF_OUTEP0_Pos)
#define USBD_EPIF_OUTEP1_Pos		17
#define USBD_EPIF_OUTEP1_Msk		(0x01 << USBD_EPIF_OUTEP1_Pos)
#define USBD_EPIF_OUTEP2_Pos		18
#define USBD_EPIF_OUTEP2_Msk		(0x01 << USBD_EPIF_OUTEP2_Pos)
#define USBD_EPIF_OUTEP3_Pos		19
#define USBD_EPIF_OUTEP3_Msk		(0x01 << USBD_EPIF_OUTEP3_Pos)
#define USBD_EPIF_OUTEP4_Pos		20
#define USBD_EPIF_OUTEP4_Msk		(0x01 << USBD_EPIF_OUTEP4_Pos)
#define USBD_EPIF_OUTEP5_Pos		21
#define USBD_EPIF_OUTEP5_Msk		(0x01 << USBD_EPIF_OUTEP5_Pos)
#define USBD_EPIF_OUTEP6_Pos		22
#define USBD_EPIF_OUTEP6_Msk		(0x01 << USBD_EPIF_OUTEP6_Pos)
#define USBD_EPIF_OUTEP7_Pos		23
#define USBD_EPIF_OUTEP7_Msk		(0x01 << USBD_EPIF_OUTEP7_Pos)

#define USBD_EPCFG_EPNR_Pos			0		// Endpoint Number
#define USBD_EPCFG_EPNR_Msk			(0x0F << USBD_EPCFG_EPNR_Pos)
#define USBD_EPCFG_DIR_Pos			4		// Endpoint Direction, 0 OUT, 1 IN
#define USBD_EPCFG_DIR_Msk			(0x01 << USBD_EPCFG_DIR_Pos)
#define USBD_EPCFG_TYPE_Pos			5		// Endpoint Type, 0 Control, 1 Isochronous, 2 Bulk, 3 Interrupt
#define USBD_EPCFG_TYPE_Msk			(0x03 << USBD_EPCFG_TYPE_Pos)
#define USBD_EPCFG_CFG_Pos			7		// Configuration number to which this endpoint belongs
#define USBD_EPCFG_CFG_Msk			(0x0F << USBD_EPCFG_CFG_Pos)
#define USBD_EPCFG_INTF_Pos			11		// Interface number to which this endpoint belongs
#define USBD_EPCFG_INTF_Msk			(0x0F << USBD_EPCFG_INTF_Pos)
#define USBD_EPCFG_ALT_Pos			15		// Alternate setting to which this endpoint belongs
#define USBD_EPCFG_ALT_Msk			(0x0F << USBD_EPCFG_ALT_Pos)
#define USBD_EPCFG_PKSZ_Pos			19		// Maximum packet size
#define USBD_EPCFG_PKSZ_Msk			(0xFF << USBD_EPCFG_PKSZ_Pos)




typedef struct {
	__IO uint32_t TXBUF[0x100];
	
		 uint32_t RESERVED1[15][0x100];		// 0x4000, 16KB
	
	__IO uint32_t RXBUF[0x100];
	
		 uint32_t RESERVED2[0x200];			// 0x4C00
	
	__IO uint32_t IF;						// Interrupt Flag
	
	__IO uint32_t IE;						// Interrupt Enable
	
	__IO uint32_t SR;						// Status Register
	
	__IO uint32_t CR;						// Control Register
	
	__IO uint32_t FFSZ;						// FIFO Size
	
	__IO uint32_t FFTHR;					// FIFO Threshold
	
	__IO uint32_t TXTRSZ;					// Tx Transfer Size in byte
	
		 uint32_t RESERVED3[(0x5034-0x4C18)/4-1];
		
	__IO uint32_t FRAMEIV;					// Frame Interval, 11999 for 1ms
	
	__IO uint32_t FRAMERM;					// Frame Remaining
	
	__IO uint32_t FRAMENR;					// Frame Number
	
		 uint32_t RESERVED4[(0x5054-0x503C)/4-1];
		 
	__IO uint32_t PORTSR;					// Port Status
	
		 uint32_t RESERVED5[(0x5090-0x5054)/4-1];
		 
	__IO uint32_t TOKEN;
	
	__IO uint32_t OTGCSR;					// OTG Control and Status
} USBH_TypeDef;


#define USBH_IF_RXSTAT_Pos			0		// receive a status write form device
#define USBH_IF_RXSTAT_Msk			(0x01 << USBH_IF_RXSTAT_Pos)
#define USBH_IF_ABVTHR_Pos			1		// RX FIFO Above Threshold
#define USBH_IF_ABVTHR_Msk			(0x01 << USBH_IF_ABVTHR_Pos)
#define USBH_IF_BLWTHR_Pos			2		// TX FIFO Below Threshold
#define USBH_IF_BLWTHR_Msk			(0x01 << USBH_IF_BLWTHR_Pos)
#define USBH_IF_SOF_Pos				3		// SOF sent
#define USBH_IF_SOF_Msk				(0x01 << USBH_IF_SOF_Pos)
#define USBH_IF_PORT_Pos			8		// Port status changed
#define USBH_IF_PORT_Msk			(0x01 << USBH_IF_PORT_Pos)
#define USBH_IF_OTG_Pos				9		// SRP or HNP
#define USBH_IF_OTG_Msk				(0x01 << USBH_IF_OTG_Pos)

#define USBH_IE_RXSTAT_Pos			0
#define USBH_IE_RXSTAT_Msk			(0x01 << USBH_IE_RXSTAT_Pos)
#define USBH_IE_ABVTHR_Pos			1
#define USBH_IE_ABVTHR_Msk			(0x01 << USBH_IE_ABVTHR_Pos)
#define USBH_IE_BLWTHR_Pos			2
#define USBH_IE_BLWTHR_Msk			(0x01 << USBH_IE_BLWTHR_Pos)
#define USBH_IE_SOF_Pos				3
#define USBH_IE_SOF_Msk				(0x01 << USBH_IE_SOF_Pos)
#define USBH_IE_PORT_Pos			8
#define USBH_IE_PORT_Msk			(0x01 << USBH_IE_PORT_Pos)
#define USBH_IE_OTG_Pos				9
#define USBH_IE_OTG_Msk				(0x01 << USBH_IE_OTG_Pos)

#define USBH_SR_RESP_Pos			0		// Response Code
#define USBH_SR_RESP_Msk			(0x0F << USBH_SR_RESP_Pos)
#define USBH_SR_TRSZ_Pos			4		// Transfer Size in previous transaction
#define USBH_SR_TRSZ_Msk			(0x3FF<< USBH_SR_TRSZ_Pos)

#define USBH_CR_FLUSHFF_Pos			0		// Flush FIFO
#define USBH_CR_FLUSHFF_Msk			(0x01 << USBH_CR_FLUSHFF_Pos)

#define USBH_FFSZ_RX_Pos			0		// RX FIFO Size in word
#define USBH_FFSZ_RX_Msk			(0x3FF<< USBH_FFSZ_RX_Pos)
#define USBH_FFSZ_TX_Pos			16		// TX FIFO Size in word
#define USBH_FFSZ_TX_Msk			(0x3FF<< USBH_FFSZ_TX_Pos)

#define USBH_FFTHR_TX_Pos			0		// TX FIFO Threshold in word
#define USBH_FFTHR_TX_Msk			(0x3FF<< USBH_FFTHR_TX_Pos)
#define USBH_FFTHR_RX_Pos			16		// RX FIFO Threshold in word
#define USBH_FFTHR_RX_Msk			(0x3FF<< USBH_FFTHR_RX_Pos)

#define USBH_PORTSR_CONN_Pos		0		// Read: 1 Device connected, 0 Device not connected
#define USBH_PORTSR_CONN_Msk		(0x01 << USBH_PORTSR_CONN_Pos)
#define USBH_PORTSR_CLRENA_Pos		0		// Write: Clear Port Enable
#define USBH_PORTSR_CLRENA_Msk		(0x01 << USBH_PORTSR_CLRENA_Pos)
#define USBH_PORTSR_ENA_Pos			1		// 1 Port enabled, 0 Port disabled
#define USBH_PORTSR_ENA_Msk			(0x01 << USBH_PORTSR_ENA_Pos)
#define USBH_PORTSR_SUSP_Pos		2		// 1 Port is suspended, 0 Port is not suspended
#define USBH_PORTSR_SUSP_Msk		(0x01 << USBH_PORTSR_SUSP_Pos)
#define USBH_PORTSR_CLRSUSP_Pos		3		// Write: Clear Suspend Status, Initiate Resume
#define USBH_PORTSR_CLRSUSP_Msk		(0x01 << USBH_PORTSR_CLRSUSP_Pos)
#define USBH_PORTSR_RESET_Pos		4		// 1 Port Reset signalling active, 0 Port Reset signalling not active
#define USBH_PORTSR_RESET_Msk		(0x01 << USBH_PORTSR_RESET_Pos)
#define USBH_PORTSR_POWER_Pos		8		// 1 Port power on, 0 Port power off
#define USBH_PORTSR_POWER_Msk		(0x01 << USBH_PORTSR_POWER_Pos)
#define USBH_PORTSR_SPEED_Pos		9		// Read: 1 Low-Speed device attached, 0 Full-Speed device attached
#define USBH_PORTSR_SPEED_Msk		(0x01 << USBH_PORTSR_SPEED_Pos)
#define USBH_PORTSR_CLRPOWER_Pos	9		// Write: Clear Port Power status, write 1 to clear PORTSR.POWER
#define USBH_PORTSR_CLRPOWER_Msk	(0x01 << USBH_PORTSR_CLRPOWER_Pos)
#define USBH_PORTSR_CONNCHG_Pos		16		// 1 PORTSR.CONN changed
#define USBH_PORTSR_CONNCHG_Msk		(0x01 << USBH_PORTSR_CONNCHG_Pos)
#define USBH_PORTSR_ENACHG_Pos		17		// 1 PORTSR.ENA changed
#define USBH_PORTSR_ENACHG_Msk		(0x01 << USBH_PORTSR_ENACHG_Pos)
#define USBH_PORTSR_SUSPCHG_Pos		18		// set when a full Resume sequence is finished, write 1 to clear
#define USBH_PORTSR_SUSPCHG_Msk		(0x01 << USBH_PORTSR_SUSPCHG_Pos)
#define USBH_PORTSR_RSTCHG_Pos		20		// set at the end of the Port Reset signal, write 1 to clear
#define USBH_PORTSR_RSTCHG_Msk		(0x01 << USBH_PORTSR_RSTCHG_Pos)

#define USBH_TOKEN_ADDR_Pos			0
#define USBH_TOKEN_ADDR_Msk			(0x7F << USBH_TOKEN_ADDR_Pos)
#define USBH_TOKEN_EPNR_Pos			7		// Endport Number
#define USBH_TOKEN_EPNR_Msk			(0x0F << USBH_TOKEN_EPNR_Pos)
#define USBH_TOKEN_DATAX_Pos		11		// 0 DATA0, 1 DATA1
#define USBH_TOKEN_DATAX_Msk		(0x01 << USBH_TOKEN_DATAX_Pos)
#define USBH_TOKEN_TYPE_Pos			13		// 13 SETUP, 1 OUT, 9 IN
#define USBH_TOKEN_TYPE_Msk			(0x0F << USBH_TOKEN_TYPE_Pos)
#define USBH_TOKEN_ISO_Pos			17		// 1 ISO transfer, 0 Non-ISO transfer
#define USBH_TOKEN_ISO_Msk			(0x01 << USBH_TOKEN_ISO_Pos)
#define USBH_TOKEN_SPEED_Pos		18		// 2 LS, 3 FS
#define USBH_TOKEN_SPEED_Msk		(0x03 << USBH_TOKEN_SPEED_Pos)
#define USBH_TOKEN_TRSZ_Pos			20		//Transfer Size in byte
#define USBH_TOKEN_TRSZ_Msk			(0x3FF<< USBH_TOKEN_TRSZ_Pos)

#define USBH_OTGCSR_SRSUCC_Pos		0		// Session Request Success
#define USBH_OTGCSR_SRSUCC_Msk		(0x01 << USBH_OTGCSR_SRSUCC_Pos)
#define USBH_OTGCSR_SRSCHG_Pos		1		// Session Request Status Change,  set to 1 on Session Request Success, write 1 to clear
#define USBH_OTGCSR_SRSCHG_Msk		(0x01 << USBH_OTGCSR_SRSCHG_Pos)
#define USBH_OTGCSR_HNSUCC_Pos		2		// Host Negotiation Success
#define USBH_OTGCSR_HNSUCC_Msk		(0x01 << USBH_OTGCSR_HNSUCC_Pos)
#define USBH_OTGCSR_HNSCHG_Pos		3		// Host Negotiation Status Change, set to 1 on Host Negotiation Success, write 1 to clear
#define USBH_OTGCSR_HNSCHG_Msk		(0x01 << USBH_OTGCSR_HNSCHG_Pos)
#define USBH_OTGCSR_SRDET_Pos		4		// Session Request Detected
#define USBH_OTGCSR_SRDET_Msk		(0x01 << USBH_OTGCSR_SRDET_Pos)
#define USBH_OTGCSR_SRDCHG_Pos		5		// Session Request Detected Change, set to 1 when a Session Request is detected, write 1 to clear
#define USBH_OTGCSR_SRDCHG_Msk		(0x01 << USBH_OTGCSR_SRDCHG_Pos)
#define USBH_OTGCSR_HNDET_Pos		6		// Host Negotiation Detected
#define USBH_OTGCSR_HNDET_Msk		(0x01 << USBH_OTGCSR_HNDET_Pos)
#define USBH_OTGCSR_HNDCHG_Pos		7		// Host Negotiation Detect Change, set to 1 when a Host Negotiation is detected, write 1 to clear
#define USBH_OTGCSR_HNDCHG_Msk		(0x01 << USBH_OTGCSR_HNDCHG_Pos)
#define USBH_OTGCSR_ID_Pos			8		// Connector ID Status, 0 in A-device mode, 1 in B-device mode
#define USBH_OTGCSR_ID_Msk			(0x01 << USBH_OTGCSR_ID_Pos)
#define USBH_OTGCSR_IDCHG_Pos		9		// set when OTGCSR.ID changed, write 1 to clear
#define USBH_OTGCSR_IDCHG_Msk		(0x01 << USBH_OTGCSR_IDCHG_Pos)
#define USBH_OTGCSR_MODE_Pos		10		// 0 Host mode, 1 Device mode
#define USBH_OTGCSR_MODE_Msk		(0x01 << USBH_OTGCSR_MODE_Pos)
#define USBH_OTGCSR_SRPREQ_Pos		16		// SRP Request
#define USBH_OTGCSR_SRPREQ_Msk		(0x01 << USBH_OTGCSR_SRPREQ_Pos)
#define USBH_OTGCSR_HNPREQ_Pos		17		// HNP Request
#define USBH_OTGCSR_HNPREQ_Msk		(0x01 << USBH_OTGCSR_HNPREQ_Pos)
#define USBH_OTGCSR_HHNPENA_Pos		18		// Host HNP Enable, set by app when Set Feature Set HNP Enable is successful on connected device
#define USBH_OTGCSR_HHNPENA_Msk		(0x01 << USBH_OTGCSR_HHNPENA_Pos)
#define USBH_OTGCSR_HNPENA_Pos		19		// set by app when receives Set Feature Set HNP Enable command
#define USBH_OTGCSR_HNPENA_Msk		(0x01 << USBH_OTGCSR_HNPENA_Pos)
#define USBH_OTGCSR_SRPCAP_Pos		20		// SRP Capable
#define USBH_OTGCSR_SRPCAP_Msk		(0x01 << USBH_OTGCSR_SRPCAP_Pos)
#define USBH_OTGCSR_HNPCAP_Pos		21		// HNP Capable
#define USBH_OTGCSR_HNPCAP_Msk		(0x01 << USBH_OTGCSR_HNPCAP_Pos)




typedef struct {
	__IO uint32_t CFG;						// JPEG DEC configure register
	
	__IO uint32_t CR;						// JPEG DEC control register
	
	__IO uint32_t IR;						// JPEG DEC interrupt register
	
	__IO uint32_t SR;						// JPEG DEC status register
	
	__IO uint32_t IMGSIZ;					// JPEG image size
	
	__IO uint32_t IMGSTR;					// JPEG image size virtual stride
	
	__IO uint32_t CSBASE;					// JPEG code-stream base address
	
	union {
		__IO uint32_t YBASE;				// YUV image Y base address
		
		__IO uint32_t RGBASE;				// RGB image base address
	};
	
	__IO uint32_t UBASE;					// YUV image U base address
	
	__IO uint32_t VBASE;					// YUV image V base address
	
	__IO uint32_t QTBASE;					// JPEG quantization table base address
	
	__IO uint32_t HTBASE;					// JPEG huffman table base address
	
	__IO uint32_t CODLEN;					// JPEG code stream total length in byte
	
		 uint32_t RESERVED[51];
	
	__O  uint32_t QTABLE[3][16];			// Quantization table
	
		 uint32_t RESERVED2[16];
	
	struct {
		__O  uint32_t DC_CODEWORD[6];
		__O  uint32_t DC_CODELEN[2];
		__O  uint32_t DC_CODEVAL[2];
		
		__O  uint32_t AC_CODEWORD[8];
		__O  uint32_t AC_CODEADDR[4];
		__O  uint32_t AC_CODEVAL[41];
		
			 uint32_t RESERVED;
	} HTABLE[2];							// Huffman table
} JPEG_TypeDef;


#define JPEG_CFG_SRCFMT_Pos			0		// JPEG Source Format, 0 YUV420(H2V2), 1 YUV422(H2V1), 2 YUV444(H1V1)
#define JPEG_CFG_SRCFMT_Msk			(0x03 << JPEG_CFG_SRCFMT_Pos)
#define JPEG_CFG_SCANMOD_Pos		2		// JPEG scan mode, 0 interleaved (three components), 1 non-interleaved (single component)
#define JPEG_CFG_SCANMOD_Msk		(0x01 << JPEG_CFG_SCANMOD_Pos)
#define JPEG_CFG_NISCOMP_Pos		3		// Non-Interleaved Scanning Component, 0 component 1, 1 component 2, 2 component 3
#define JPEG_CFG_NISCOMP_Msk		(0x03 << JPEG_CFG_NISCOMP_Pos)
#define JPEG_CFG_HT1COMP_Pos		5		// Huffman Table for Component 1, 0 table 0, 1 table 1
#define JPEG_CFG_HT1COMP_Msk		(0x01 << JPEG_CFG_HT1COMP_Pos)
#define JPEG_CFG_HT2COMP_Pos		6
#define JPEG_CFG_HT2COMP_Msk		(0x01 << JPEG_CFG_HT2COMP_Pos)
#define JPEG_CFG_HT3COMP_Pos		7
#define JPEG_CFG_HT3COMP_Msk		(0x01 << JPEG_CFG_HT3COMP_Pos)
#define JPEG_CFG_QT1COMP_Pos		8		// Quantization Table for Component 1, 0 table 0, 1 table 1, 2 table 2
#define JPEG_CFG_QT1COMP_Msk		(0x03 << JPEG_CFG_QT1COMP_Pos)
#define JPEG_CFG_QT2COMP_Pos		10
#define JPEG_CFG_QT2COMP_Msk		(0x03 << JPEG_CFG_QT2COMP_Pos)
#define JPEG_CFG_QT3COMP_Pos		12
#define JPEG_CFG_QT3COMP_Msk		(0x03 << JPEG_CFG_QT3COMP_Pos)
#define JPEG_CFG_OUTFMT_Pos			14		// Image data output format, 0 YUV planar, 1 YUV semi-planar, 2 XRGB888, 3 RGB888, 4 RGB565
#define JPEG_CFG_OUTFMT_Msk			(0x07 << JPEG_CFG_OUTFMT_Pos)
#define JPEG_CFG_YUV2RGB_Pos		17		// YUV2RGB translation, 0 Disable, 1 Enable
#define JPEG_CFG_YUV2RGB_Msk		(0x01 << JPEG_CFG_YUV2RGB_Pos)
#define JPEG_CFG_UVSWAP_Pos			18		// UV output swap, 0 YUV, 1 YVU
#define JPEG_CFG_UVSWAP_Msk			(0x01 << JPEG_CFG_UVSWAP_Pos)
#define JPEG_CFG_RBSWAP_Pos			19		// RB output swap, 0 RGB, 1 BGR
#define JPEG_CFG_RBSWAP_Msk			(0x01 << JPEG_CFG_RBSWAP_Pos)
#define JPEG_CFG_EDSWAP_Pos			20		// endian swap, 0 XRGB, 1 BGRX
#define JPEG_CFG_EDSWAP_Msk			(0x01 << JPEG_CFG_EDSWAP_Pos)
#define JPEG_CFG_565DITH_Pos		21		// RGB565 dithering, 0 Disable, 1 Enable
#define JPEG_CFG_565DITH_Msk		(0x01 << JPEG_CFG_565DITH_Pos)

#define JPEG_CR_START_Pos			0		// JPEG DEC frame start, automatically clear when done
#define JPEG_CR_START_Msk			(0x01 << JPEG_CR_START_Pos)
#define JPEG_CR_RESTART_Pos			1		// Decoder stream buffering restart
#define JPEG_CR_RESTART_Msk			(0x01 << JPEG_CR_RESTART_Pos)
#define JPEG_CR_RESET_Pos			3		// JPEG DEC core reset, automatically clear
#define JPEG_CR_RESET_Msk			(0x01 << JPEG_CR_RESET_Pos)
#define JPEG_CR_REINTRV_Pos			4		// Restart interval marker enable
#define JPEG_CR_REINTRV_Msk			(0x01 << JPEG_CR_REINTRV_Pos)
#define JPEG_CR_LASTBUF_Pos			5		// Decoder last stream buffering
#define JPEG_CR_LASTBUF_Msk			(0x01 << JPEG_CR_LASTBUF_Pos)
#define JPEG_CR_QTAUTO_Pos			7		// JPEG quantization tables auto refresh enable
#define JPEG_CR_QTAUTO_Msk			(0x01 << JPEG_CR_QTAUTO_Pos)
#define JPEG_CR_HTAUTO_Pos			8		// JPEG huffman tables auto refresh enable
#define JPEG_CR_HTAUTO_Msk			(0x01 << JPEG_CR_HTAUTO_Pos)
#define JPEG_CR_QTCNT_Pos			9		// Number of quantization tables
#define JPEG_CR_QTCNT_Msk			(0x03 << JPEG_CR_QTCNT_Pos)
#define JPEG_CR_HTCNT_Pos			11		// Number of huffman tables
#define JPEG_CR_HTCNT_Msk			(0x01 << JPEG_CR_HTCNT_Pos)
#define JPEG_CR_CUCNT_Pos			12		// The number of CU in the restart interval
#define JPEG_CR_CUCNT_Msk			(0xFFFF<<JPEG_CR_CUCNT_Pos)
#define JPEG_CR_HALT_Pos			28		// decoding halt control bit
#define JPEG_CR_HALT_Msk			(0x01 << JPEG_CR_HALT_Pos)

#define JPEG_IR_IEDONE_Pos			0		// Frame processing finish interrupt enable
#define JPEG_IR_IEDONE_Msk			(0x01 << JPEG_IR_IEDONE_Pos)
#define JPEG_IR_IEEMPTY_Pos			2		// JPEG decoder stream buffering empty interrupt enable
#define JPEG_IR_IEEMPTY_Msk			(0x01 << JPEG_IR_IEEMPTY_Pos)
#define JPEG_IR_IEERROR_Pos			3		// JPEG error interrupt enable
#define JPEG_IR_IEERROR_Msk			(0x01 << JPEG_IR_IEERROR_Pos)
#define JPEG_IR_ICDONE_Pos			5		// Interrupt Clear, write-ongly
#define JPEG_IR_ICDONE_Msk			(0x01 << JPEG_IR_ICDONE_Pos)
#define JPEG_IR_ICEMPTY_Pos			7
#define JPEG_IR_ICEMPTY_Msk			(0x01 << JPEG_IR_ICEMPTY_Pos)
#define JPEG_IR_ICERROR_Pos			8
#define JPEG_IR_ICERROR_Msk			(0x01 << JPEG_IR_ICERROR_Pos)
#define JPEG_IR_IFDONE_Pos			10		// Interrupt Flag, read-only
#define JPEG_IR_IFDONE_Msk			(0x01 << JPEG_IR_IFDONE_Pos)
#define JPEG_IR_IFEMPTY_Pos			12
#define JPEG_IR_IFEMPTY_Msk			(0x01 << JPEG_IR_IFEMPTY_Pos)
#define JPEG_IR_IFERROR_Pos			13
#define JPEG_IR_IFERROR_Msk			(0x01 << JPEG_IR_IFERROR_Pos)

#define JPEG_SR_BUSY_Pos			0
#define JPEG_SR_BUSY_Msk			(0x01 << JPEG_SR_BUSY_Pos)
#define JPEG_SR_CUOVR_Pos			1		// JPEG CU run length overflow error
#define JPEG_SR_CUOVR_Msk			(0x01 << JPEG_SR_CUOVR_Pos)
#define JPEG_SR_REIMERR_Pos			2		// JPEG Restart interval marker error
#define JPEG_SR_REIMERR_Msk			(0x01 << JPEG_SR_REIMERR_Pos)
#define JPEG_SR_BUFBUSY_Pos			4		// JPEG decoder stream buffering status
#define JPEG_SR_BUFBUSY_Msk			(0x01 << JPEG_SR_BUFBUSY_Pos)
#define JPEG_SR_DMARDBUSY_Pos		5
#define JPEG_SR_DMARDBUSY_Msk		(0x01 << JPEG_SR_DMARDBUSY_Pos)
#define JPEG_SR_DMAWRBUSY_Pos		6
#define JPEG_SR_DMAWRBUSY_Msk		(0x01 << JPEG_SR_DMAWRBUSY_Pos)

#define JPEG_IMGSIZ_HPIX_Pos		0		// JPEG image width (minus 1)
#define JPEG_IMGSIZ_HPIX_Msk		(0x3FF<< JPEG_IMGSIZ_HPIX_Pos)
#define JPEG_IMGSIZ_VPIX_Pos		16		// JPEG image height (minus 1)
#define JPEG_IMGSIZ_VPIX_Msk		(0x3FF<< JPEG_IMGSIZ_VPIX_Pos)

#define JPEG_IMGSTR_RGBLINE_Pos		0		// number of words of RGB line width, XRGB888: img_width, RGB888: ceil(img_width*3/4), RGB565: ceil(img_width/2)
#define JPEG_IMGSTR_RGBLINE_Msk		(0x7FF<< JPEG_IMGSTR_RGBLINE_Pos)
#define JPEG_IMGSTR_YLINE_Pos		0		// number of words of Y line width, ceil(img_width/4)
#define JPEG_IMGSTR_YLINE_Msk		(0x7FF<< JPEG_IMGSTR_YLINE_Pos)
#define JPEG_IMGSTR_UVLINE_Pos		16		// number of words of UV line width, UV-planer 444: ceil(img_width/4)   UV-planar 420/422: ceil(img_width/8)
											//									 UV-semi planer 444: ceil(img_width/2)   UV-semi planer 420/422: ceil(img_width/4)
#define JPEG_IMGSTR_UVLINE_Msk		(0x7FF<< JPEG_IMGSTR_UVLINE_Pos)




/******************************************************************************/
/*						 Peripheral memory map							  */
/******************************************************************************/
#define RAM_BASE		   	0x20000000
#define AHB_BASE			0x40000000
#define APB1_BASE		 	0x40040000
#define APB2_BASE			0x400A0000

#define SFLASH_BASE			0x70000000	// Serial Flash

#define SDRAMM_BASE			0x80000000
#define SDRAMC_BASE			0x88000000



/* AHB Peripheral memory map */
#define SYS_BASE			(AHB_BASE + 0x00000)

#define DMA_BASE			(AHB_BASE + 0x00800)

#define SDIO_BASE			(AHB_BASE + 0x01800)

#define LCD_BASE		    (AHB_BASE + 0x02000)

#define CRC_BASE		    (AHB_BASE + 0x02800)

#define CORDIC_BASE			(AHB_BASE + 0x03000)

#define DIV_BASE			(AHB_BASE + 0x03800)

#define GPIOM_BASE			(AHB_BASE + 0x04000)
#define GPION_BASE			(AHB_BASE + 0x04800)

#define USBD_BASE			(AHB_BASE + 0x05000)
#define USBH_BASE			(AHB_BASE + 0x05000)

#define JPEG_BASE			(AHB_BASE + 0x0B000)
#define DMA2D_BASE			(AHB_BASE + 0x0C000)

/* APB Peripheral memory map */
#define GPIOA_BASE			(APB1_BASE + 0x00000)
#define GPIOB_BASE			(APB1_BASE + 0x00800)
#define GPIOC_BASE			(APB1_BASE + 0x01000)
#define GPIOD_BASE			(APB1_BASE + 0x01800)

#define UART0_BASE			(APB1_BASE + 0x02000)
#define UART1_BASE			(APB1_BASE + 0x02800)
#define UART2_BASE			(APB1_BASE + 0x03000)
#define UART3_BASE			(APB1_BASE + 0x03800)

#define SPI0_BASE			(APB1_BASE + 0x04000)
#define SPI1_BASE           (APB1_BASE + 0x04800)

#define PWM0_BASE			(APB1_BASE + 0x06000)
#define PWM1_BASE			(APB1_BASE + 0x06080)
#define PWM2_BASE			(APB1_BASE + 0x06100)
#define PWM3_BASE			(APB1_BASE + 0x06180)
#define PWM4_BASE			(APB1_BASE + 0x06200)
#define PWMG_BASE			(APB1_BASE + 0x06400)

#define TIMR0_BASE			(APB1_BASE + 0x06800)
#define TIMR1_BASE			(APB1_BASE + 0x06840)
#define TIMR2_BASE			(APB1_BASE + 0x06880)
#define TIMR3_BASE			(APB1_BASE + 0x068C0)
#define TIMR4_BASE			(APB1_BASE + 0x06900)
#define TIMRG_BASE			(APB1_BASE + 0x06C00)

#define BTIMR0_BASE			(APB1_BASE + 0x08800)
#define BTIMR1_BASE			(APB1_BASE + 0x08840)
#define BTIMR2_BASE			(APB1_BASE + 0x08880)
#define BTIMR3_BASE			(APB1_BASE + 0x088C0)
#define BTIMR4_BASE			(APB1_BASE + 0x08900)
#define BTIMR5_BASE			(APB1_BASE + 0x08940)
#define BTIMR6_BASE			(APB1_BASE + 0x08980)
#define BTIMR7_BASE			(APB1_BASE + 0x089C0)
#define BTIMR8_BASE			(APB1_BASE + 0x08A00)
#define BTIMR9_BASE			(APB1_BASE + 0x08A40)
#define BTIMR10_BASE		(APB1_BASE + 0x08A80)
#define BTIMR11_BASE		(APB1_BASE + 0x08AC0)
#define BTIMRG_BASE			(APB1_BASE + 0x08C00)

#define ADC0_BASE			(APB1_BASE + 0x09000)
#define ADC1_BASE			(APB1_BASE + 0x09800)

#define FMC_BASE			(APB1_BASE + 0x0A000)	// Flash Memory Controller

#define SFC_BASE			(APB1_BASE + 0x0A800)	// Serial Flash Controller

#define RTC_BASE			(APB1_BASE + 0x0B800)

#define DAC_BASE			(APB1_BASE + 0x0C000)

#define QEI_BASE			(APB1_BASE + 0x0C800)

#define PORTA_BASE			(APB2_BASE + 0x00000)
#define PORTB_BASE			(APB2_BASE + 0x00010)
#define PORTC_BASE			(APB2_BASE + 0x00020)
#define PORTD_BASE			(APB2_BASE + 0x00030)
#define PORTE_BASE			(APB2_BASE + 0x00040)
#define PORTM_BASE			(APB2_BASE + 0x00080)
#define PORTN_BASE			(APB2_BASE + 0x00090)

#define WDT_BASE			(APB2_BASE + 0x00800)

#define GPIOE_BASE			(APB2_BASE + 0x01000)

#define I2C0_BASE			(APB2_BASE + 0x06000)
#define I2C1_BASE			(APB2_BASE + 0x06800)

#define CAN0_BASE			(APB2_BASE + 0x08000)
#define CAN1_BASE			(APB2_BASE + 0x08800)



/******************************************************************************/
/*						 Peripheral declaration							 */
/******************************************************************************/
#define SYS					((SYS_TypeDef  *) SYS_BASE)

#define PORTA				((PORT_TypeDef *) PORTA_BASE)
#define PORTB				((PORT_TypeDef *) PORTB_BASE)
#define PORTC				((PORT_TypeDef *) PORTC_BASE)
#define PORTD				((PORT_TypeDef *) PORTD_BASE)
#define PORTE				((PORT_TypeDef *) PORTE_BASE)
#define PORTM				((PORT_TypeDef *) PORTM_BASE)
#define PORTN				((PORT_TypeDef *) PORTN_BASE)

#define GPIOA				((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB				((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC				((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD				((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE				((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOM				((GPIO_TypeDef *) GPIOM_BASE)
#define GPION				((GPIO_TypeDef *) GPION_BASE)

#define TIMR0				((TIMR_TypeDef *) TIMR0_BASE)
#define TIMR1				((TIMR_TypeDef *) TIMR1_BASE)
#define TIMR2				((TIMR_TypeDef *) TIMR2_BASE)
#define TIMR3				((TIMR_TypeDef *) TIMR3_BASE)
#define TIMR4				((TIMR_TypeDef *) TIMR4_BASE)
#define TIMRG				((TIMRG_TypeDef*) TIMRG_BASE)

#define BTIMR0				((TIMR_TypeDef *) BTIMR0_BASE)
#define BTIMR1				((TIMR_TypeDef *) BTIMR1_BASE)
#define BTIMR2				((TIMR_TypeDef *) BTIMR2_BASE)
#define BTIMR3				((TIMR_TypeDef *) BTIMR3_BASE)
#define BTIMR4				((TIMR_TypeDef *) BTIMR4_BASE)
#define BTIMR5				((TIMR_TypeDef *) BTIMR5_BASE)
#define BTIMR6				((TIMR_TypeDef *) BTIMR6_BASE)
#define BTIMR7				((TIMR_TypeDef *) BTIMR7_BASE)
#define BTIMR8				((TIMR_TypeDef *) BTIMR8_BASE)
#define BTIMR9				((TIMR_TypeDef *) BTIMR9_BASE)
#define BTIMR10				((TIMR_TypeDef *) BTIMR10_BASE)
#define BTIMR11				((TIMR_TypeDef *) BTIMR11_BASE)
#define BTIMRG				((TIMRG_TypeDef*) BTIMRG_BASE)

#define UART0				((UART_TypeDef *) UART0_BASE)
#define UART1				((UART_TypeDef *) UART1_BASE)
#define UART2				((UART_TypeDef *) UART2_BASE)
#define UART3   			((UART_TypeDef *) UART3_BASE)

#define SPI0				((SPI_TypeDef  *) SPI0_BASE)
#define SPI1				((SPI_TypeDef  *) SPI1_BASE)

#define I2C0				((I2C_TypeDef  *) I2C0_BASE)
#define I2C1				((I2C_TypeDef  *) I2C1_BASE)

#define ADC0 				((ADC_TypeDef  *) ADC0_BASE)
#define ADC1 				((ADC_TypeDef  *) ADC1_BASE)

#define PWM0				((PWM_TypeDef  *) PWM0_BASE)
#define PWM1				((PWM_TypeDef  *) PWM1_BASE)
#define PWM2				((PWM_TypeDef  *) PWM2_BASE)
#define PWM3				((PWM_TypeDef  *) PWM3_BASE)
#define PWM4				((PWM_TypeDef  *) PWM4_BASE)
#define PWMG				((PWMG_TypeDef *) PWMG_BASE)

#define CAN0 				((CAN_TypeDef  *) CAN0_BASE)
#define CAN1				((CAN_TypeDef  *) CAN1_BASE)

#define USBD				((USBD_TypeDef *) USBD_BASE)
#define USBH				((USBH_TypeDef *) USBH_BASE)

#define SDIO				((SDIO_TypeDef *) SDIO_BASE)

#define SDRAMC				((SDRAMC_TypeDef*)SDRAMC_BASE)

#define DMA 				((DMA_TypeDef  *) DMA_BASE)

#define LCD					((LCD_TypeDef  *) LCD_BASE)

#define DMA2D				((DMA2D_TypeDef*) DMA2D_BASE)

#define DAC					((DAC_TypeDef  *) DAC_BASE)

#define CRC					((CRC_TypeDef  *) CRC_BASE)

#define CORDIC				((CORDIC_TypeDef*)CORDIC_BASE)

#define DIV					((DIV_TypeDef  *) DIV_BASE)

#define RTC					((RTC_TypeDef  *) RTC_BASE)

#define WDT					((WDT_TypeDef  *) WDT_BASE)

#define QEI					((QEI_TypeDef  *) QEI_BASE)

#define FMC					((FMC_TypeDef  *) FMC_BASE)

#define SFC					((SFC_TypeDef  *) SFC_BASE)

#define JPEG				((JPEG_TypeDef *) JPEG_BASE)



#include "SWM341_port.h"
#include "SWM341_gpio.h"
#include "SWM341_exti.h"
#include "SWM341_timr.h"
#include "SWM341_uart.h"
#include "SWM341_spi.h"
#include "SWM341_i2c.h"
#include "SWM341_adc.h"
#include "SWM341_pwm.h"
#include "SWM341_dma.h"
#include "SWM341_can.h"
#include "SWM341_sdio.h"
#include "SWM341_sdram.h"
#include "SWM341_flash.h"
#include "SWM341_lcd.h"
#include "SWM341_sfc.h"
#include "SWM341_dac.h"
#include "SWM341_cordic.h"
#include "SWM341_crc.h"
#include "SWM341_div.h"
#include "SWM341_rtc.h"
#include "SWM341_wdt.h"
#include "SWM341_qei.h"
#include "SWM341_usb.h"
#include "SWM341_usbd.h"
#include "SWM341_usbh.h"
#include "SWM341_jpeg.h"
#include "SWM341_dma2d.h"
#include "SWM341_iofilt.h"


#ifdef  SW_LOG_RTT
#define log_printf(...)	 	SEGGER_RTT_printf(0, __VA_ARGS__)
#else
#define log_printf(...)	 	printf(__VA_ARGS__)
#endif


#ifndef SW_LOG_LEVEL
#define SW_LOG_LEVEL        0
#endif

#if (SW_LOG_LEVEL > 0)
#define SW_LOG_ERR(...)    	{						 \
							log_printf("ERROR: ");   \
							log_printf(__VA_ARGS__); \
							log_printf("\n");		 \
							}

#if (SW_LOG_LEVEL > 1)
#define SW_LOG_WARN(...) 	{						 \
							log_printf("WARN : ");   \
							log_printf(__VA_ARGS__); \
							log_printf("\n");		 \
							}

#if (SW_LOG_LEVEL > 2)
#define SW_LOG_INFO(...)   	{						 \
							log_printf("INFO : ");   \
							log_printf(__VA_ARGS__); \
							log_printf("\n");		 \
							}
#else
#define SW_LOG_INFO(...)
#endif

#else
#define SW_LOG_WARN(...)
#define SW_LOG_INFO(...)
#endif

#else
#define SW_LOG_ERR(...)
#define SW_LOG_WARN(...)
#define SW_LOG_INFO(...)
#endif


#endif //__SWM341_H__
