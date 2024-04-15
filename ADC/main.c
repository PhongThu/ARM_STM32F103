#include "stm32f10x.h"
#include "math.h"
#include "Delay.h"
uint16_t adcValue = 0;

void Delay_Ms(uint32_t nCount)
{
  while(nCount--)
  {
  }
}

void ADC_Register(void)
{
	/* Config GPIO */
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL &= ~GPIO_CRL_CNF6;
	GPIOA->CRL &= ~GPIO_CRL_MODE6;
	
	/* Config ADC */
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // ennable clock cho ADC 14MHz
	RCC->CFGR &= ~RCC_CFGR_ADCPRE; // clear bit
	RCC->CFGR |= RCC_CFGR_ADCPRE_1; // set bit <=> PCLK2 divide 4 = 3.5Mhz
	
	ADC1->CR1 &= ~ADC_CR1_DUALMOD;
	ADC1->CR1 &= ~ADC_CR1_SCAN; // disable scan mode
	ADC1->CR2 |= ADC_CR2_CONT; // 
	ADC1->CR2 &= ~ADC_CR2_ALIGN;
	ADC1->CR2 |= ADC_CR2_EXTSEL;
	ADC1->SQR1 &= ~ADC_SQR1_L; 
	
	ADC1->SQR3 = 6; // set gia tri no set thanh ghi
	
	ADC1->SMPR2 |= ADC_SMPR2_SMP0; // 1.5 cycle
	
	ADC1->CR2 |= ADC_CR2_ADON;
	__NOP(); __NOP(); __NOP(); __NOP(); __NOP();
	ADC1->CR2 |= ADC_CR2_ADON;
	
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	while((ADC1->CR2 & ADC_CR2_RSTCAL) == ADC_CR2_RSTCAL); // waiting bit RSTCAL clear
	ADC1->CR2 |= ADC_CR2_CAL;
	while((ADC1->CR2 & ADC_CR2_CAL) == ADC_CR2_CAL);  

	ADC1->CR2 |= ADC_CR2_SWSTART;
}

//uint32_t sine[255];

const unsigned char wave[181] = {128,132,137,141,146,150,155,159,163,
																 168,172,176,180,184,188,192,196,200,
																 203,207,210,214,217,220,223,226,229,
																 232,234,237,239,241,243,245,247,248,
																 250,251,252,253,254,254,255,255,255,
																 255,255,255,255,255,254,253,252,251,
																 250,248,247,245,243,241,239,237,234,
																 232,229,226,223,220,217,214,210,207,
																 203,200,196,192,188,184,180,176,172,
																 168,163,159,155,150,146,141,137,132,
																 128,124,119,115,110,106,101,97,93,88,
																 84,80,76,72,68,64,60,56,53,49,46,42,
																 39,36,33,30,27,24,22,19,17,15,13,11,
																 9,8,6,5,4,3,2,1,1,0,0,0,0,0,1,1,2,3,
																 4,5,6,8,9,11,13,15,17,19,22,24,27,30,
																 33,36,39,42,46,49,53,56,60,64,68,72,
																 76,80,84,88,93,97,101,106,110,115,119,124,128};

// Config pin in port A
void ConfigPortB()
{
	RCC->APB2ENR |= 1 << 3; // Enable clock port B
	GPIOB->CRL &= 0x00000000; // Reset pin B0 -> B7
	GPIOB->CRL |= 0x11111111; // Set output push-pull pin B0 -> B7 
}
void ConfigPortA()
{
	RCC->APB2ENR |= 1 << 2;
	GPIOA->CRL &= 0x0000;
	GPIOA->CRL |= 0x8888;
	
	GPIOA->ODR |= 1 | 1 << 1 | 1 << 2 | 1 << 3;
}
/*void SetupSin()
{
	double x, y;
	for (int i = 0; i < 256; i++)
		{
    x = (double)i;
    y = sin((x / 256) * 2 * 3.1415926535897932384626433832799);
    sine[i] = (int) (y * 128) + 128;
		}
}*/

// Generate a sine signal
void Sine_Funtion(uint32_t freq)
{
		for (int i = 0; i < 181; i++) 
		{
			GPIOB->ODR = wave[i]; 
			Delay_ms(freq);
		}
}


// Generate a triangle signal
void Triangle_Funtion(uint32_t freq)
{
	for (int i = 0; i <= 255; i++)
		{
			GPIOB->ODR = i;
			Delay_ms(freq/2);
		}

		for (int i = 255; i >= 0; i--)
		{
			GPIOB->ODR = i;
			Delay_ms(freq/2);
		}
}

// Generate a saw signal
void Saw_Funtion(uint32_t freq)
{
	for (int i = 0; i <= 255; i++)
	{
		GPIOB->ODR = i;
		Delay_ms(freq);
	}
		
}

// Generate a square signal
void Square_Funtion(uint32_t freq)
{
	for (int i = 0; i <= 255; i++)
	{
		GPIOB->ODR = 255;
		Delay_ms(freq/2);
	}
	for (int i = 255; i >= 0; i--)
	{
		GPIOB->ODR = 0;
		Delay_ms(freq/2);
	}
	
}
int main()
{
	
	ConfigPortA();
	ConfigPortB();
	//ADC_Register();
	initSysTickInterrupt();
	SysTick_Handler();
	//SetupSin();
	while (1)
	{

		Sine_Funtion(20);
//		while((ADC1->SR & ADC_SR_EOC) != ADC_SR_EOC);
//		adcValue = ADC1->DR;
//		ADC1->SR &= ~ADC_SR_EOC;
//		Delay_ms(1000U);
		/*
		if ((GPIOA->IDR & 1 << 1) == 0)
		{
			Triangle_Funtion(20);
		}
		if ((GPIOA->IDR & 1 << 2) == 0)
		{
			Saw_Funtion(20);
		}
		if ((GPIOA->IDR & 1 << 3) == 0)
		{
			Square_Funtion(20);
		}
		*/
		//Sine_Funtion(20);
		//Triangle_Funtion(20);
		//Square_Funtion(20);
		//Saw_Funtion(20);
	}
}

/*
int main(void)
{
	ADC_Register();
	while(1)
	{
		while((ADC1->SR & ADC_SR_EOC) != ADC_SR_EOC);
		adcValue = ADC1->DR;
		ADC1->SR &= ~ADC_SR_EOC;
		Delay(1000U);
	}
	
}

*/