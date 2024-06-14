#include "SysClock.h"
#include "stm32f10x.h"

void SysClkConf(void) {

	//1.Turn on HSE to use.
	RCC->CR |= 1<<16; // HSE on
	while((RCC->CR & (1<<17)) == 0); // wait HSERDY.

	//2. config PLL (HSE, MUL9).
	RCC->CFGR |= 0x07<<18; // PLLMUL9 -> systemclock = 72MHz
	RCC->CFGR |= 1<<15; // ADC prescale 6.
	RCC->CFGR |= 1<<10; //APB1 prescale 2.

	//3. choose new clock source.
	RCC->CFGR |= (1<<16); // PLLSRC HSE

	//4. enable PLL
	RCC->CR |= 1<<24;
	while((RCC->CR & 1<<25) == 0); // wait PLLRDY.

	//5. switch sysclk to PLL
	RCC->CFGR |= 0x02;
	while((RCC->CFGR & (1<<3)) == 0); //wait SWS.

	//6.turn off original source
	RCC->CR &= ~(0x01); // off HSION
	while((RCC->CR & 0x02)==0x02);
	//7. supply clock
	RCC->APB2ENR |= 0x7<<2; //en clk for GPIOA, B, C.
	RCC->APB2ENR |= 1<<9; // en clk for ADC1.
	RCC->APB2ENR |= 0x01; // en clk for AFIO.
	RCC->APB1ENR |= 0x03; // en clk for TIM2, 3.
	RCC->APB2ENR |= 1<<14 | 1<<2; // en clk for usart1, gpio A
}
void delay(uint32_t time) {
	uint32_t i;
    for(i = 0; i < time; i++) {
        uint32_t delay_count = 4000;
        while(delay_count--);
    }
}