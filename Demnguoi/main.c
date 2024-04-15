#include "stm32f10x.h"

uint8_t LedCode7Seg[10] = {0x81,0xCF,0x92,0x86,0xCC,0xA4,0xA0,0x8F,0x80,0x84};
void ConfigOutput(void);
void ConfigInput(void);
void ShowLeg7sSeg(uint8_t no, uint32_t Number);
void Display_7seg(uint32_t u32Number);

void ConfigInput(void)
{
	RCC->APB2ENR |= (1<<2);
	GPIOA->CRH &= ~(0xFFF00U); 	// 1111 1111 1111 0000 0000 
	GPIOA->CRH |= 0x88800U; 		// 1000 1000 1000 0000 0000
	GPIOA->ODR |= 0x1C00U;			// 0001 1010 0000 0000
}

void Delay_Ms(uint32_t nCount) 
{
  while(nCount--);
}

void ConfigOutput(void)
{
	RCC->APB2ENR |= 0x04;				// 0000 0100
	GPIOA->CRL &= 0x00000000;		// 0
	GPIOA->CRL |= 0x33333333;		// 0010 .... 0010
	GPIOA->CRH &= 0xFFFFFF00; 	// 1111 1111 1111 1111 1111 1111 0000 0000
	GPIOA->CRH |= 0x00000033;		// 0000 .... 								0000 0010 0010
}

void ShowLed7sSeg(uint8_t no, uint32_t Number)
{	 
	
	uint32_t u323TempNumber = LedCode7Seg[Number];
	if((u323TempNumber&0x01U) == 1) 
		GPIOA->ODR |= GPIO_ODR_ODR6;
	else GPIOA->ODR &= ~GPIO_ODR_ODR6;
	
	u323TempNumber = u323TempNumber >> 1;
	if((u323TempNumber&0x01U) == 1) 
		GPIOA->ODR |= GPIO_ODR_ODR5;
	else GPIOA->ODR &= ~GPIO_ODR_ODR5;
	
	u323TempNumber = u323TempNumber >> 1;
	if((u323TempNumber&0x01U) == 1) 
		GPIOA->ODR |= GPIO_ODR_ODR4;
	else GPIOA->ODR &= ~GPIO_ODR_ODR4;
	
	u323TempNumber = u323TempNumber >> 1;
	if((u323TempNumber&0x01U) == 1) 
		GPIOA->ODR |= GPIO_ODR_ODR3;
	else GPIOA->ODR &= ~GPIO_ODR_ODR3;
	
	u323TempNumber = u323TempNumber >> 1;
	if((u323TempNumber&0x01U) == 1) 
		GPIOA->ODR |= GPIO_ODR_ODR2;
	else GPIOA->ODR &= ~GPIO_ODR_ODR2;
	
	u323TempNumber = u323TempNumber >> 1;
	if((u323TempNumber&0x01U) == 1) 
		GPIOA->ODR |= GPIO_ODR_ODR1;
	else GPIOA->ODR &= ~GPIO_ODR_ODR1;
	
	u323TempNumber = u323TempNumber >> 1;
	if((u323TempNumber&0x01U) == 1) 
		GPIOA->ODR |= GPIO_ODR_ODR0;
	else GPIOA->ODR &= ~GPIO_ODR_ODR0;
	
	if(no == 1) GPIOA->ODR |= GPIO_ODR_ODR7;
	else if(no == 2) GPIOA->ODR |= GPIO_ODR_ODR8;
	else if(no == 3) GPIOA->ODR |= GPIO_ODR_ODR9;
	else if(no == 4) GPIOA->ODR |= GPIO_ODR_ODR7 | GPIO_ODR_ODR8 | GPIO_ODR_ODR9;
	else GPIOA->ODR &= ~0x380;
	
	Delay_Ms(70000);
}

void Display_7seg(uint32_t u32Number)
{
	uint32_t u32Digit;
	
	if(u32Number >= 999) 
	{
		ShowLed7sSeg(4, 9);
	} else 
	{
		u32Digit = u32Number / 100;
		ShowLed7sSeg(1, u32Digit);
		GPIOA->ODR &= ~0x380;	
		
		u32Number = u32Number - u32Digit*100;
		u32Digit = u32Number / 10;
		ShowLed7sSeg(2, u32Digit);
		GPIOA->ODR &= ~0x380;
		
		u32Digit = u32Number - u32Digit*10;
		ShowLed7sSeg(3, u32Digit);
		GPIOA->ODR &= ~0x380;
	}
}

int main(void)
{
	uint32_t u32Count = 1;
	ConfigOutput();
	ConfigInput();
	/*
	while(1)
	{
		Delay_Ms(100U);
		if((GPIOA->IDR & GPIO_IDR_IDR10) == 0U) 
		{
			Delay_Ms(100U);
			while((GPIOA->IDR & GPIO_IDR_IDR10) == 0U);
			u32Count = 0;
		}
		
		if((GPIOA->IDR & GPIO_IDR_IDR11) == 0 && (GPIOA->IDR & GPIO_IDR_IDR12) != 0)
		{
			Delay_Ms(100U);
			while((GPIOA->IDR & GPIO_IDR_IDR11) == 0 || (GPIOA->IDR & GPIO_IDR_IDR12) == 0);
			u32Count ++;
		}
		
		if((GPIOA->IDR & GPIO_IDR_IDR11) != 0 && (GPIOA->IDR & GPIO_IDR_IDR12) == 0)
		{
			Delay_Ms(100U);
			while((GPIOA->IDR & GPIO_IDR_IDR11) == 0 || (GPIOA->IDR & GPIO_IDR_IDR12) == 0);
			if(u32Count != 0) u32Count --;
		}
		
		Display_7seg(u32Count);
	}
	*/
	while (1) {
		Delay_Ms(1000U);
		while (u32Count < 100) {
			Display_7seg(u32Count);
			u32Count++;
			Delay_Ms(1000U);
		}
		u32Count = 0;
	}
}
