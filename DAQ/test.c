#include "stm32f10x.h"

volatile uint32_t range = 0;

void Timer2_Register(void)
{
	/* enable clock cho timer2 */
	RCC->APB1ENR |= 0x01;
	/* Dat reload la gia tri cao nhat */
	TIM2->ARR |= 0xFFFF;
	/* Set gia tri cho thanh PSC de chia: 1 tick = fCLK/(PSC[15]-1) */
	TIM2->PSC = 71;
	TIM2->CR1 |= 0x01;
	TIM2->EGR |= 0x01;
}

void Delay_Timer2_Us(uint32_t u32Delay)
{
	TIM2->CNT = 0;
	while((TIM2->CNT) < u32Delay);
}

void ConfigInOutput(void)
{
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRL &= ~(0xFFU);
	/* Input pin A0 */
	GPIOA->CRL |= 0x08U;
	GPIOA->ODR &= ~0x01U; // pull down
	/* Output pin A1 */
	GPIOA->CRL |=  0x30;
	GPIOA->ODR &= ~0x02;
}

/*
void Timer_Caculator(void)
{
	while((GPIOA->IDR & GPIO_IDR_IDR0) == 0);
	TIM2->CNT = 0U;	
	while((GPIOA->IDR & GPIO_IDR_IDR0) != 0);
	uint32_t range = (TIM2->CNT * 0.343 / 2);
}
*/

int main(void)
{
	Timer2_Register();
	ConfigInOutput();
	while(1)
	{
		/* Phat xung 40us */
		GPIOA->ODR |= 0x02;
		Delay_Timer2_Us(40);
		GPIOA->ODR &= ~0x02;
		while((GPIOA->IDR & GPIO_IDR_IDR0) == 0);
		TIM2->CNT = 0;	
		while((GPIOA->IDR & GPIO_IDR_IDR0) != 0);
		range = (uint32_t)((double)TIM2->CNT * 0.173);
	}
}
