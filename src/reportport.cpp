/*
 * reportport.cpp
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

#include "reportport.h"

namespace PMOD {

ReportPort::ReportPort(Pin pin_latch, Pin pin_clock, Pin pin_data) :
	latch(pin_latch), clock(pin_clock), data(pin_data)
{
}

void ReportPort::begin() {
	funPinMode(latch, CONTROLLER_PIN_DEFAULTSETTINGS_OUTPUT);
	funPinMode(clock, CONTROLLER_PIN_DEFAULTSETTINGS_OUTPUT);
	funPinMode(data, CONTROLLER_PIN_DEFAULTSETTINGS_OUTPUT);
}


#if REPORTER_CLOCK_PERIOD_US < 2
#define REPORT_DELAY_HALFCLOCK() 	DelaySysTick( (REPORTER_CLOCK_PERIOD_US * DELAY_US_TIME)/2 )
#else
#define REPORT_DELAY_HALFCLOCK() 	Delay_Us(REPORTER_CLOCK_PERIOD_US/2)
#endif


void ReportPort::send(uint32_t out_data, uint8_t num_bits) {


	for (uint8_t i=0; i<num_bits; i++) {
		if (out_data & (1<<i)) {

			funDigitalWrite(data, FUN_HIGH);
		} else {
			funDigitalWrite(data, FUN_LOW);
		}
		funDigitalWrite(clock, FUN_HIGH);
		REPORT_DELAY_HALFCLOCK();
		funDigitalWrite(clock, FUN_LOW);
		REPORT_DELAY_HALFCLOCK();
	}


	funDigitalWrite(latch, FUN_HIGH);
	Delay_Us(REPORTER_LATCH_PULSE_WIDTH_US);
	funDigitalWrite(latch, FUN_LOW);
#if REPORTER_LATCH_PULSE_POSTDELAY_US
	Delay_Us(REPORTER_LATCH_PULSE_POSTDELAY_US);
#endif

	// idle low
	funDigitalWrite(data, FUN_LOW);





}



} /* namespace PMOD */
