#include "stm32f10x.h"
#include "UART.h"

// Ð?a ch? HMC5883L
#define HMC5883L_ADDRESS 0x1E

void I2C1_Init(void) {
    // B?t clock cho I2C1 và GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // C?u hình chân I2C (PB6 - SCL, PB7 - SDA)
    GPIOB->CRL &= ~((GPIO_CRL_MODE6 | GPIO_CRL_CNF6 | GPIO_CRL_MODE7 | GPIO_CRL_CNF7));
    GPIOB->CRL |= ((GPIO_CRL_MODE6_1 | GPIO_CRL_MODE7_1) | (GPIO_CRL_CNF6 | GPIO_CRL_CNF7));
    GPIOB->ODR |= (GPIO_ODR_ODR6 | GPIO_ODR_ODR7);

    // C?u hình I2C1
    I2C1->CR2 |= 36; // PCLK1 = 36MHz
    I2C1->CCR |= 180; // 100kHz
    I2C1->TRISE = 37; // 1000ns / (1/36MHz) + 1

    I2C1->CR1 |= I2C_CR1_PE; // Kích ho?t I2C1
}

void I2C_WriteData(uint8_t address, uint8_t reg, uint8_t data) {
    // B?t d?u truy?n
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));

    // G?i d?a ch?
    I2C1->DR = (address << 1);
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2; // Ð?c SR2 d? xóa c?

    // G?i thanh ghi
    I2C1->DR = reg;
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    // G?i d? li?u
    I2C1->DR = data;
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    // D?ng truy?n
    I2C1->CR1 |= I2C_CR1_STOP;
}

void HMC5883L_Init(void) {
    // Cau hinh thanh ghi dieu khien A
    I2C_WriteData(HMC5883L_ADDRESS, 0x00, 0x70);

    // Cau hinh thanh ghi dieu khien B
    I2C_WriteData(HMC5883L_ADDRESS, 0x01, 0xA0);

    // Cau hình thanh ghi mode
    I2C_WriteData(HMC5883L_ADDRESS, 0x02, 0x00);
}

void I2C_ReadData(uint8_t address, uint8_t reg, uint8_t* buffer, uint8_t length) {
    // Bat dau truyen
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));

    // Gui dia chi
    I2C1->DR = (address << 1);
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2; // Doc SR2 de xoa co

    // Gui thanh ghi
    I2C1->DR = reg;
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    // Bat dau truyen nhan du lieu
    I2C1->CR1 |= I2C_CR1_START;
    while (!(I2C1->SR1 & I2C_SR1_SB));

    // Gui dia chi
    I2C1->DR = (address << 1) | 1;
    while (!(I2C1->SR1 & I2C_SR1_ADDR));
    (void)I2C1->SR2; // Doc SR2 de xoa co

    // Doc du lieu
    while (length) {
        if (length == 1) {
            I2C1->CR1 &= ~I2C_CR1_ACK; // Tat ACK
            I2C1->CR1 |= I2C_CR1_STOP; // Tao tín hieu STOP
        }

        if (I2C1->SR1 & I2C_SR1_RXNE) {
            *buffer = I2C1->DR;
            buffer++;
            length--;
        }
    }

    // Bat lai ACK
    I2C1->CR1 |= I2C_CR1_ACK;
}

void HMC5883L_ReadData(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t buffer[6];
    I2C_ReadData(HMC5883L_ADDRESS, 0x03, buffer, 6);

    *x = (int16_t)(buffer[0] << 8 | buffer[1]);
    *y = (int16_t)(buffer[4] << 8 | buffer[5]);
    *z = (int16_t)(buffer[2] << 8 | buffer[3]);
}

int main(void) {
	
    int16_t x, y, z;
	char buffer[100];
		RCC_Configuration();
    GPIO_Configuration();
    USART_Configuration();
	
    // Khoi tao I2C
    I2C1_Init();

    // Khoi tao HMC5883L
    HMC5883L_Init();

	
    while (1) {
			
        // Ðoc du lieu tu HMC5883L
//        HMC5883L_ReadData(&x, &y, &z);
//				sprintf(buffer, "X: %d, Y: %d, Z: %d\r\n", x, y, z);
//				
        // Xu ly du lieu
//			USART_SendString(buffer);
			USART_SendString("ok");
		}
}
