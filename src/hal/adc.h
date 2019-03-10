#ifndef _H_ADC_
#define _H_ADC_

#include "gpio.h"

#define GPIO_ADC_CURRENT_A		XGPIO_DEF3('A', 2, 2)
#define GPIO_ADC_CURRENT_B		XGPIO_DEF3('A', 1, 1)
#define GPIO_ADC_VOLTAGE_U		XGPIO_DEF3('A', 0, 0)
#define GPIO_ADC_VOLTAGE_A		XGPIO_DEF3('A', 4, 4)
#define GPIO_ADC_VOLTAGE_B		XGPIO_DEF3('A', 5, 5)
#define GPIO_ADC_VOLTAGE_C		XGPIO_DEF3('A', 6, 6)
#define GPIO_ADC_PCB_NTC		XGPIO_DEF3('A', 3, 3)
#define GPIO_ADC_EXT_NTC		XGPIO_DEF3('A', 3, 3)
#define GPIO_ADC_ANALOG			XGPIO_DEF3('C', 3, 3)
#define GPIO_ADC_INTERNAL_TEMP		XGPIO_DEF3('J', 0, 0)

void ADC_irq_lock();
void ADC_irq_unlock();

void ADC_startup();
float ADC_get_VALUE(int xGPIO);

extern void ADC_IRQ();

#endif /* _H_ADC_ */

