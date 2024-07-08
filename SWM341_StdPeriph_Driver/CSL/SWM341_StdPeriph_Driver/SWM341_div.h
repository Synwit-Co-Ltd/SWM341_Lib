#ifndef __SWM341_DIV_H__
#define __SWM341_DIV_H__


void DIV_Init(DIV_TypeDef * DIVx);


/*******************************************************************************************************************************
* @brief	DIV perform unsigned number division operation
* @param	dividend
* @param	divisor
* @return
*******************************************************************************************************************************/
static __INLINE void DIV_UDiv(uint32_t dividend, uint32_t divisor)
{
	DIV->DIVIDEND = dividend;
	DIV->DIVISOR = divisor;
	
	DIV->CR = (1 << DIV_CR_DIVSIGN_Pos) | (1 << DIV_CR_DIVGO_Pos);
}

/*******************************************************************************************************************************
* @brief	DIV perform signed number division operation
* @param	dividend
* @param	divisor
* @return
*******************************************************************************************************************************/
static __INLINE void DIV_SDiv(int32_t dividend, int32_t divisor)
{
	DIV->DIVIDEND = dividend;
	DIV->DIVISOR = divisor;
	
	DIV->CR = (0 << DIV_CR_DIVSIGN_Pos) | (1 << DIV_CR_DIVGO_Pos);
}

/*******************************************************************************************************************************
* @brief	whether DIV is performing division operation
* @param
* @return	1 DIV is busy, 0 operation completed, can read result out
*******************************************************************************************************************************/
static __INLINE uint32_t DIV_Div_IsBusy(void)
{
	return (DIV->SR & DIV_SR_DIVBUSY_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	read unsigned number division result
* @param	quotient is used to save quotient of division operation
* @param	remainder is used to save remainder of division operation
* @return
*******************************************************************************************************************************/
static __INLINE void DIV_UDiv_Result(uint32_t *quotient, uint32_t *remainder)
{
	*quotient = DIV->QUO;
	*remainder = DIV->REMAIN;
}

/*******************************************************************************************************************************
* @brief	read signed number division result
* @param	quotient is used to save quotient of division operation
* @param	remainder is used to save remainder of division operation
* @return
*******************************************************************************************************************************/
static __INLINE void DIV_SDiv_Result(int32_t *quotient, int32_t *remainder)
{
	*quotient = DIV->QUO & 0x7FFFFFFF;
	if(DIV->QUO & (1u << 31)) *quotient = 0 - *quotient;
	
	*remainder = DIV->REMAIN & 0x7FFFFFFF;
	if(DIV->REMAIN & (1u << 31)) *remainder = 0 - *remainder;
}

/*******************************************************************************************************************************
* @brief	DIV perform root operation
* @param	radicand
* @param	calcu_frac specify result format, 0 result is a 16-bit integer, 1 result is 16-bit integer and 16-bit decimal place
* @return
*******************************************************************************************************************************/
static __INLINE void DIV_Root(uint32_t radicand, uint32_t calcu_fractional)
{
	DIV->RADICAND = radicand;
	
	DIV->CR = (1 << DIV_CR_ROOTGO_Pos) | (calcu_fractional << DIV_CR_ROOTMOD_Pos);
}

/*******************************************************************************************************************************
* @brief	whether DIV is performing root operation
* @param
* @return	1 DIV is busy, 0 operation completed, can read result out
*******************************************************************************************************************************/
static __INLINE uint32_t DIV_Root_IsBusy(void)
{
	return (DIV->SR & DIV_SR_ROOTBUSY_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	read root result
* @param
* @return	root result
*******************************************************************************************************************************/
static __INLINE uint32_t DIV_Root_Result(void)
{
	if(DIV->CR & DIV_CR_ROOTMOD_Msk)
	{
		return DIV->ROOT;
	}
	else
	{
		return DIV->ROOT >> 16;
	}
}


#endif
