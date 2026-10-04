/*
 * helperfunctions.h
 *
 *  Created on: Jul 2, 2026
 *      Author: timjv
 */

#ifndef INCLUDE_HELPERFUNCTIONS_H_
#define INCLUDE_HELPERFUNCTIONS_H_

#include <stdint.h>
#include <stdio.h>

// Set a specific bit to 1
static inline void BitSet(uint32_t *reg, uint32_t bit) {
    *reg |= (1U << bit);
}

// Clear a specific bit to 0
static inline void BitClear(uint32_t *reg, uint32_t bit) {
    *reg &= ~(1U << bit);
}

// Check if a specific bit is set (returns non-zero if true)
static inline uint32_t BitCheck(uint32_t reg, uint32_t bit) {
    return (reg & (1U << bit));
}


static inline void ConfigBit(uint32_t *reg, uint32_t bit, uint8_t val)
{
	if (reg == NULL || val > 1)
	{
		return;
	}

	if (val)
	{
		BitSet(reg, bit);
	}
	else
	{
		BitClear(reg, bit);
	}
}

//static inline int SetNibble32(uint32_t* word, uint8_t highBit, uint8_t reservedMask, uint8_t nibble)
//{
//	if (word == nullptr || highBit > 31)
//	{
//		return -1;
//	}
//
//	uint32_t maskedNibble = (reservedMask | nibble) & 0x0F;
//
//	uint8_t bitShift = (((highBit / 4) + 1) * 4) - 1;
//
//
//
//
//
//	return 0;
//}


#endif /* INCLUDE_HELPERFUNCTIONS_H_ */
