/*******************************************************************************************************************************
* @brief	USB host CDC driver
*
*
********************************************************************************************************************************
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS WITH CODING INFORMATION 
* REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME. AS A RESULT, SYNWIT SHALL NOT BE HELD LIABLE 
* FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT 
* OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONN-
* -ECTION WITH THEIR PRODUCTS.
*
* COPYRIGHT 2012 Synwit Technology 
*******************************************************************************************************************************/
#include "SWM341.h"
#include "usbh_core.h"
#include "usbh_cdc_core.h"
#include "usbh_cdc_funct.h"


/* wValue for SetControlLineState */
#define CDC_RTS_ACTIVATE		0x0002
#define CDC_RTS_DEACTIVATE		0x0000
#define CDC_DTR_ACTIVATE		0x0001
#define CDC_DTR_DEACTIVATE		0x0000


CDC_LineCoding_t CDC_GetLineCode;
CDC_LineCoding_t CDC_SetLineCode;


/*******************************************************************************************************************************
* @brief	USB host get line coding
* @param
* @return
*******************************************************************************************************************************/
USBH_Status USBH_CDC_GetLineCoding(USBH_Info_t *phost)
{
	phost->Ctrl.setup.bRequestType = USB_REQ_D2H | USB_REQ_CLASS | USB_REQ_TO_INTERFACE;
	phost->Ctrl.setup.bRequest = CDC_GET_LINE_CODING;
	phost->Ctrl.setup.wValue = 0;
	phost->Ctrl.setup.wIndex = 0;
	phost->Ctrl.setup.wLength = sizeof(CDC_LineCoding_t);
	
	return USBH_CtrlTransfer(phost, (uint8_t *)&CDC_GetLineCode, sizeof(CDC_LineCoding_t));
}

/*******************************************************************************************************************************
* @brief	USB host set line coding
* @param
* @return
*******************************************************************************************************************************/
USBH_Status USBH_CDC_SetLineCoding(USBH_Info_t *phost)
{
	phost->Ctrl.setup.bRequestType = USB_REQ_H2D | USB_REQ_CLASS | USB_REQ_TO_INTERFACE;
	phost->Ctrl.setup.bRequest = CDC_SET_LINE_CODING;
	phost->Ctrl.setup.wValue = 0;
	phost->Ctrl.setup.wIndex = 0;
	phost->Ctrl.setup.wLength = sizeof(CDC_LineCoding_t);

	return USBH_CtrlTransfer(phost, (uint8_t *)&CDC_SetLineCode , sizeof(CDC_LineCoding_t));
}

/*******************************************************************************************************************************
* @brief	USB host set control line
* @param
* @return
*******************************************************************************************************************************/
USBH_Status USBH_CDC_SetControlLine(USBH_Info_t *phost)
{
	phost->Ctrl.setup.bRequestType = USB_REQ_H2D | USB_REQ_CLASS | USB_REQ_TO_INTERFACE;
	phost->Ctrl.setup.bRequest = CDC_SET_CONTROL_LINE;
	phost->Ctrl.setup.wValue = CDC_RTS_DEACTIVATE | CDC_DTR_DEACTIVATE;
	phost->Ctrl.setup.wIndex = 0;
	phost->Ctrl.setup.wLength = 0;

	return USBH_CtrlTransfer(phost, 0 , 0);
}

/*******************************************************************************************************************************
* @brief	USB host issue set line coding
* @param
* @return
*******************************************************************************************************************************/
void USBH_CDC_IssueSetLineCoding(USBH_Info_t *phost)
{
	USBH_CDC_Info.reqState = CDC_SET_LINE_CODING;
	
	phost->StateBkp = phost->State;
	phost->State = HOST_CLASS_REQUEST;
}

/*******************************************************************************************************************************
* @brief	USB host issue get line coding
* @param
* @return
*******************************************************************************************************************************/
void USBH_CDC_IssueGetLineCoding(USBH_Info_t *phost)
{
	USBH_CDC_Info.reqState = CDC_GET_LINE_CODING;
	
	phost->StateBkp =  phost->State;
	phost->State = HOST_CLASS_REQUEST;
}
