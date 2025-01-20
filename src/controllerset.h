/*
 * controllerset.h
 * Main handle to all the controllers present.  Assumes that the
 * latch and clock lines are shared by all SNES::Controller instances.
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

#ifndef CONTROLLERSET_H_
#define CONTROLLERSET_H_

#include "ch32snes_types.h"
#include "controller.h"

namespace SNES {


#if CONTROLLERSET_NUM_CONTROLLERS < 1
#error "Need at least one controller, y'all"
#elif CONTROLLERSET_NUM_CONTROLLERS > 2
#error "Only setup to handle to controllers, max"
#endif


class ControllerSet {
public:

	/* current state represented as 'num' bits
	 * in 'state', from 0th to (num-1) bit.
	 */
	typedef struct {
		uint8_t num;
		uint32_t state;
	} StateBits;


	ControllerSet(Pin latch, Pin clock, Pin data1, Pin data2);

	void begin();

	bool poll();

	const StateBits & state();


private:
	Pin latch;
	Pin clock;
	Controller controllers[CONTROLLERSET_NUM_CONTROLLERS];

};

} /* namespace SNES */

#endif /* CONTROLLERSET_H_ */
