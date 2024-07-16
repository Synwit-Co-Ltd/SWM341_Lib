#include "SWM341.h"


int main(void)
{	
	PWM_InitStructure  PWM_initStruct;
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN5, 1, 0, 0, 0);			// debug indication pin
	
	PORT_Init(PORTM, PIN1,  PORTM_PIN1_PWM0A,   0);
	PORT_Init(PORTM, PIN4,  PORTM_PIN4_PWM0AN,  0);
	PORT_Init(PORTM, PIN2,  PORTM_PIN2_PWM0B,   0);
	PORT_Init(PORTM, PIN5,  PORTM_PIN5_PWM0BN,  0);
	
	PORT_Init(PORTM, PIN3,  PORTM_PIN3_PWM1A,   0);
	PORT_Init(PORTM, PIN6,  PORTM_PIN6_PWM1AN,  0);
	PORT_Init(PORTD, PIN9,  PORTD_PIN9_PWM1B,   0);
	PORT_Init(PORTD, PIN8,  PORTD_PIN8_PWM1BN,  0);
	
	PORT_Init(PORTM, PIN12, PORTM_PIN12_PWM2A,  0);
	PORT_Init(PORTM, PIN9,  PORTM_PIN9_PWM2AN,  0);
	PORT_Init(PORTM, PIN11, PORTM_PIN11_PWM2B,  0);
	PORT_Init(PORTM, PIN8,  PORTM_PIN8_PWM2BN,  0);
	
	PORT_Init(PORTC, PIN2,  PORTC_PIN2_PWM3A,   0);
	PORT_Init(PORTC, PIN3,  PORTC_PIN3_PWM3AN,  0);
	PORT_Init(PORTB, PIN1,  PORTB_PIN1_PWM3B,   0);
	PORT_Init(PORTB, PIN0,  PORTB_PIN0_PWM3BN,  0);
	
 	PORT_Init(PORTB, PIN15, PORTB_PIN15_PWM4A,  0);
// 	PORT_Init(PORTB, PIN14, PORTB_PIN14_PWM4AN, 0);		// SWDIO
 	PORT_Init(PORTB, PIN13, PORTB_PIN13_PWM4B,  0);
// 	PORT_Init(PORTB, PIN12, PORTB_PIN12_PWM4BN, 0);		// SWDCK
	
	PWM_initStruct.Mode = PWM_CENTER_ALIGNED;
	PWM_initStruct.Clkdiv = 4;					// F_PWM = 20M/4 = 5M
	PWM_initStruct.Period = 10000;				// 5M/10000 = 500Hz, 250Hz for center alignment mode
	PWM_initStruct.HdutyA =  2500;				// 2500/10000 = 25%
	PWM_initStruct.DeadzoneA = 50;				// 50/5M = 10us
	PWM_initStruct.IdleLevelA = 0;
	PWM_initStruct.IdleLevelAN= 0;
	PWM_initStruct.OutputInvA = 0;
	PWM_initStruct.OutputInvAN= 0;
	PWM_initStruct.HdutyB =  7500;				// 7500/10000 = 75%
	PWM_initStruct.DeadzoneB = 50;				// 50/5M = 10us
	PWM_initStruct.IdleLevelB = 0;
	PWM_initStruct.IdleLevelBN= 0;
	PWM_initStruct.OutputInvB = 0;
	PWM_initStruct.OutputInvBN= 0;
	PWM_initStruct.UpOvfIE    = 0;
	PWM_initStruct.DownOvfIE  = 0;
	PWM_initStruct.UpCmpAIE   = 0;
	PWM_initStruct.DownCmpAIE = 0;
	PWM_initStruct.UpCmpBIE   = 0;
	PWM_initStruct.DownCmpBIE = 0;
	PWM_Init(PWM0, &PWM_initStruct);
	PWM_Init(PWM1, &PWM_initStruct);
	PWM_Init(PWM2, &PWM_initStruct);
	PWM_Init(PWM3, &PWM_initStruct);
	PWM_Init(PWM4, &PWM_initStruct);
	
	PWM_Start(PWM0_MSK|PWM1_MSK|PWM2_MSK|PWM3_MSK|PWM4_MSK);
	
	
	/* Brake function using demo */
#if 0
	PORT_Init(PORTA, PIN2, PORTA_PIN2_PWM_BRK0, 1);
	PORTA->PULLU |= (1 << PIN2);
	PORT_Init(PORTM, PIN0, PORTM_PIN0_PWM_BRK1, 1);
	PORTM->PULLU |= (1 << PIN0);
	
	PWM_BrkInPolarity(PWM_BRK0 | PWM_BRK1 | PWM_BRK2, 0);			// brake when PWM_BRK0, PWM_BRK1 and PWM_BRK2 is low level
	
	PWM_BrkConfig(PWM0, PWM_CH_A, PWM_BRK0 | PWM_BRK1, 0, 1, 1, 0);	// PWM0 channel A is braked by brake inputs PWM_BRK0 and PWM_BRK1
	PWM_BrkConfig(PWM0, PWM_CH_B, PWM_BRK0 | PWM_BRK1, 0, 1, 1, 0);	// PWM0 channel B is braked by brake inputs PWM_BRK0 and PWM_BRK1
	
	/* Brake interrupt using demo */
#if 1
	PWM_BrkIntEn(PWM_BRKIT_BRK0);
	NVIC_EnableIRQ(PWMBRK_IRQn);
#endif
#endif

	/* Software Brake using demo */
#if 0
	PWM_BrkConfig(PWM0, PWM_CH_A, 0, 0, 1, 1, 0);	// PWM0 channel A cannot be braked by brake input signal, only can brake by software
	PWM_BrkConfig(PWM0, PWM_CH_B, 0, 0, 1, 1, 0);	// PWM0 channel B cannot be braked by brake input signal, only can brake by software
	
	while(1)
	{
		GPIO_SetBit(GPIOA, PIN5);
		PWMG->SWBRK |= (PWMG_SWBRK_PWM0A_Msk |		// software brake
						PWMG_SWBRK_PWM0B_Msk);
		for(int i = 0; i < 300000; i++) __NOP();
		
		GPIO_ClrBit(GPIOA, PIN5);
		PWMG->SWBRK &=~(PWMG_SWBRK_PWM0A_Msk |		// software brake exit, restore normal output
						PWMG_SWBRK_PWM0B_Msk);
		for(int i = 0; i < 900000; i++) __NOP();
	}
#endif

	while(1==1)
	{
	}
}


void PWMBRK_Handler(void)
{
	if(PWM_BrkIntStat(PWM_BRKIT_BRK0))
	{
		PWM_BrkIntDis(PWM_BRKIT_BRK0);	// The brake interrupt must be turned off, otherwise it will trigger the interrupt continuously during braking
		
		GPIO_InvBit(GPIOA, PIN5);
	}
}
