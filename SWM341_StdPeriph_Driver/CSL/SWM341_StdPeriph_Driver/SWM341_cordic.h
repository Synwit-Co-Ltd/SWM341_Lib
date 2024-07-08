#ifndef __SWM341_CORDIC_H__
#define __SWM341_CORDIC_H__


#define CORDIC_PI	3.141592653589793


void CORDIC_Init(CORDIC_TypeDef * CORDICx);


/*******************************************************************************************************************************
* @brief	compute the sin of specified radian
* @param	radian value range is 0.01~1.56, but it must be multiplied by 16384 when passed in as an argument
* @return
*******************************************************************************************************************************/
static __INLINE void CORDIC_Sin(uint32_t radian)
{
	CORDIC->INPUT = radian;
	
	CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (1 << CORDIC_CMD_SINCOS_Pos);
}

/*******************************************************************************************************************************
* @brief	whether the sin compute done
* @param
* @return	1 compute done, can read compute result out, 0 compute not done
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Sin_IsDone(void)
{
	return (CORDIC->SIN & CORDIC_SIN_DONE_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	read the sin compute result
* @param
* @return	sin compute result, Result[15:14] is the integer part, and Result[13:0] is the decimal part
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Sin_Result(void)
{
	return CORDIC->SIN & (CORDIC_SIN_F_Msk | CORDIC_SIN_I_Msk);
}

/*******************************************************************************************************************************
* @brief	compute the cos of specified radian
* @param	radian value range is 0.01~1.56, but it must be multiplied by 16384 when passed in as an argument
* @return
*******************************************************************************************************************************/
static __INLINE void CORDIC_Cos(uint32_t radian)
{
	CORDIC->INPUT = radian;
	
	CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (1 << CORDIC_CMD_SINCOS_Pos);
}

/*******************************************************************************************************************************
* @brief	whether the cos compute done
* @param
* @return	1 compute done, can read compute result out, 0 compute not done
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Cos_IsDone(void)
{
	return (CORDIC->COS & CORDIC_COS_DONE_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	read the cos compute result
* @param
* @return	cos compute result, Result[15:14] is the integer part, and Result[13:0] is the decimal part
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Cos_Result(void)
{
	return CORDIC->COS & (CORDIC_COS_F_Msk | CORDIC_COS_I_Msk);
}

/*******************************************************************************************************************************
* @brief	compute the arctan of specified input
* @param	input value range is 0.05~10000, but it must be multiplied by 16384 when passed in as an argument
* @return
*******************************************************************************************************************************/
static __INLINE void CORDIC_Arctan(uint32_t input)
{
	if((input > 819) && (input <= 8192))
	{
		CORDIC->INPUT = input * 2;
		
		CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (0 << CORDIC_CMD_SINCOS_Pos) | (0 << CORDIC_CMD_RANGE_Pos);
	}
	else if((input > 8192) && (input <= 32768))
	{
		CORDIC->INPUT = input;
		
		CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (0 << CORDIC_CMD_SINCOS_Pos) | (1 << CORDIC_CMD_RANGE_Pos);
	}
	else if((input > 32768) && (input < 10000*16384))
	{
		CORDIC->INPUT = 32768*16384/input;
		
		CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (0 << CORDIC_CMD_SINCOS_Pos) | (2 << CORDIC_CMD_RANGE_Pos);
	}
}

/*******************************************************************************************************************************
* @brief	whether the arctan compute done
* @param
* @return	1 compute done, can read compute result out, 0 compute not done
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Arctan_IsDone(void)
{
	return (CORDIC->ARCTAN & CORDIC_ARCTAN_DONE_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	read the arctan compute result
* @param
* @return	arctan compute result, Result[15:14] is the integer part, and Result[13:0] is the decimal part
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Arctan_Result(void)
{
	return CORDIC->ARCTAN & (CORDIC_ARCTAN_F_Msk | CORDIC_ARCTAN_I_Msk);
}

/*******************************************************************************************************************************
* @brief	compute the product of specified input
* @param	mul1 value range is [1, 2) , but it must be multiplied by 16384 when passed in as an argument
* @param	mul2 value range is [1, 2) , but it must be multiplied by 16384 when passed in as an argument
* @return
*******************************************************************************************************************************/
static __INLINE void CORDIC_Mul(uint16_t mul1, uint16_t mul2)
{
	CORDIC->INPUT = mul1 | (mul2 << 16);
	
	CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (3 << CORDIC_CMD_MULDIV_Pos);
}

/*******************************************************************************************************************************
* @brief	whether the multiply compute done
* @param
* @return	1 compute done, can read compute result out, 0 compute not done
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Mul_IsDone(void)
{
	return (CORDIC->SIN & CORDIC_SIN_DONE_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	read the multiply compute result
* @param
* @return	multiply compute result, Result[15:14] is the integer part, and Result[13:0] is the decimal part
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Mul_Result(void)
{
	return CORDIC->SIN & (CORDIC_SIN_F_Msk | CORDIC_SIN_I_Msk);
}

/*******************************************************************************************************************************
* @brief	compute the quotient of specified input
* @param	dividend value range is [1, 2) , but it must be multiplied by 16384 when passed in as an argument
* @param	divisor value range is [1, 2) , but it must be multiplied by 16384 when passed in as an argument
* @return
*******************************************************************************************************************************/
static __INLINE void CORDIC_Div(uint16_t dividend, uint16_t divisor)
{
	CORDIC->INPUT = divisor | (dividend << 16);
	
	CORDIC->CMD = (1 << CORDIC_CMD_START_Pos) | (2 << CORDIC_CMD_MULDIV_Pos);
}

/*******************************************************************************************************************************
* @brief	whether the division compute done
* @param
* @return	1 compute done, can read compute result out, 0 compute not done
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Div_IsDone(void)
{
	return (CORDIC->ARCTAN & CORDIC_ARCTAN_DONE_Msk) ? 1 : 0;
}

/*******************************************************************************************************************************
* @brief	read the division compute result
* @param
* @return	division compute result, Result[15:14] is the integer part, and Result[13:0] is the decimal part
*******************************************************************************************************************************/
static __INLINE uint32_t CORDIC_Div_Result(void)
{
	return CORDIC->ARCTAN & (CORDIC_ARCTAN_F_Msk | CORDIC_ARCTAN_I_Msk);
}


#endif
