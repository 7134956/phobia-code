/*
   Phobia Motor Controller for RC and robotics.
   Copyright (C) 2015 Roman Belov <romblv@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "cmsis/stm32f4xx.h"
#include "hal.h"

halUSART_t	 		halUSART;

void irqUSART1() { }

void irqDMA2_Stream7()
{
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTEIF7
		| DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CFEIF7;
}

void usartEnable()
{
	/* Enable USART1 clock.
	 * */
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	/* Enable DMA2 clock.
	 * */
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

	/* Enable PB6 (TX), PB7 (RX) pins.
	 * */
	MODIFY_REG(GPIOB->AFR[0], (15UL << 28) | (15UL << 24),
			(7UL << 28) | (7UL << 24));
	MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER6 | GPIO_MODER_MODER7,
			GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);

	/* Configure USART.
	 * */
	USART1->BRR = HZ_APB2 / halUSART.baudRate;
	USART1->CR1 = USART_CR1_UE | USART_CR1_M | USART_CR1_PCE
		| USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE;
	USART1->CR2 = 0;
	USART1->CR3 = USART_CR3_DMAT | USART_CR3_DMAR;

	/* Flush RX buffer.
	 * */
	halUSART.rN = 0;

	/* Configure DMA for RX.
	 * */
	DMA2->HIFCR |= DMA_HIFCR_CTCIF5 | DMA_HIFCR_CHTIF5 | DMA_HIFCR_CTEIF5
		| DMA_HIFCR_CDMEIF5 | DMA_HIFCR_CFEIF5;
	DMA2_Stream5->PAR = (unsigned int) &USART1->DR;
	DMA2_Stream5->M0AR = (unsigned int) halUSART.RX;
	DMA2_Stream5->NDTR = USART_RXBUF_SZ;
	DMA2_Stream5->FCR = 0;
	DMA2_Stream5->CR = DMA_SxCR_CHSEL_2 | DMA_SxCR_PL_0 | DMA_SxCR_MINC
		| DMA_SxCR_CIRC;

	DMA2_Stream5->CR |= DMA_SxCR_EN;

	/* Configure DMA for TX.
	 * */
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTEIF7
		| DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CFEIF7;
	DMA2_Stream7->PAR = (unsigned int) &USART1->DR;
	DMA2_Stream7->M0AR = (unsigned int) halUSART.TX;
	DMA2_Stream7->NDTR = 0;
	DMA2_Stream7->FCR = 0;
	DMA2_Stream7->CR = DMA_SxCR_CHSEL_2 | DMA_SxCR_PL_0 | DMA_SxCR_MINC
		| DMA_SxCR_DIR_0 | DMA_SxCR_TCIE;

	/* Enable IRQs.
	 * */
	NVIC_SetPriority(DMA2_Stream7_IRQn, 11);
	NVIC_SetPriority(USART1_IRQn, 11);
	NVIC_EnableIRQ(DMA2_Stream7_IRQn);
	NVIC_EnableIRQ(USART1_IRQn);
}

void uartDisable()
{
	/* Disable IRQs.
	 * */
	NVIC_DisableIRQ(DMA2_Stream7_IRQn);
	NVIC_DisableIRQ(USART1_IRQn);

	/* Disable DMA.
	 * */
	DMA2_Stream5->CR = 0;

	/* Disable USART.
	 * */
	USART1->CR1 = 0;

	/* Disable PB6, PB7 pins.
	 * */
	MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER6 | GPIO_MODER_MODER7, 0);

	/* Disable DMA1 clock.
	 * */
	RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA2EN;

	/* Disable USART1 clock.
	 * */
	RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
}

int usartRecv()
{
	int	rN, rW, xC;

	rN = halUSART.rN;
	rW = USART_RXBUF_SZ - DMA2_Stream5->NDTR;

	if (rN != rW) {

		/* There are data.
		 * */
		xC = halUSART.RX[rN];
		halUSART.rN = (rN < (USART_RXBUF_SZ - 1)) ? rN + 1 : 0;
	}
	else
		xC = -1;

	return xC;
}

int usartPoll()
{
	return (DMA2_Stream7->CR & DMA_SxCR_EN) ? 0 : 1;
}

void usartPushAll(int N)
{
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7 | DMA_HIFCR_CHTIF7 | DMA_HIFCR_CTEIF7
		| DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CFEIF7;
	DMA2_Stream7->NDTR = N;
	DMA2_Stream7->CR |= DMA_SxCR_EN;
}

