#include "uart.h"
#include "stm32f10x.h"
void UART_Config(void) {
	//PA10 - UART1-Rx; PA9 - UART1-Tx
	RCC->APB2ENR |= 1<<2 | 1<< 14; // enable clock for gpioa and uart1
	GPIOA->CRH =0;
	GPIOA->CRH |= 0x8<<8; // PA10 input pull
	GPIOA->CRH |= 0xB<<4; // PA9 AF output Push-pull 50MHz
	
	AFIO->MAPR &= ~(1<< 2); // AF remap usart1 = 0 => PA10 PA9
	//base clock 8MHz, baud rate 9600=> usart_brr mantissa = 52, fraction =1
	USART1->BRR = (52UL<<4|1);
	//usart_cr1 bit13: usart enable; bit 5 : ngat khi nhan du lieu; bit 3: bat truyen du lieu; bit2: nhan du lieu
	USART1->CR1 =  0;
	USART1->CR1 |= 1<<13 | 1<<5 | 1<<3 | 1<<2;
}

void UART_SendData(unsigned char Data ) {
	USART1->DR = Data;
	//doi transmit complete
	while (!(USART1->SR & 1<<6));
	USART1->SR &= ~(1<<6);
}
void UART_Transmit (char* string) {
	while (*string) UART_SendData(*string++);
}