/*
 * reportport.h
 * A simplified and usecase-specific PMOD output port, with only 3 lines
 *  - latch
 *  - clock
 *  - data
 * Report bits are sent using send().  For each send(), the latch will pulse,
 * then every bit will be clocked out on clock/data.  Data transitions on clock going
 * high and is to be sampled as clock transitions low.
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

#ifndef REPORTPORT_H_
#define REPORTPORT_H_

#include "ch32snes_types.h"

namespace PMOD {

class ReportPort {
public:
	ReportPort(Pin latch, Pin clock, Pin data);

	void begin();

	void send(uint32_t data, uint8_t num_bits);


private:
	Pin latch;
	Pin clock;
	Pin data;
};

} /* namespace PMOD */

#endif /* REPORTPORT_H_ */
