/*
 * PMODport.h, part of the CH32SNES project
 * A representation of an output PMOD, mainly just for debugging/test.
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

#ifndef PMODPORT_H_
#define PMODPORT_H_

#include "ch32snes_types.h"

#define PMOD_NUM_PINS_IN_PORT		8
namespace PMOD {

class Port {
public:
	Port(Pin io1, Pin io2, Pin io3, Pin io4,
			Pin io5, Pin io6, Pin io7, Pin io8);
	void begin();

	uint8_t read();
	void write(uint8_t v);

private:
	Pin portpins[PMOD_NUM_PINS_IN_PORT];

};

} /* namespace PMOD */

#endif /* PMODPORT_H_ */
