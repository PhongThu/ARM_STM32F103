#include "stm32f10x.h"                  // Device header

void GPIO_ConfigPinC13(void)
{
	// Or 0b10000 -> Anabling Preiph GPIOC
	RCC->APB2ENR |= 1 << 4;
	// Reset PORT C Pin 13
	GPIOC->CRH &= 0xFF0FFFFF;
	// 11: Output mode, max speed 50MHz
	GPIOC->CRH |= 0x00100000;
}

void GPIO_ConfigInput(void)
{
	// Enable clock Port A
	RCC->APB2ENR |= 1<<2;
	
	// Clear 4 bit from 0 -> 3
	//GPIOA->CRL &= ~(0xf);
	
	// Clear all bits
	GPIOA->CRL &= ~(0xff);
	
	// Select mode PA0: input ~3 = 1100
	//GPIOA->CRL &= ~(0x3);
	
	// Select input pull up/down 
	GPIOA->CRL |= 1 << 3;
	
	// Choose input pull up
	GPIOA->ODR |= 1;
}
