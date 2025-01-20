/*
 * controller.cpp
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

#include "controller.h"

namespace SNES {

Controller::Controller() : data(0xff)
{
}

Controller::Controller(Pin pin_data) : data(pin_data)
{
}

void Controller::begin() {

	funPinMode(data, CONTROLLER_PIN_DEFAULTSETTINGS_INPUT);
}

void Controller::debugStatusOutput() {

#if CH32SNES_DEBUG_LEVEL_VERBOSE()
		DEBUG_OUT_VERBOSE("Controller status: ");
		if (! current_status.rawbits) {
			DEBUG_OUT_VERBOSE("all OFF\n");
		} else {

			if (current_status.L) {
				DEBUG_OUT_VERBOSE("L ");
			}

			if (current_status.R) {
				DEBUG_OUT_VERBOSE("R ");
			}

			if (current_status.A) {
				DEBUG_OUT_VERBOSE("A ");
			}

			if (current_status.B) {
				DEBUG_OUT_VERBOSE("B ");
			}
			if (current_status.X) {
				DEBUG_OUT_VERBOSE("X ");
			}
			if (current_status.Y) {
				DEBUG_OUT_VERBOSE("Y ");
			}
			if (current_status.down) {
				DEBUG_OUT_VERBOSE("down ");
			}
			if (current_status.left) {
				DEBUG_OUT_VERBOSE("left ");
			}
			if (current_status.right) {
				DEBUG_OUT_VERBOSE("right ");
			}
			if (current_status.up) {
				DEBUG_OUT_VERBOSE("up ");
			}
			if (current_status.select) {
				DEBUG_OUT_VERBOSE("select ");
			}
			if (current_status.start) {
				DEBUG_OUT_VERBOSE("start ");
			}

			DEBUG_OUT_VERBOSE("\n");
		}
#else
		DEBUG_OUT("Controller status updated\n");
#endif
}
bool Controller::setStatus(const Controller::Status & stat) {
	if (current_status.rawbits != stat.rawbits) {
		current_status.rawbits = stat.rawbits;
		debugStatusOutput();
		return true;
	}
	return false;
}

bool Controller::inData() {
	if (funDigitalRead(data)) {
#if CONTROLLER_RESPONSE_INVERTED
		return false;
#else
		return true;
#endif
	}
#if CONTROLLER_RESPONSE_INVERTED
	return true;
#else
	return false;
#endif
}

} /* namespace SNES */
