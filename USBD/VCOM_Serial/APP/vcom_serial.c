#include <string.h>
#include "SWM341.h"
#include "vcom_serial.h"

extern uint8_t  USBD_DeviceDescriptor[];
extern uint8_t  USBD_ConfigDescriptor[];
extern uint8_t *USBD_StringDescriptor[];

void VCOM_Init(void)
{
	UART_InitStructure UART_initStruct;
	
	USBD_Info.Mode = USBD_MODE_DEV;
	USBD_Info.Speed = USBD_SPEED_FS;
	USBD_Info.CtrlPkSiz = EP_CTRL_PKSZ;
	USBD_Info.DescDevice = USBD_DeviceDescriptor;
	USBD_Info.DescConfig = USBD_ConfigDescriptor;
	USBD_Info.DescString = USBD_StringDescriptor;
	USBD_Info.pClassRequest_Callback = VCOM_ClassRequest;
	USBD_Init();
	
	
	PORT_Init(PORTM, PIN0, PORTM_PIN0_UART0_RX, 1);
 	PORT_Init(PORTM, PIN1, PORTM_PIN1_UART0_TX, 0);
 	
 	UART_initStruct.Baudrate = 57600;
	UART_initStruct.DataBits = UART_DATA_8BIT;
	UART_initStruct.Parity = UART_PARITY_NONE;
	UART_initStruct.StopBits = UART_STOP_1BIT;
	UART_initStruct.RXThreshold = 3;
	UART_initStruct.RXThresholdIEn = 1;
	UART_initStruct.TXThreshold = 3;
	UART_initStruct.TXThresholdIEn = 0;
	UART_initStruct.TimeoutTime = 10;		//10个字符时间内未接收到新的数据则触发超时中断
	UART_initStruct.TimeoutIEn = 1;
 	UART_Init(UART0, &UART_initStruct);
	UART_Open(UART0);
}


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
		
		USBD_RxReady(EP_BULK_OUT_NUM);
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
        if(epif & USBD_EPIF_INEP0_Msk)					// Ctrl IN
        {
			if(USBD_TxSuccess(0))
			{
				USBD_CtrlIn();
			}
			USBD_TxIntClr(0);
        }
        else if(epif & USBD_EPIF_OUTEP0_Msk)			// Ctrl OUT
        {
			USBD_RxIntClr();
			if(USBD_RxSuccess())
			{
				if(pUSB_Setup->bRequest == SET_LINE_CODE)
				{
					USBD_CtrlOut();
					
					USBD_TxWrite(0, 0, 0);	// Status stage
					
					VCOM_SetLineCoding();
				}
			}
        }
		else if(epif & (1 << EP_INT_IN_NUM))			// Int IN
        {
			USBD_TxIntClr(EP_INT_IN_NUM);
        }
        else if(epif & (1 << EP_BULK_IN_NUM))			// Bulk IN
        {
			if(USBD_TxSuccess(EP_BULK_IN_NUM) )
			{
				vcom.in_bytes = 0;
			}
			USBD_TxIntClr(EP_BULK_IN_NUM);
        }
        else if(epif & (1 << (EP_BULK_OUT_NUM + 16)))	// Bulk OUT
        {
			USBD_RxIntClr();
			if(USBD_RxSuccess())
			{
				vcom.out_bytes = USBD_RxRead((uint8_t *)vcom.out_buff, 64);

				vcom.out_ready = 1;
			}
        }
    }
}


void VCOM_ClassRequest(USB_Setup_Packet_t * pSetup)
{
    if(pSetup->bRequestType & 0x80)		// Device to host
    {
        switch(pSetup->bRequest)
        {
        case GET_LINE_CODE:
			/* Data stage */
			USBD_TxWrite(0, (uint8_t *)&LineCfg, sizeof(LineCfg));

            /* Status stage */
			USBD_RxReady(0);
            break;

        default:
            /* Setup error, stall the device */
            USBD_Stall0();
            break;
        }
    }
    else								// Host to device
    {
        switch(pSetup->bRequest)
        {
        case SET_CONTROL_LINE_STATE:
			vcom.hw_flow = pSetup->wValue;	// hw_flow.0 DTR   hw_flow.1 RTS

            /* Status stage */
			USBD_TxWrite(0, 0, 0);
            break;

        case SET_LINE_CODE:
			USBD_PrepareCtrlOut((uint8_t *)&LineCfg, 7);

            /* Status stage
			   读取数据再后执行 USBD_TxWrite(0, 0, 0)，否则主机可能会发来新的数据覆盖旧数据 */
            break;

        default:
            /* Setup error, stall the device */
            USBD_Stall0();
            break;
        }
    }
}


/****************************************************************************************************************************************/
volatile VCOM vcom;


STR_VCOM_LINE_CODING LineCfg = {57600, 0, 0, 8};   // Baud rate : 57600, stop bits, parity bits, data bits


