#ifndef _H_MAIN_
#define _H_MAIN_

#include "phobia/pm.h"
#include "phobia/libm.h"

#include "libc.h"
#include "ntc.h"
#include "tel.h"

typedef struct {

	/* Serial IO interfaces.
	 * */
	io_ops_t		io_USART;
	io_ops_t		io_CAN;

	/* CAN interface.
	 * */
	int			can_node_ID;

	/* PPM interface.
	 * */
	int			ppm_reg_ID;
	int			ppm_locked;
	float			ppm_pulse_cached;
	float			ppm_pulse_range[3];
	float			ppm_pulse_lost[2];
	float			ppm_control_range[3];
	float			ppm_startup_range[2];

	/* Analog interface.
	 * */
	int			analog_enabled;
	int			analog_reg_ID;
	int			analog_locked;
	float			analog_voltage_ratio;
	float			analog_timeout;
	float			analog_voltage_ANALOG[3];
	float			analog_voltage_BRAKE[3];
	float			analog_voltage_lost[2];
	float			analog_control_ANALOG[3];
	float			analog_control_BRAKE[3];
	float			analog_startup_range[2];

	/* CPU load.
	 * */
	int			lc_flag;
	int			lc_tick;
	int			lc_idle;

	/* NTC constants.
	 * */
	ntc_t			ntc_PCB;
	ntc_t			ntc_EXT;

	/* Thermal info.
	 * */
	float			temp_PCB;
	float			temp_EXT;
	float			temp_INT;

	/* Heat control.
	 * */
	float			heat_PCB;
	float			heat_PCB_derated_i;
	float			heat_EXT;
	float			heat_EXT_derated_i;
	float			heat_PCB_FAN;
	float			heat_gap;

	/* Load cell.
	 * */
	float			pull_g;
	float			pull_ad[2];
}
application_t;

extern application_t		ap;
extern pmc_t			pm;
extern tel_t			ti;

extern int flash_block_load();
extern int pm_wait_for_IDLE();

float ADC_get_ANALOG();
float ADC_get_BRAKE();

void lowTRACE(const char *fmt, ...);

#endif /* _H_MAIN_ */

