#include "stm32f10x.h"

void PWM_Timer2_Reister(void)
{
	// enable clock cho timer2 ~ 36MHz
	RCC->APB1ENR |= 0x01;
	// dat gia tri reload, no tuong duong voi chon tan so, chon do rong xung 
	TIM2->ARR = 10000-1;
	// chia clock, set tgian cho 1 tick
	TIM2->PSC = 72*2-1;
	// set cho 4 kenh
	TIM2->CCMR1 |= 0x6060;
	
	TIM2->CCR1 = 10000/20;
	TIM2->CCR2 = 10000/4;
	
	TIM2->CCER = 0x1111;
	TIM2->CR1 = 0x01;
}

void ConfigAlternate(void)
{
	RCC->APB2ENR |= 0x04;
	GPIOA->CRL &= ~0x0FFFF;
	GPIOA->CRL |= 0x0BBBB;
}

int main()
{
	ConfigAlternate();
	PWM_Timer2_Reister();
	
	while(1)
	{
		// Chan PA0 dung de out tin hieu
	}
		
}

