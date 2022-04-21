
#include "serial.h"
#include "stm32f1xx.h"

void serial_init(void)
{
    /* enable clock in GPIOA */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL |= GPIO_CRL_CNF2_1 | GPIO_CRL_MODE2_1;
    GPIOA->CRL |= GPIO_CRL_CNF3_1 | GPIO_CRL_MODE3_1;

    /* enable clock in UART2 */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    /*  Baud rate
			BRR should be 39.0625 for 115200 baud rate in 72MHZ
			Thus manista is 39 (0x27) and fraction is 1 (0x01) (1/16 is .0625)
			Making it 0x0D01 */
    USART2->BRR = 0x0D01;
    USART2->CR1 = 0x0000;
    /*  Word length 8 data */
    USART2->CR1 &= ~USART_CR1_M;
    /*  1 stop bits */
    USART2->CR2 &= ~USART_CR2_STOP;
    /* USART enable */
    USART2->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;

    /* Interrupt rx serial */
    USART2->CR1 |= USART_CR1_RXNEIE;
    /* Set priority */
    NVIC->IP[USART2_IRQn] = 0x10;
}

void USART2_IRQHandler(void)
{
}