#include "stm32f10x.h"
/*
void ConfigInput() {
	RCC->APB2ENR |= 1 << 2; //Clock access enable for GPIOC.
	GPIOA->CRH &= ~(0xFFF00U);
	
	// 1000 1000 1000 0000 0000
	GPIOA->CRH |= 0x88800U;		//Set port 10, 11, 12: input with pull up/down
	
	// 0001 1010 0000 0000 	
	GPIOA->ODR |= 0x1C00U;			//Set Port Output Data Reg: 9, 11, 12
	
}

void ConfigOutput() {
	RCC->APB2ENR |= 1 << 3; //Clock access enable for GPIOD
	
	
}
*/