#include "stm32f103x.h"

void ConfigPortA() {

	RCC->APBENR2 = 1 << 2;
	GPIOA->CRL &= ~(0xFFFFFFFF);
	GPIOA->CRL |= 0x33333333;
	GPIOA->ODR |= 0x11111111;
	
}
int main()
{
	
	return 0;
}
	