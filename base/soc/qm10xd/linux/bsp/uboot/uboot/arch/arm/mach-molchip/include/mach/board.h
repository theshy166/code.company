/*
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _MOLCHIP_BOARD_H_
#define _MOLCHIP_BOARD_H_


u8 nvm_boot_device(void);

void jump_to(uint addr);

void ana_bias_reset(void);

void nosec_jump_to(uint addr);

void nosec_boot(uint address);

#endif

