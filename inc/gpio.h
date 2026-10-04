/*
 * gpio.h
 *
 *  Created on: Jul 3, 2026
 *      Author: timjv
 */

#ifndef INCLUDE_GPIO_H_
#define INCLUDE_GPIO_H_

#define GPIO_A_BASE 0x40020000
#define GPIO_C_BASE 0x40020800


// 0b00 input, 0b01: output, 0b10 Alternate, 0b11: Analog
#define GPIO_MODE_R_OFFSET   0x00

// only 16 bits, 31:16 must not be changed, 15:0 each set output type
// 0b0: Push-Pull, 0b1: Open-Drain
#define GPIO_OTYPE_R_OFFEST  0x04

// 0b00 Slow, 0b01: Medium, 0b10 Fast, 0b11: High
// Refer to Vdd and external load for speed config chart in manual
#define GPIO_OSPEED_R_OFFSET 0x08

// 0b00: No PU or PD, 0b01: Pull-up, 0b10: Pull-down, 0b11: reserved
#define GPIO_PUPD_R_OFFSET   0x0C

// Input Data Register, 31:16 reserved, 15:0 are port input data bits
#define GPIO_ID_R_OFFSET     0x10

// Output Data Register, 31:16 reserved, 15:0 are port input data bits
#define GPIO_OD_R_OFFSET     0x14
#define GPIO_BSR_R_OFFSET    0x18
#define GPIO_LCK_R_OFFSET    0x1C
#define GPIO_AF_RL_OFFSET    0x20
#define GPIO_AF_RH_OFFSET    0x24

//


#endif /* INCLUDE_GPIO_H_ */
