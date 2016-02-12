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

halPWM_t			halPWM;

void irqTIM8_UP_TIM13() { }

void pwmEnable()
{
	int		R, D;

	/* Update configuration.
	 * */
	R = HZ_APB2 * 2UL / 2UL / halPWM.freq_hz;
	R = (R & 1) ? R + 1 : R;
	halPWM.freq_hz = HZ_APB2 * 2UL / 2UL / R;
	halPWM.resolution = R;

	D = ((HZ_APB2 * 2UL / 1000UL) * halPWM.dead_time_ns + 500000UL) / 1000000UL;
	D = (D < 128) ? D : (D < 256) ? 128 + (D - 128) / 2 : 191;
	halPWM.dead_time_tk = ((D < 128) ? D : (D < 192) ? 128 + (D - 128) * 2 : 255);
	halPWM.dead_time_ns = halPWM.dead_time_tk * 1000000UL / (HZ_APB2 * 2UL / 1000UL);

	/* Enable TIM8 clock.
	 * */
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;

	/* Configure TIM8.
	 * */
	TIM8->CR1 = TIM_CR1_ARPE | TIM_CR1_CMS_0 | TIM_CR1_CMS_1;
	TIM8->CR2 = TIM_CR2_MMS_1 | TIM_CR2_CCPC;
	TIM8->SMCR = 0;
	TIM8->DIER = 0;
	TIM8->CCMR1 = TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE
		| TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
	TIM8->CCMR2 = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
	TIM8->CCER = TIM_CCER_CC3NE | TIM_CCER_CC3E | TIM_CCER_CC2NE
		| TIM_CCER_CC2E | TIM_CCER_CC1NE | TIM_CCER_CC1E;
	TIM8->CNT = 0;
	TIM8->PSC = 0;
	TIM8->ARR = R;
	TIM8->RCR = 0;
	TIM8->CCR1 = 0;
	TIM8->CCR2 = 0;
	TIM8->CCR3 = 0;
	TIM8->CCR4 = 0;
	TIM8->BDTR = TIM_BDTR_MOE | TIM_BDTR_OSSR | D;

	/* Start TIM8.
	 * */
	TIM8->EGR |= TIM_EGR_COMG | TIM_EGR_UG;
	TIM8->CR1 |= TIM_CR1_CEN;
	TIM8->RCR = 1;

	/* Enable PC6 PC7 PC8 PA7 PB0 PB1 pins.
	 * */
	MODIFY_REG(GPIOC->AFR[0], (15UL << 24) | (15UL << 28),
			(3UL << 0) | (3UL << 4));
	MODIFY_REG(GPIOC->AFR[1], (15UL << 0), (3UL << 8));
	MODIFY_REG(GPIOA->AFR[0], (15UL << 28), (3UL << 28));
	MODIFY_REG(GPIOB->AFR[0], (15UL << 0) | (15UL << 4),
			(3UL << 0) | (3UL << 4));
	MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7
			| GPIO_OSPEEDER_OSPEEDR8,
			GPIO_OSPEEDER_OSPEEDR6_0 | GPIO_OSPEEDER_OSPEEDR7_0
			| GPIO_OSPEEDER_OSPEEDR8_0);
	MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR7, GPIO_OSPEEDER_OSPEEDR7_0);
	MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1,
			GPIO_OSPEEDER_OSPEEDR0_0 | GPIO_OSPEEDER_OSPEEDR1_0);
	MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODER6 | GPIO_MODER_MODER7
			| GPIO_MODER_MODER8,
			GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1
			| GPIO_MODER_MODER8_1);
	MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER7, GPIO_MODER_MODER7_1);
	MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER0 | GPIO_MODER_MODER1,
			GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1);
}

void pwmDisable()
{
	/* Disable pins.
	 * TODO
	 * */
	/*MODIFY_REG(GPIOE->MODER, GPIO_MODER_MODER8 | GPIO_MODER_MODER9
			| GPIO_MODER_MODER10 | GPIO_MODER_MODER11
			| GPIO_MODER_MODER12 | GPIO_MODER_MODER13, 0);
	MODIFY_REG(GPIOE->AFR[1], (15UL << 0) | (15UL << 4)
			| (15UL << 8) | (15UL << 12)
			| (15UL << 16) | (15UL << 20), 0);*/

	/* Disable TIM8.
	 * */
	TIM8->CR1 = 0;
	TIM8->CR2 = 0;

	/* Disable clock.
	 * */
	RCC->APB2ENR &= ~RCC_APB2ENR_TIM8EN;
}

void pwmDC(int uA, int uB, int uC)
{
	TIM8->CCR1 = uA;
	TIM8->CCR2 = uB;
	TIM8->CCR3 = uC;
}

void pwmZ(int Z)
{
	if (Z & PWM_A) {

		TIM8->CCER &= ~TIM_CCER_CC1NE;
		TIM8->CCMR1 &= ~TIM_CCMR1_OC1M_1;
	}
	else {
		TIM8->CCER |= TIM_CCER_CC1NE;
		TIM8->CCMR1 |= TIM_CCMR1_OC1M_1;
	}

	if (Z & PWM_B) {

		TIM8->CCER &= ~TIM_CCER_CC2NE;
		TIM8->CCMR1 &= ~TIM_CCMR1_OC2M_1;
	}
	else {
		TIM8->CCER |= TIM_CCER_CC2NE;
		TIM8->CCMR1 |= TIM_CCMR1_OC2M_1;
	}

	if (Z & PWM_C) {

		TIM8->CCER &= ~TIM_CCER_CC3NE;
		TIM8->CCMR2 &= ~TIM_CCMR2_OC3M_1;
	}
	else {
		TIM8->CCER |= TIM_CCER_CC3NE;
		TIM8->CCMR2 |= TIM_CCMR2_OC3M_1;
	}

	TIM8->EGR |= TIM_EGR_COMG;
}

