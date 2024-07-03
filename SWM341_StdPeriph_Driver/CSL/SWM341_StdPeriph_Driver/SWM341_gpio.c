/*******************************************************************************************************************************
* @brief	GPIO driver
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
#include "SWM341_gpio.h"


/*******************************************************************************************************************************
* @brief	GPIO pin init
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @param	dir specify pin direction
* @param	pull_up is pull up enable
* @param	pull_down is pull down enable
* @param	open_drain is open drain enable
* @return
*******************************************************************************************************************************/
void GPIO_Init(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t dir, uint32_t pull_up, uint32_t pull_down, uint32_t open_drain)
{
	PORT_TypeDef * PORTx = PORTA;
	
	switch((uint32_t)GPIOx)
	{
	case ((uint32_t)GPIOA):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_GPIOA_Pos);
		
		PORTx = PORTA;
		break;
	
	case ((uint32_t)GPIOB):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_GPIOB_Pos);
		
		PORTx = PORTB;
		break;
	
	case ((uint32_t)GPIOC):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_GPIOC_Pos);
		
		PORTx = PORTC;
		break;
	
	case ((uint32_t)GPIOD):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_GPIOD_Pos);
		
		PORTx = PORTD;
		break;
	
	case ((uint32_t)GPIOE):
		SYS->CLKEN1 |= (0x01 << SYS_CLKEN1_GPIOE_Pos);
		
		PORTx = PORTE;
		break;
		
	case ((uint32_t)GPIOM):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_GPIOM_Pos);
		
		PORTx = PORTM;
		break;
		
	case ((uint32_t)GPION):
		SYS->CLKEN0 |= (0x01 << SYS_CLKEN0_GPION_Pos);
		
		PORTx = PORTN;
		break;
	}
	
	PORT_Init(PORTx, n, 0, 1);
	if(dir == 1)
	{
		GPIOx->DIR |= (0x01 << n);
	}
	else
	{
		GPIOx->DIR &= ~(0x01 << n);
	}
	
	if(pull_up == 1)     PORTx->PULLU |=  (1 << n);
	else                 PORTx->PULLU &= ~(1 << n);
	if(pull_down == 1)   PORTx->PULLD |=  (1 << n);
	else                 PORTx->PULLD &= ~(1 << n);
	if(open_drain == 1)  PORTx->OPEND |=  (1 << n);
	else                 PORTx->OPEND &= ~(1 << n);
}

/*******************************************************************************************************************************
* @brief	set specify pin to specify level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @param	v is level to set
* @return
*******************************************************************************************************************************/
void GPIO_WriteBit(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t v)
{
	*(&GPIOx->DATAPIN0 + n) = v;
}

/*******************************************************************************************************************************
* @brief	set specify pin to high level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @return
*******************************************************************************************************************************/
void GPIO_SetBit(GPIO_TypeDef * GPIOx, uint32_t n)
{
	*(&GPIOx->DATAPIN0 + n) = 1;
}

/*******************************************************************************************************************************
* @brief	clear specify pin to low level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @return
*******************************************************************************************************************************/
void GPIO_ClrBit(GPIO_TypeDef * GPIOx, uint32_t n)
{
	*(&GPIOx->DATAPIN0 + n) = 0;
}

/*******************************************************************************************************************************
* @brief	invert specify pin level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @return
*******************************************************************************************************************************/
void GPIO_InvBit(GPIO_TypeDef * GPIOx, uint32_t n)
{
	*(&GPIOx->DATAPIN0 + n) = 1 - *(&GPIOx->DATAPIN0 + n);
}

/*******************************************************************************************************************************
* @brief	get specify pin level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @return	0 low level, 1 high level
*******************************************************************************************************************************/
uint32_t GPIO_GetBit(GPIO_TypeDef * GPIOx, uint32_t n)
{
	return *(&GPIOx->DATAPIN0 + n);
}

/*******************************************************************************************************************************
* @brief	set specify pins (by start pin and pin count) to specify level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @param	w is pin count to set
* @param	v is level to set
* @return
*******************************************************************************************************************************/
void GPIO_WriteBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w, uint32_t v)
{
	uint32_t bits = 0xFFFF >> (16 - w);
	
	GPIOx->ODR &=~(bits << n);
	GPIOx->ODR |= (v << n);
}

/*******************************************************************************************************************************
* @brief	set specify pins (by start pin and pin count) to high level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @param	w is pin count to set
* @return
*******************************************************************************************************************************/
void GPIO_SetBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w)
{
	uint32_t bits = 0xFFFF >> (16 - w);
	
	GPIOx->ODR |= (bits << n);
}

/*******************************************************************************************************************************
* @brief	clear specify pins (by start pin and pin count) to low level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @param	w is pin count to set
* @return
*******************************************************************************************************************************/
void GPIO_ClrBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w)
{
	uint32_t bits = 0xFFFF >> (16 - w);
	
	GPIOx->ODR &= ~(bits << n);
}

/*******************************************************************************************************************************
* @brief	invert specify pins (by start pin and pin count) level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @param	w is pin count to set
* @return
*******************************************************************************************************************************/
void GPIO_InvBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w)
{
	uint32_t bits = 0xFFFF >> (16 - w);
	
	GPIOx->ODR ^= (bits << n);
}

/*******************************************************************************************************************************
* @brief	get specify pins (by start pin and pin count) level
* @param	GPIOx is port of pin
* @param	n is pin number in port
* @param	w is pin count to set
* @return	pin level, 1 bit per pin
*******************************************************************************************************************************/
uint32_t GPIO_GetBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w)
{
	uint32_t bits = 0xFFFF >> (16 - w);
	
	return ((GPIOx->IDR >> n) & bits);
}
