#ifndef __SWM341_GPIO_H__
#define	__SWM341_GPIO_H__


void GPIO_Init(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t dir, uint32_t pull_up, uint32_t pull_down, uint32_t open_drain);

#define GPIO_INPUT						((0 << 0) | (0 << 1) | (0 << 2) | (0 << 3))
#define GPIO_INPUT_PullUp				((0 << 0) | (1 << 1) | (0 << 2) | (0 << 3))
#define GPIO_INPUT_PullDown				((0 << 0) | (0 << 1) | (1 << 2) | (0 << 3))
#define GPIO_OUTPUT						((1 << 0) | (0 << 1) | (0 << 2) | (0 << 3))
#define GPIO_OUTPUT_OpenDrain			((1 << 0) | (0 << 1) | (0 << 2) | (1 << 3))
#define GPIO_OUTPUT_OpenDrain_PullUp	((1 << 0) | (1 << 1) | (0 << 2) | (1 << 3))

#define GPIO_INIT(GPIOx, n, mode)  GPIO_Init(GPIOx, n, (mode & 1) ? 1 : 0, (mode & 2) ? 1 : 0, (mode & 4) ? 1 : 0, (mode & 8) ? 1 : 0)


void GPIO_WriteBit(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t v);
void GPIO_SetBit(GPIO_TypeDef * GPIOx, uint32_t n);
void GPIO_ClrBit(GPIO_TypeDef * GPIOx, uint32_t n);
void GPIO_InvBit(GPIO_TypeDef * GPIOx, uint32_t n);
uint32_t GPIO_GetBit(GPIO_TypeDef * GPIOx, uint32_t n);

void GPIO_WriteBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w, uint32_t v);
void GPIO_SetBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w);
void GPIO_ClrBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w);
void GPIO_InvBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w);
uint32_t GPIO_GetBits(GPIO_TypeDef * GPIOx, uint32_t n, uint32_t w);



// for compatibility
#define GPIO_AtomicSetBit	GPIO_SetBit
#define GPIO_AtomicClrBit	GPIO_ClrBit
#define GPIO_AtomicInvBit	GPIO_InvBit

#define GPIO_AtomicSetBits(GPIOx, n, w)	{ __disable_irq(); GPIO_SetBits(GPIOx, n, w); __enable_irq(); }
#define GPIO_AtomicClrBits(GPIOx, n, w)	{ __disable_irq(); GPIO_ClrBits(GPIOx, n, w); __enable_irq(); }
#define GPIO_AtomicInvBits(GPIOx, n, w)	{ __disable_irq(); GPIO_InvBits(GPIOx, n, w); __enable_irq(); }


#endif
