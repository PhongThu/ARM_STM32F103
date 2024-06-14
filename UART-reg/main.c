#include "stm32f10x.h"
#include "uart.h"
#include "SysClock.h"

int main(void) {
	SysClkConf();
	UART_Config();
	while(1) {
		UART_Transmit("Hello");
		delay(1000);
	}
}
