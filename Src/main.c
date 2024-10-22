/*
 *
 * PA5 as internal LED pin: On - On, Off - Off
 * PC13 as user-button pin
 *
 */
#include <stm32f0xx.h>
#include <stm32f030x8.h>

#include <stdint.h>

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void Init_Clock();
void Init_Pin();

volatile uint8_t i = 0;

int main() {
	Init_Clock();
	Init_Pin();

	while (1) {
//		GPIOA->BSRR  |= (1 << 21);
//		for (volatile int i = 0; i < 60000; i++) {
//			asm("nop");
//		}
//		GPIOA->BSRR |= (1 << 5);
//		for (volatile int i = 0; i < 60000; i++) {
//			asm("nop");
//		}
	}
}

void Init_Clock() {
	RCC->CR |= (1 << 0); /* HSI on */
	while (((RCC->CR >> 1) & 1UL) == 0) {

	}

//	FLASH->ACR &= ~(7 << 0); /* Zero wait state */

	RCC->CFGR &= ~(1 << 7); /* HCLK not divided */
	RCC->CFGR &= ~(1 << 10); /* PCLK not divided */

	RCC->CFGR &= ~(1 << 3); /* Use HSI */
	RCC->CFGR &= ~(1 << 2);
}

void Init_Pin() {
	RCC->AHBENR |= (1 << 17); /* CLK to PORTA */
	RCC->AHBENR |= (1 << 19); /* CLK to PORTC */

	GPIOA->MODER &= ~(1 << 11);
	GPIOA->MODER |= (1 << 10); /* PA5 as generic output */

	GPIOC->MODER &= ~(3 << 26); /* PC13 as input */
	GPIOC->PUPDR &= ~(3 << 26);
	GPIOC->PUPDR |= (1 << 27); /* PC13 as pull-down */

	GPIOA->ODR |= (1 << 5);

	RCC->APB2ENR |= (1 << 0); /* CLK to SYSCFG */

	SYSCFG->EXTICR[3] &= ~(15 << 4);
	SYSCFG->EXTICR[3] |= (1 << 5); /* PC13 as EXTI */
	EXTI->IMR |= (1 << 13); /* EXTI line 13 unmasked */
	EXTI->RTSR |= (1 << 13); /* EXTI line 13 interrupt-on-rising edge */
	EXTI->FTSR &= ~(1 << 13); /* EXTI line 13 NOT interrupt-on-falling edge */

	NVIC_SetPriority (EXTI4_15_IRQn, 1);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

void EXTI4_15_IRQHandler() {
	if (i == 1) {
		i = 0;
		GPIOA->BSRR  |= (1 << 21);
	} else {
		i = 1;
		GPIOA->BSRR  |= (1 << 5);
	}
	EXTI->PR |= (1 << 13);
}
