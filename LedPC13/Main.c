#include "stm32f10x.h"
#include "Config.h"
#include "math.h"
uint32_t u32Count = 0U;
void Delay_Ms(uint32_t nCount) 
{
  while(nCount--);
}
int main(void)
{
	GPIO_ConfigPinC13();
	GPIO_ConfigInput();
	while (1)
	{
		/*
		GPIOC->ODR |= (1 << 13);
		Delay_Ms(1000000);
		GPIOC->ODR &= ~(1 << 13);
		Delay_Ms(1000000);

		*/
		Delay_Ms(10000);
		if ((GPIOA->IDR & 1) == 0U)
		{
			while (0U == (GPIOA->IDR & 1)){};
				u32Count++;
			
				if (u32Count % 2 != 0)
				{
					GPIOC->ODR |= (1 << 13);
				}
				else
				{
					//Delay_Ms(1000000);
					GPIOC->ODR &= ~(0x01<<13);
				}
		}
		
		
	}
	return 0;
}
