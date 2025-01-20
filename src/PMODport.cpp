/*
 * PMODport.cpp, part of the CH32SNES project
 *
 *  Created on: Jan 20, 2025
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

#include "PMODport.h"

namespace PMOD {

Port::Port(Pin io1, Pin io2, Pin io3, Pin io4, Pin io5, Pin io6, Pin io7,
		Pin io8) {
	Pin pinlist[] = {io1, io2, io3, io4, io5, io6, io7, io8};
	for (uint8_t i=0; i<PMOD_NUM_PINS_IN_PORT; i++) {
		portpins[i] = pinlist[i];
	}

}

void Port::begin() {
	for (uint8_t i=0; i<PMOD_NUM_PINS_IN_PORT; i++) {
		funPinMode(portpins[i], CONTROLLER_PIN_DEFAULTSETTINGS_OUTPUT);
	}
}
uint8_t Port::read() {
	uint8_t v = 0;
	for (uint8_t i=0; i<PMOD_NUM_PINS_IN_PORT; i++) {
		v |= (funDigitalRead(portpins[i]) << i);
	}

	return v;

}

void Port::write(uint8_t v) {

	for (uint8_t i=0; i<PMOD_NUM_PINS_IN_PORT; i++) {
		funDigitalWrite(portpins[i], (v & (1 << i)) ? FUN_HIGH : FUN_LOW);
	}

}

} /* namespace PMOD */
