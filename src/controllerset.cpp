/*
 * controllerset.cpp
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

#include "controllerset.h"

namespace SNES {


ControllerSet::ControllerSet(Pin pin_latch, Pin pin_clock,
		Pin data1, Pin data2):
				latch(pin_latch),
				clock(pin_clock)
{
	controllers[0] = Controller(data1);
#if CONTROLLERSET_NUM_CONTROLLERS > 1
	controllers[1] = Controller(data2);
#endif
}

void ControllerSet::begin() {
	for (uint8_t i=0; i<CONTROLLERSET_NUM_CONTROLLERS; i++) {
		controllers[i].begin();
	}

	funPinMode(clock, CONTROLLER_PIN_DEFAULTSETTINGS_OUTPUT);
	funPinMode(latch, CONTROLLER_PIN_DEFAULTSETTINGS_OUTPUT);

}

bool ControllerSet::poll() {
	Controller::Status polled_statii[CONTROLLERSET_NUM_CONTROLLERS] = {0};
	funDigitalWrite(latch, FUN_HIGH);
	Delay_Us(CONTROLLER_LATCH_PULSE_WIDTH_US);
	funDigitalWrite(latch, FUN_LOW);
	Delay_Us(CONTROLLER_LATCH_PULSE_POSTDELAY_US);

	for (uint8_t ctrl=0; ctrl<CONTROLLERSET_NUM_CONTROLLERS; ctrl++) {
		polled_statii[ctrl].rawbits = 0;
	}

	for (uint8_t i=0; i< (CONTROLLER_STATUS_NUM_BITS - 1); i++) {
		// first bit is present after latching
		for (uint8_t ctrl=0; ctrl<CONTROLLERSET_NUM_CONTROLLERS; ctrl++) {
			if (controllers[ctrl].inData()) {
				polled_statii[ctrl].rawbits |= (1 << i);
			}
		}
		funDigitalWrite(clock, FUN_HIGH);
		Delay_Us(CONTROLLER_CLOCK_PERIOD_US/2);
		funDigitalWrite(clock, FUN_LOW);

		Delay_Us(CONTROLLER_CLOCK_PERIOD_US/2);
	}

	bool haveChanges = false;
	for (uint8_t ctrl=0; ctrl<CONTROLLERSET_NUM_CONTROLLERS; ctrl++) {
		if (controllers[ctrl].setStatus(polled_statii[ctrl])) {
			haveChanges = true;
			DEBUG_OUT("controller %u changed to %u\n", ctrl, polled_statii[ctrl].rawbits);

		}
	}


	return haveChanges;
}


const ControllerSet::StateBits & ControllerSet::state() {
	static StateBits rep = {0};

	static uint16_t controller_status_mask = 0;

	if (controller_status_mask == 0) {
		rep.num = CONTROLLER_STATUS_NUM_BITS_TO_REPORT*CONTROLLERSET_NUM_CONTROLLERS;
		for (uint8_t i=0; i< CONTROLLER_STATUS_NUM_BITS_TO_REPORT; i++) {
			controller_status_mask |= (1 << i);
		}
	}

	rep.state = 0;
	uint8_t numpresent = 0;
	bool present[CONTROLLERSET_NUM_CONTROLLERS];
	for (uint8_t ctrl=0; ctrl<CONTROLLERSET_NUM_CONTROLLERS; ctrl++) {
		rep.state |=  ( (controllers[ctrl].status().rawbits & controller_status_mask) << (ctrl*CONTROLLER_STATUS_NUM_BITS_TO_REPORT));
		present[ctrl] = controllers[ctrl].is_present();
		if (present[ctrl]) {
			numpresent++;
		}
	}
#ifdef REPORTER_SHIFT_SINGLE_CTRL_PRESENT_TO_LAST_BITS
#if CONTROLLERSET_NUM_CONTROLLERS > 1
	// Always send all bits, but move things around if necessary
	// rep.num = CONTROLLER_STATUS_NUM_BITS_TO_REPORT * (numpresent ? numpresent : 1);
	if (present[0] && ! present[1]) {
		// shift the bits around so controller 0 is sent last
		// in order to accomodate systems that only expect a
		// single controller and have only 12 flops to store data
		rep.state = (controllers[0].status().rawbits << (CONTROLLER_STATUS_NUM_BITS_TO_REPORT)) | controller_status_mask;
	}
#endif
#endif

	return rep;
}

} /* namespace SNES */
