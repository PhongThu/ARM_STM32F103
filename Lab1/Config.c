#include "stm32f10x.h"                  // Device header

void GPIO_ConfigPinC13(void)
{
	// Or 0b10000 -> Anabling Preiph GPIOC
	RCC->APB2ENR |= 0x10;
	// Reset PORT C Pin 13
	GPIOC->CRH &= 0xFF0FFFFF;
	// 11: Output mode, max speed 50MHz
	GPIOC->CRH |= 0x00300000;
	
}