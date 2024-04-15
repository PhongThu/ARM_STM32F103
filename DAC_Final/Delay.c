#include "stm32f10x.h"

int32_t volatile msTicks = 0; // Interval counter in ms

void initSysTickInterrupt()
{
	SysTick->LOAD = SystemCoreClock/72000; // configured the SysTick to count in 1ms
	/* Select Core Clock & Enable SysTick & Enable Interrupt */
	//SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL |= 0x7;
}

void SysTick_Handler()
{
	msTicks++;
}

void Delay_ms(uint32_t TICK)
{
	while (msTicks < TICK); // Wait 72MHz/72000 = 1000Hz
	msTicks = 0; 
}
