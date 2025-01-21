/*
 * ch32snes_pinout.h, part of the CH32SNES project
 * Schematic-specific pinout.
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

#ifndef CH32SNES_PINOUT_H_
#define CH32SNES_PINOUT_H_

#define PIN_PMOD_IO1				GPIOv_from_PORT_PIN(GPIO_port_C, 4)
#define PIN_PMOD_IO2				GPIOv_from_PORT_PIN(GPIO_port_C, 6)
#define PIN_PMOD_IO3				GPIOv_from_PORT_PIN(GPIO_port_D, 2)
#define PIN_PMOD_IO4				GPIOv_from_PORT_PIN(GPIO_port_D, 4)
#define PIN_PMOD_IO5				GPIOv_from_PORT_PIN(GPIO_port_D, 0)
#define PIN_PMOD_IO6				GPIOv_from_PORT_PIN(GPIO_port_C, 5)
#define PIN_PMOD_IO7				GPIOv_from_PORT_PIN(GPIO_port_C, 7)
#define PIN_PMOD_IO8				GPIOv_from_PORT_PIN(GPIO_port_D, 3)


#define PIN_UART_TX					GPIOv_from_PORT_PIN(GPIO_port_D, 5)
#define PIN_UART_RX					GPIOv_from_PORT_PIN(GPIO_port_D, 6)
#define PIN_LED						PIN_UART_TX


#define PIN_CONTROLLERS_LATCH		GPIOv_from_PORT_PIN(GPIO_port_C, 3)
#define PIN_CONTROLLERS_CLOCK		GPIOv_from_PORT_PIN(GPIO_port_C, 2)
#define PIN_CONTROLLERS_DATA1		GPIOv_from_PORT_PIN(GPIO_port_C, 0)
#define PIN_CONTROLLERS_DATA2 		GPIOv_from_PORT_PIN(GPIO_port_C, 1)

#endif /* CH32SNES_PINOUT_H_ */
