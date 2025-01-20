/*
 * ch32snes.cpp
 * Main driver for the module.
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

#include "ch32snes_includes.h"
#include "controllerset.h"
#include "reportport.h"
#include "PMODport.h"
#include <stdio.h>

// define TEST_PMOD /* used to cycle through PMOD pins and do QA, ignore */
#ifdef TEST_PMOD
void testPMOD();
#endif

#define MAX_SKIPPED_REPORTS ((int)(MINIMUM_REPORT_INTERVAL_SECONDS * (1000/CONTROLLER_POLLING_INTERVAL_MS)))

int main() {
	SystemInit();

	GPIO_port_enable(GPIO_port_C);
	GPIO_port_enable(GPIO_port_D);

	DEBUG_OUT("CH32SNES boot...");

#ifdef TEST_PMOD
	DEBUG_OUT("PMOD test running...\n");
	// will get stuck in this infinite loop
	testPMOD();
#endif


	SNES::ControllerSet controllers(PIN_CONTROLLERS_LATCH,
			PIN_CONTROLLERS_CLOCK,
			PIN_CONTROLLERS_DATA1,
			PIN_CONTROLLERS_DATA2);


	PMOD::ReportPort reporter(PIN_REPORT_LATCH ,
			PIN_REPORT_CLOCK,
			PIN_REPORT_DATA);


	controllers.begin();
	reporter.begin();
	DEBUG_OUT("started\n");

	uint16_t numSkipped = 0;
	while (true) {
		if (controllers.poll() || !(REPORT_ONLY_ON_CHANGE)) {
			const SNES::ControllerSet::StateBits & curstate = controllers.state();
			reporter.send(curstate.state, curstate.num);
			numSkipped = 0;
		} else {

			if (numSkipped < MAX_SKIPPED_REPORTS) {
				numSkipped ++;
			} else {
				const SNES::ControllerSet::StateBits & curstate = controllers.state();
				reporter.send(curstate.state, curstate.num);
				numSkipped = 0;
			}
		}
		Delay_Ms(CONTROLLER_POLLING_INTERVAL_MS);
	}
}


#ifdef TEST_PMOD
void testPMOD() {
	PMOD::Port pmod(PIN_PMOD_IO1, PIN_PMOD_IO2, PIN_PMOD_IO3, PIN_PMOD_IO4,
					PIN_PMOD_IO5, PIN_PMOD_IO6, PIN_PMOD_IO7, PIN_PMOD_IO8);
	pmod.begin();
	while (true) {
		for (uint8_t i=0; i<8; i++) {
			uint8_t v = 1 << i;
			pmod.write(v);

			DEBUG_OUT("Wrote %u, read %u\n", v, pmod.read());
			Delay_Ms(20);
		}
	}
}
#endif



