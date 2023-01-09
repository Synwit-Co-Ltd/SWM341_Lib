#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__

#include "SWM341.h"

#define CPU_CFG_NVIC_PRIO_BITS    __NVIC_PRIO_BITS
#define CPU_CFG_KA_IPL_BOUNDARY	  (1 << (__NVIC_PRIO_BITS - 1))	// Kernel Aware Interrupt Priority Level


/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  TASK_ADC_PRIO              	3
#define  TASK_PWM_PRIO              	5

#define  OS_TASK_TMR_PRIO             	(OS_LOWEST_PRIO - 2)

/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  TASK_ADC_STK_SIZE            	512
#define  TASK_PWM_STK_SIZE				512


#endif
