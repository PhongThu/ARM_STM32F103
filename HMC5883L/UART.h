void RCC_Configuration();
void GPIO_Configuration();
void USART_Configuration();
void USART_SendChar(uint8_t ch);
void USART_SendString(const char *str);
uint8_t USART_ReceiveChar(void);