#include <stddef.h>

#include "freertos/FreeRTOS.h"
#include "hal/hal.h"

#include "phobia/libm.h"

#include "libc.h"
#include "main.h"
#include "shell.h"

void apDEMO(void *pData)
{
	int			led_counter = 0;
	int			timeout = 0, action = 0;

	pm_fsm_req(&pm, PM_STATE_LU_INITIATE);
	pm_wait_for_IDLE();

	do {
		vTaskDelay((TickType_t) 10);

		if ((led_counter * .2f - .9f) < pm.lu_X[2]) {

			GPIO_set_HIGH(GPIO_LED);
		}
		else {
			GPIO_set_LOW(GPIO_LED);
		}

		led_counter = (led_counter < 9) ? led_counter + 1 : 0;

		if (timeout == 0 && m_fabsf(pm.lu_X[1]) > 5.f) {

			switch (action) {

				case 0:
					reg_SET_F(ID_PM_S_SETPOINT_RPM, 90.f);
					action = 1;
					break;

				case 1:
					reg_SET_F(ID_PM_S_SETPOINT_RPM, - 90.f);
					action = 2;
					break;

				case 2:
					reg_SET_F(ID_PM_S_SETPOINT_RPM, 0.f);
					action = 0;
					break;
			}

			timeout = 100;
		}

		timeout = (timeout > 0) ? timeout - 1 : 0;

	}
	while (1);
}

static TaskHandle_t		xHandle;

SH_DEF(ap_demo_startup)
{
	if (xHandle == NULL) {

		xTaskCreate(apDEMO, "apDEMO", configMINIMAL_STACK_SIZE, NULL, 1, &xHandle);
	}
}

SH_DEF(ap_demo_halt)
{
	if (xHandle != NULL) {

		vTaskDelete(xHandle);
		xHandle = NULL;
	}
}

