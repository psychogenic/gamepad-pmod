/*
 * controller.h
 * Represents a single SNES controller.
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

#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "ch32gamepad_types.h"

namespace SNES {

class Controller {
public:
	typedef union {
		struct {
			unsigned B: 1;
			unsigned Y: 1;
			unsigned select: 1;
			unsigned start: 1;
			unsigned up: 1;
			unsigned down: 1;
			unsigned left: 1;
			unsigned right: 1;
			unsigned A: 1;
			unsigned X: 1;
			unsigned L: 1;
			unsigned R: 1;
			unsigned res: 4;

		};

		uint16_t	rawbits;


	} Status;

	Controller();
	Controller(Pin data);

	void begin();

	bool inData();

	const Status & status() { return current_status;}
	bool is_present() { return (current_status.rawbits & 0xfff) != 0xfff;}
	/* setStatus returns TRUE if there was an actual change */
	bool setStatus(const Status & stat);


private:
	void debugStatusOutput();

	Pin data;
	Status current_status;
};



} /* namespace SNES */

#endif /* CONTROLLER_H_ */
