#include "stm32f10x.h"
#include "startup.h"
#include "usart1.h"
#include "delay.h"
#include <stdio.h>

uint16_t adcValue = 0;
char strBuffer[32];

int main(void)
{
  /* Th�m chuong tr�nh ch�nh t?i d�y */

  /* ___LIB___ */

  /* Kh?i t?o h? th?ng */
  startup_init();

  /* Kh?i t?o delay */
  delay_init();

  /* Kh?i t?o usart1 */
  usart1_init();

  /* ___GPIO___ */

  /* B?t Clock PortA */
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; // 1: Enable

  /* C?u h�nh PA6 (ADC1 Channel6) */
  GPIOA->CRL &= ~GPIO_CRL_MODE6; // 00: Input mode
  GPIOA->CRL &= ~GPIO_CRL_CNF6; // 00: Analog mode

  /* ___ADC___ */

  /* B?t Clock ADC1 */
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // 1: Enable

  /* C?u h�nh Clock ADC1 */
  RCC->CFGR &= ~RCC_CFGR_ADCPRE; // Clear
  RCC->CFGR |= RCC_CFGR_ADCPRE_0; // 01: PCLK2 divided by 4

  /* C?u h�nh ADC1 */
  ADC1->CR1 &= ~ADC_CR1_DUALMOD; // 0000: Independent mode
  ADC1->CR1 &= ~ADC_CR1_SCAN; // 0: Scan mode disabled
  ADC1->CR2 |= ADC_CR2_CONT; // 1: Continuous conversion mode
  ADC1->CR2 &= ~ADC_CR2_ALIGN; // 0: Right Alignment
  ADC1->CR2 |= ADC_CR2_EXTSEL; // 111: SWSTART (Software Trigger)
  ADC1->SQR1 &= ~ADC_SQR1_L; // 0000: 1 conversion (Regular channel sequence length)

  /* C?u h�nh ADC1 Rank 1 */
  uint32_t adcChannel = 6;
  uint32_t adcRank = 1;
  ADC1->SQR3 |= (adcChannel << (5 * (adcRank - 1))); // Rank = 1th conversion in regular sequence
  ADC1->SMPR2 |= ADC_SMPR2_SMP6; // 111: 239.5 cycles

  /* B?t ADC1 */
  /* If this bit holds a value of zero and a 1 is written to it then it wakes up the ADC from Power Down state */
  /* Conversion starts when this bit holds a value of 1 and a 1 is written to it. The application should allow a delay of "tSTAB" between power up and start of conversion */
  ADC1->CR2 |= ADC_CR2_ADON; // hold_0->write_1: Wakes up the ADC from Power Down state
  __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); // tSTAB
  ADC1->CR2 |= ADC_CR2_ADON; // hold_1->write_1: Conversion starts

  /* Hi?u chu?n di?n �p tham chi?u (Vref) */
  ADC1->CR2 |= ADC_CR2_RSTCAL; // 1: Initialize calibration register
  while ((ADC1->CR2 & ADC_CR2_RSTCAL) == ADC_CR2_RSTCAL); // This bit is set by software and cleared by hardware. It is cleared after the calibration registersare initialized
  ADC1->CR2 |= ADC_CR2_CAL; // 1: Enable calibration
  while ((ADC1->CR2 & ADC_CR2_CAL) == ADC_CR2_CAL); // This bit is set by software to start the calibration. It is reset by hardware after calibration is complete

  /* ___MAIN___ */

  /* B?t d?u chuy?n d?i */
  ADC1->CR2 |= ADC_CR2_SWSTART; // S? d?ng v?i mode Continuous

  /* H�m l?p v� h?n */
  while (1)
  {
    /* B?t d?u chuy?n d?i */
    //ADC1->CR2 |= ADC_CR2_SWSTART; // S? d?ng v?i mode Discontinuous

    /* Ch? cho d?n khi chuy?n d?i th�nh c�ng */
    while ((ADC1->SR & ADC_SR_EOC) != ADC_SR_EOC);

    /* �?c gi� tr? ADC1 */
    adcValue = ADC1->DR;

    /* X�a c? EOC */
    ADC1->SR &= ~ADC_SR_EOC;

    /* D?ng chuy?n d?i */
    //ADC1->CR2 &= ~ADC_CR2_SWSTART; // S? d?ng v?i mode Discontinuous

    /* G?i gi� tr? ADC1 Channel 6 ra usart1 */
    sprintf(strBuffer, "ADC1 (CH6)=%d\r\n", adcValue);
    usart1_putString((uint8_t *)strBuffer);
    delay_ms(100);
  }
}