/*
 * Copyright (c) 2019 Molchip Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _DT_BINDINGS_PINCTRL_MCXCXX_H
#define _DT_BINDINGS_PINCTRL_MCXCXX_H

#define MC_PIN_NO(x) ((x) << 8)
#define MC_GET_PIN_NO(x) ((x) >> 8)
#define MC_GET_PIN_FUNC(x) ((x) & 0xf)

#define MC_PUPD_SET_R1R0_00 100
#define MC_PUPD_SET_R1R0_01 101
#define MC_PUPD_SET_R1R0_10 102
#define MC_PUPD_SET_R1R0_11 103
#define MC_PUPD_SET_R2_0 104
#define MC_PUPD_SET_R2_1 105

#define MC_DRIVE_1mA  1
#define MC_DRIVE_2mA  2
#define MC_DRIVE_3mA  3
#define MC_DRIVE_4mA  4
#define MC_DRIVE_5mA  5
#define MC_DRIVE_6mA  6
#define MC_DRIVE_7mA  7
#define MC_DRIVE_8mA  8
#define MC_DRIVE_10mA 10
#define MC_DRIVE_12mA 12
#define MC_DRIVE_14mA 14
#define MC_DRIVE_16mA 16
#define MC_DRIVE_20mA 20
#define MC_DRIVE_24mA 24
#define MC_DRIVE_28mA 28
#define MC_DRIVE_32mA 32

//spec for SD1
#define MC_DRIVE_1_5mA  1
#define MC_DRIVE_3_0mA  2
#define MC_DRIVE_4_5mA  3
#define MC_DRIVE_6_0mA  4
#define MC_DRIVE_7_5mA  5
#define MC_DRIVE_9_0mA  6
#define MC_DRIVE_10_5mA  7
#define MC_DRIVE_12_0mA  8

#endif /* _DT_BINDINGS_PINCTRL_MCXCXX_H */
