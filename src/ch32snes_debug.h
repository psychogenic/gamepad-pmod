/*
 * ch32snes_debug.h, part of the CH32SNES project
 * Simple DEBUG_OUT* macros.  Their behaviour depends on the
 * CH32SNES_DEBUG_LEVEL set in ch32snes_config.h
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

#ifndef CH32SNES_DEBUG_H_
#define CH32SNES_DEBUG_H_

#include "ch32snes_config.h"


#define CH32SNES_DEBUG_LEVEL_VERBOSE() (CH32SNES_DEBUG_LEVEL > 1)

#if CH32SNES_DEBUG_LEVEL
#define DEBUG_OUT(...)		printf(__VA_ARGS__)

#if CH32SNES_DEBUG_LEVEL > 1
#define DEBUG_OUT_VERBOSE(...)  printf(__VA_ARGS__)
#else
#define DEBUG_OUT_VERBOSE(...)
#endif /* debug verbose */

#else
#define DEBUG_OUT(...)
#define DEBUG_OUT_VERBOSE(...)
#endif

#endif /* CH32SNES_DEBUG_H_ */
