
#include "serial.h"

void serial_init(void)
{
    /* enable clock in GPIOA */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2_1;
    GPIOA->CRL |= GPIO_CRL_CNF3_1 | GPIO_CRL_MODE3_1;

    /* enable clock in UART2 */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    /*  Baud rate
			BRR should be 208.333 for 9600 baud rate in 32MHZ
			Thus manista is 208 (0xD0) and fraction is 5 (0x05) (5/16 is .333)
			Making it 0x0D05 */
    USART2->BRR = 0x0D05;
    USART2->CR1 = 0x0000;
    /*  Word length 8 data */
    USART2->CR1 &= ~USART_CR1_M;
    /*  1 stop bits */
    USART2->CR2 &= ~USART_CR2_STOP;
    /* USART enable */
    USART2->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
}

uint8_t write(char data)
{
    USART2->DR = (data & (uint16_t)0x01FF);
    while ((USART2->SR & USART_SR_TXE) == (uint16_t)0);
		return 1;
}
