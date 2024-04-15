#include "stm32f10x.h"
#include "math.h"

void GPIO_Config(void)
{
	RCC->APB2ENR |= 0x04;
	GPIOA->CRH &= ~0xFFFFFFFF;
	GPIOA->CRH |= 0x33333333;
//	GPIOA->ODR |= 0xFF00;
}

void Delay_Ms(uint32_t nCount) 
{
  while(nCount--);
}
int main(void)
{
	GPIO_Config();
	uint32_t i;
	uint32_t x;
	
	while(1)
	{
		for(i=0; i<1000; i++)
		{
			x =(uint32_t)256*sin((float)i/1000 * 2*3.14);
			x = x << 8;
			GPIOA->ODR = x; 
			Delay_Ms(100);
		}
	}
}