void VCOM_SetLineCoding(void)
{
    uint8_t parity;

    NVIC_DisableIRQ(UART0_IRQn);
	
	// Reset software FIFO
	vcom.rx_bytes = 0;
	vcom.rx_head = 0;
	vcom.rx_tail = 0;

	vcom.tx_bytes = 0;
	vcom.tx_head = 0;
	vcom.tx_tail = 0;
	
	switch(LineCfg.u8ParityType)
	{
	case 0:  parity = UART_PARITY_NONE; break;
	case 1:  parity = UART_PARITY_ODD;  break;
	case 2:  parity = UART_PARITY_EVEN; break;
	case 3:  parity = UART_PARITY_ONE;  break;
	case 4:  parity = UART_PARITY_ZERO; break;
	default: parity = UART_PARITY_NONE;	break;
	}
	
	UART0->CTRL &= ~(UART_CTRL_DATA9b_Msk | UART_CTRL_PARITY_Msk | UART_CTRL_STOP2b_Msk);
	UART0->CTRL |= ((LineCfg.u8DataBits == 9)   << UART_CTRL_DATA9b_Pos) |
				   (parity                      << UART_CTRL_PARITY_Pos) |
				   ((LineCfg.u8CharFormat == 2) << UART_CTRL_STOP2b_Pos);
	
	UART_SetBaudrate(UART0, LineCfg.u32DTERate);
	
	NVIC_EnableIRQ(UART0_IRQn);
}

static volatile uint32_t RX_Timeout = 0;

void VCOM_TransferData(void)
{
    int32_t i, len;

    /* Check whether USB is ready for next packet or not */
    if(vcom.in_bytes == 0)
    {
        /* Check whether we have new COM Rx data to send to USB or not */
        if((vcom.rx_bytes >= EP_BULK_IN_PKSZ) || RX_Timeout)
        {
			RX_Timeout = 0;
			
            len = vcom.rx_bytes;
            if(len > EP_BULK_IN_PKSZ)
                len = EP_BULK_IN_PKSZ;

            for(i = 0; i < len; i++)
            {
                vcom.in_buff[i] = vcom.rx_buff[vcom.rx_head++];
                if(vcom.rx_head >= RX_BUFF_SIZE)
                    vcom.rx_head = 0;
            }

			__disable_irq();
            vcom.rx_bytes -= len;
            __enable_irq();

            vcom.in_bytes = len;
            USBD_TxWrite(EP_BULK_IN_NUM, (uint8_t *)vcom.in_buff, len);
        }
        else
        {
            /* Prepare a zero packet if previous packet size is EP_BULK_IN_PKSZ and
               no more data to send at this moment to note Host the transfer has been done */
            if(USBD->INEP[EP_BULK_IN_NUM].TXTRSZ == EP_BULK_IN_PKSZ)
                USBD_TxWrite(EP_BULK_IN_NUM, 0, 0);
        }
    }

    /* Process the Bulk out data when bulk out data is ready. */
    if(vcom.out_ready && (vcom.out_bytes <= TX_BUFF_SIZE - vcom.tx_bytes))
    {
        for(i = 0; i < vcom.out_bytes; i++)
        {
            vcom.tx_buff[vcom.tx_tail++] = vcom.out_buff[i];
            if(vcom.tx_tail >= TX_BUFF_SIZE)
                vcom.tx_tail = 0;
        }

        __disable_irq();
        vcom.tx_bytes += vcom.out_bytes;
		__enable_irq();

        vcom.out_ready = 0; 	// Clear bulk out ready flag

        /* Ready to get next BULK out */
		USBD_RxReady(EP_BULK_OUT_NUM);
    }

    /* Process the software Tx FIFO */
    if(vcom.tx_bytes)
    {
        while(!UART_IsTXFIFOFull(UART0))
		{
			UART0->DATA = vcom.tx_buff[vcom.tx_head++];
			if(vcom.tx_head >= TX_BUFF_SIZE)
				vcom.tx_head = 0;
			vcom.tx_bytes--;
			
			if(vcom.tx_bytes == 0)
				break;
		}
    }
}


static void putOneChar(char chr)
{
	if(vcom.rx_bytes < RX_BUFF_SIZE)  	// Check if buffer full
	{
		vcom.rx_buff[vcom.rx_tail++] = chr;
		if(vcom.rx_tail >= RX_BUFF_SIZE)
			vcom.rx_tail = 0;
		vcom.rx_bytes++;
	}
}

void UART0_Handler(void)
{
    uint32_t chr;
	
	if(UART_INTStat(UART0, UART_IT_RX_THR | UART_IT_RX_TOUT))
	{
		while(UART_IsRXFIFOEmpty(UART0) == 0)
		{
			UART_ReadByte(UART0, &chr);
			putOneChar(chr);
		}
		
		if(UART_INTStat(UART0, UART_IT_RX_TOUT))
		{
			UART_INTClr(UART0, UART_IT_RX_TOUT);
			
			RX_Timeout = 1;
		}
	}
}
