#include "stm32f10x.h"

void I2C_Config(void);
void I2C_Start(void);
void I2C_Write(uint8_t data);
void I2C_Address(uint8_t address);
void I2C_Stop(void);
void I2C_WriteMul(uint8_t *data, uint8_t size);
void LCD_Write(uint8_t address, uint8_t *data, int size);
void lcd_send_cmd(char cmd);
void lcd_send_data(char data);
void lcd_clear(void);
void lcd_put_cur(int row, int col);
void lcd_init(void);
void lcd_send_string(char *str);
void Delay_Config(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

#define SLAVE_ADDRESS_LCD 0x27
#define TIMEOUT 10000

/* I2C Configuration */
void I2C_Config(void)
{
    /* Enable clock */
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // I2C1
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Port B
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    
    /* Configure I2C GPIO PB6 and PB7 as alternate function */
    GPIOB->CRL &= ~0xFF000000;
    GPIOB->CRL |= GPIO_CRL_CNF6 | GPIO_CRL_CNF7;
    GPIOB->CRL |= GPIO_CRL_MODE6 | GPIO_CRL_MODE7;
    
    /* Reset I2C */
    I2C1->CR1 |= I2C_CR1_SWRST;
    I2C1->CR1 &= ~I2C_CR1_SWRST;
    
    /* Configure I2C */
    I2C1->CR2 = 36; // Set APB1 clock frequency (36 MHz)
    I2C1->OAR1 |= (1 << 14); // Set own address
    I2C1->CCR = 180; // Set clock control register (100 kHz)
    I2C1->TRISE = 37; // Set maximum rise time

    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C_Start(void)
{
    I2C1->CR1 |= I2C_CR1_ACK;
    I2C1->CR1 |= I2C_CR1_START;

    uint32_t timeout = TIMEOUT;
    while (!(I2C1->SR1 & I2C_SR1_SB))
    {
        if (--timeout == 0) return; // Timeout handling
    }
}

void I2C_Write(uint8_t data)
{
    uint32_t timeout = TIMEOUT;
    while (!(I2C1->SR1 & I2C_SR1_TXE))
    {
        if (--timeout == 0) return; // Timeout handling
    }
    I2C1->DR = data;

    timeout = TIMEOUT;
    while (!(I2C1->SR1 & I2C_SR1_BTF))
    {
        if (--timeout == 0) return; // Timeout handling
    }
}

void I2C_Address(uint8_t address)
{
    uint8_t addr = (uint8_t)(address << 1);
    I2C1->DR = addr;

    uint32_t timeout = TIMEOUT;
    while (!(I2C1->SR1 & I2C_SR1_ADDR))
    {
        if (--timeout == 0) return; // Timeout handling
    }

    (void)I2C1->SR1;
    (void)I2C1->SR2;
}

void I2C_Stop(void)
{
    I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_WriteMul(uint8_t *data, uint8_t size)
{
    uint32_t timeout = TIMEOUT;
    while (!(I2C1->SR1 & I2C_SR1_TXE))
    {
        if (--timeout == 0) return; // Timeout handling
    }

    while (size)
    {
        timeout = TIMEOUT;
        while (!(I2C1->SR1 & I2C_SR1_TXE))
        {
            if (--timeout == 0) return; // Timeout handling
        }
        I2C1->DR = *data;
        data++;
        size--;
    }

    timeout = TIMEOUT;
    while (!(I2C1->SR1 & I2C_SR1_BTF))
    {
        if (--timeout == 0) return; // Timeout handling
    }
}

/* LCD Functions */
void LCD_Write(uint8_t address, uint8_t *data, int size)
{
    I2C_Start();
    I2C_Address(address);

    while (size)
    {
        uint32_t timeout = TIMEOUT;
        while (!(I2C1->SR1 & I2C_SR1_TXE))
        {
            if (--timeout == 0) return; // Timeout handling
        }
        I2C1->DR = *data;
        data++;
        size--;
    }

    uint32_t timeout = TIMEOUT;
    while (!(I2C1->SR1 & I2C_SR1_BTF))
    {
        if (--timeout == 0) return; // Timeout handling
    }

    I2C_Stop();
}

void lcd_send_cmd(char cmd)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C;  // en=1, rs=0
    data_t[1] = data_u | 0x08;  // en=0, rs=0
    data_t[2] = data_l | 0x0C;  // en=1, rs=0
    data_t[3] = data_l | 0x08;  // en=0, rs=0
    LCD_Write(SLAVE_ADDRESS_LCD, data_t, 4);
}

void lcd_send_data(char data)
{
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D;  // en=1, rs=1
    data_t[1] = data_u | 0x09;  // en=0, rs=1
    data_t[2] = data_l | 0x0D;  // en=1, rs=1
    data_t[3] = data_l | 0x09;  // en=0, rs=1
    LCD_Write(SLAVE_ADDRESS_LCD, data_t, 4);
}

void lcd_clear(void)
{
    lcd_send_cmd(0x80);
    for (int i = 0; i < 70; i++)
    {
        lcd_send_data(' ');
    }
}

void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    lcd_send_cmd(col);
}

void lcd_init(void)
{
    // 4 bit initialisation
    Delay_ms(50);  // wait for >40ms
    lcd_send_cmd(0x30);
    Delay_ms(5);  // wait for >4.1ms
    lcd_send_cmd(0x30);
    Delay_us(150);  // wait for >100us
    lcd_send_cmd(0x30);
    Delay_ms(10);
    lcd_send_cmd(0x20);  // 4bit mode
    Delay_ms(10);

    // display initialisation
    lcd_send_cmd(0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
    Delay_ms(1);
    lcd_send_cmd(0x08); // Display on/off control --> D=0,C=0, B=0  ---> display off
    Delay_ms(1);
    lcd_send_cmd(0x01);  // clear display
    Delay_ms(1);
    lcd_send_cmd(0x06); // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
    Delay_ms(1);
    lcd_send_cmd(0x0C); // Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
}

void lcd_send_string(char *str)
{
    while (*str) lcd_send_data(*str++);
}

/* Delay Functions */
void Delay_Config(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->ARR = 0xFFFF;
    TIM2->PSC = 71;
    TIM2->CR1 |= TIM_CR1_CEN;
    TIM2->EGR |= TIM_EGR_UG;
}

void Delay_us(uint32_t us)
{
    TIM2->CNT = 0;
    while(TIM2->CNT < us);
}

void Delay_ms(uint32_t ms)
{
    for(int i = 0; i < ms; i++)
    {
        Delay_us(1000);
    }
}

/* Main Function */
int main(void)
{
    Delay_Config();
    I2C_Config();
    lcd_init();
    
    lcd_put_cur(0, 0);
    lcd_send_string("hello Van Quyen");
    lcd_put_cur(1, 0);
    lcd_send_string("hello Fhong Thu");
    
    while (1)
    {
    }
}