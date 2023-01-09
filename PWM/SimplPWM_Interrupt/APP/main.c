#include "SWM341.h"


int main(void)
{
	PWM_InitStructure  PWM_initStruct;
	
	SystemInit();
	
	GPIO_Init(GPIOA, PIN8, 1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN9, 1, 0, 0, 0);
	GPIO_Init(GPIOA, PIN10,1, 0, 0, 0);
	
	PORT_Init(PORTM, PIN1,  PORTM_PIN1_PWM0A,   0);
	PORT_Init(PORTM, PIN4,  PORTM_PIN4_PWM0AN,  0);
	PORT_Init(PORTM, PIN2,  PORTM_PIN2_PWM0B,   0);
	PORT_Init(PORTM, PIN5,  PORTM_PIN5_PWM0BN,  0);
	
	PWM_initStruct.Mode = PWM_CENTER_ALIGNED;
	PWM_initStruct.Clkdiv = 6;					//F_PWM = 30M/6 = 5M	
	PWM_initStruct.Period = 10000;				//5M/10000 = 500Hz，中心对称模式下频率降低到250Hz
	PWM_initStruct.HdutyA =  2500;				//2500/10000 = 25%
	PWM_initStruct.DeadzoneA = 50;				//50/5M = 10us
	PWM_initStruct.IdleLevelA = 0;
	PWM_initStruct.IdleLevelAN= 0;
	PWM_initStruct.OutputInvA = 0;
	PWM_initStruct.OutputInvAN= 0;
	PWM_initStruct.HdutyB =  7500;				//5000/10000 = 50%
	PWM_initStruct.DeadzoneB = 50;				//50/5M = 10us
	PWM_initStruct.IdleLevelB = 0;
	PWM_initStruct.IdleLevelBN= 0;
	PWM_initStruct.OutputInvB = 0;
	PWM_initStruct.OutputInvBN= 0;
	PWM_initStruct.UpOvfIE    = 1;
	PWM_initStruct.DownOvfIE  = 1;
	PWM_initStruct.UpCmpAIE   = 1;
	PWM_initStruct.DownCmpAIE = 0;
	PWM_initStruct.UpCmpBIE   = 0;
	PWM_initStruct.DownCmpBIE = 0;
	PWM_Init(PWM0, &PWM_initStruct);
	
 	PWM_Start(PWM0_MSK);
	
	while(1==1)
	{
	}
}

void PWM0_Handler(void)
{
	if(PWM_IntStat(PWM0, PWM_IT_OVF_UP))
	{
		PWM_IntClr(PWM0, PWM_IT_OVF_UP);
		
		GPIO_InvBit(GPIOA, PIN8);
	}
	else if(PWM_IntStat(PWM0, PWM_IT_CMPA_UP))
	{
		PWM_IntClr(PWM0, PWM_IT_CMPA_UP);
		
		GPIO_InvBit(GPIOA, PIN9);
	}
	else if(PWM_IntStat(PWM0, PWM_IT_CMPB_UP))
	{
		PWM_IntClr(PWM0, PWM_IT_CMPB_UP);
		
		GPIO_InvBit(GPIOA, PIN10);
	}
}
