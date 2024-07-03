#ifndef __SWM341_EXTI_H__
#define	__SWM341_EXTI_H__


void EXTI_Init(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t mode);
void EXTI_Open(GPIO_TypeDef * GPIOx, uint32_t n);
void EXTI_Close(GPIO_TypeDef * GPIOx, uint32_t n);

uint32_t EXTI_State(GPIO_TypeDef * GPIOx, uint32_t n);
uint32_t EXTI_RawState(GPIO_TypeDef * GPIOx, uint32_t n);
void EXTI_Clear(GPIO_TypeDef * GPIOx, uint32_t n);


#define EXTI_FALL_EDGE	0x00
#define EXTI_RISE_EDGE	0x01
#define EXTI_BOTH_EDGE	0x02
#define EXTI_LOW_LEVEL	0x10
#define EXTI_HIGH_LEVEL	0x11


#endif
