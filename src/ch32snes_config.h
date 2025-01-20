/*
 * ch32snes_config.h
 * Configuration for SNES controller PMOD.
 *
 * Edit this file to set which pins are used for reporting data on the PMOD,
 * the timing and behaviour of reports and more.
 *
 *  Created on: Jan 19, 2025
 *      Author: Pat Deegan
 *    Copyright (C) 2025 Pat Deegan, https://psychogenic.com
 *    
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CH32SNES_CONFIG_H_
#define CH32SNES_CONFIG_H_


// makes PIN_PMOD_IO* and PIN_CONTROLLERS_* available, mapped to schem
#include "ch32snes_pinout.h"

/*
 * Map PIN_REPORT_* using the PIN_PMOD_IO[1-8] of your choice
 */
#define PIN_REPORT_LATCH			PIN_PMOD_IO4
#define PIN_REPORT_CLOCK			PIN_PMOD_IO5
#define PIN_REPORT_DATA				PIN_PMOD_IO6


/* REPORT_ONLY_ON_CHANGE: send reports only
 * if value has changed (1) or always (0)
 */
#define REPORT_ONLY_ON_CHANGE			1

/*
 * MINIMUM_REPORT_INTERVAL_SECONDS
 * If REPORT_ONLY_ON_CHANGE, a MINIMUM_REPORT_INTERVAL_SECONDS will
 * still shoot out a report this often.  May be fractional.
 */
#define MINIMUM_REPORT_INTERVAL_SECONDS		2

/*
 * CONTROLLERSET_NUM_CONTROLLERS: number of controllers to report for.
 * May be 1 or 2.  A total of CONTROLLERSET_NUM_CONTROLLERS*12 bits of
 * report will be sent after each latch.
 */
#define CONTROLLERSET_NUM_CONTROLLERS		2


/*
 * Reporter reports data back to you over PMOD header
 * These values set the baudrate of everything, namely:
 *  - width of the "latch" (this is a new data block) pulse
 *  - time delay after pulse to clock out data (may be 0)
 *  - the period of the clocked data
 * all in microseconds.
 */
#define REPORTER_LATCH_PULSE_WIDTH_US			10
#define REPORTER_LATCH_PULSE_POSTDELAY_US		0
#define REPORTER_CLOCK_PERIOD_US				8




/*
 * CH32SNES_DEBUG_LEVEL 0 - 2, 0 == disabled, 2 == verbose
 * With a programmer connected, see debug using
 *  ch32v003fun/minichlink/minichlink -T
 *
 */
#define CH32SNES_DEBUG_LEVEL	2



/*
 * SNES controlle protocol defaults.
 * 60Hz poll rate and these values seem to be the values used
 * by consoles.
 * Note the _MS (millis) and _US (micros) suffixen.
 */
#define CONTROLLER_POLLING_INTERVAL_MS			16
#define CONTROLLER_LATCH_PULSE_WIDTH_US			12
#define CONTROLLER_LATCH_PULSE_POSTDELAY_US		6
#define CONTROLLER_CLOCK_PERIOD_US				12

/*
 * CONTROLLER_RESPONSE_INVERTED -- 1==logic low for press
 */
#define CONTROLLER_RESPONSE_INVERTED			1


/*
 * We clock out 16 bits from controllers, but high 4 are always 1,
 * only 12 bits contain data
 */
#define CONTROLLER_STATUS_NUM_BITS				16
#define CONTROLLER_STATUS_NUM_BITS_TO_REPORT	12

/*
 * Default config for I/O. Using pull-ups on inputs by default.
 */
#define CONTROLLER_PIN_DEFAULTSETTINGS_INPUT 	(GPIO_Speed_In | GPIO_CNF_IN_PUPD)
#define CONTROLLER_PIN_DEFAULTSETTINGS_OUTPUT 	(GPIO_Speed_10MHz | GPIO_CNF_OUT_PP)


#endif /* CH32SNES_CONFIG_H_ */
