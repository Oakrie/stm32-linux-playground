/*
 * systemclock.h
 *
 *  Created on: Jul 2, 2026
 *      Author: timjv
 * 
 * This was a successful attempt at writing a driver to control the RCC for my basic needs
 * 
 * This driver should not be used as I am moving to using the LL drivers provided by STM32
 */

#ifndef INCLUDE_SYSTEMCLOCK_H_
#define INCLUDE_SYSTEMCLOCK_H_


// Want to get to 180 mhz
// only have HSI, no HSE. Will need to set PLL to source from HSI and there are three ouputs from the PLL
// 1. The main clock (up to 180 mhz)
// 2. The usb clock
// 3. clock for i2s 1 and 2, SPDIF-Rx or the System clock again
// PLLi2S and SAI are used for high quality audio output'




/// Note:

#include "helperfunctions.h"


// RCC General Defines

#define RCC_BASE_REG 0x40023800

/// RCC_CR Register Defines

#define RCC_CR_OFFSET 0x0

#define RCC_CR_ADDR (RCC_BASE_REG + RCC_CR_OFFSET)


#define RCC_CR_PLLSAIRDY_BIT  29  //Default: Off
#define RCC_CR_PLLSAION_BIT   28  //Default: Off
#define RCC_CR_PLLI2SRDY_BIT  27  //Default: Off
#define RCC_CR_PLLI2SON_BIT   26  //Default: Off
#define RCC_CR_PLLRDY_BIT     25  //Default: Off
#define RCC_CR_PLLON_BIT      24  //Default: Off

#define RCC_CR_CSSON_BIT      19  //Default: Off
#define RCC_CR_HSEBYP_BIT     18  //Default: Off
#define RCC_CR_HSERDY_BIT     17  //Default: Off
#define RCC_CR_HSEON_BIT      16  //Default: Off

//Bits 15:8 HSICAL[7:0]: Internal high-speed clock calibration // Default is undefined
//Bits 7:3 HSITRIM[4:0]: Internal high-speed clock trimming    // Default is 10000

#define RCC_CR_HSIRDY_BIT     1  // Default on
#define RCC_CR_HSION_BIT      0  // Default on


// RCC_PLLCFGR

#define RCC_PLLCFGR_OFFSET 0x4

#define RCC_PLLCFGR_ADDR (RCC_BASE_REG + RCC_PLLCFGR_OFFSET)
// VCO Clock Freq = PLL input Clock * PLLN/PLLM
// PLL general clock output = VCO  / PLLP
// USB OTG FS, SDIO = VCO / PLLQ

// PLLR is three bits, Bits 30:28 in register. PLLR Must be 2 ≤ PLLR ≤ 7
#define RCC_PLLCFGR_PLLR_HIGH  30 // default is set to 2 0b010
#define PLLR_MIN 2
#define PLLR_MAX 7

static inline int SetPLLR(uint32_t value)
{
	if (value < PLLR_MIN || value > PLLR_MAX )
	{
		return -1;
	}

	uint32_t bit0 = value & 0b0001;
	uint32_t bit1 = value & 0b0010;
	uint32_t bit2 = value & 0b0100;

	uint32_t* pPLLCFGR = (uint32_t*)RCC_PLLCFGR_ADDR;

	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLR_HIGH - 2, bit0);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLR_HIGH - 1, bit1);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLR_HIGH, bit2);
	return 0;
}

// PLLQ is 4 bits, Bits 27:24 in register. PLLQ Must be 2 ≤ PLLR ≤ 15
#define RCC_PLLCFGR_PLLQ_HIGH  27 // default is set to 4 0b0100
#define PLLQ_MIN 2
#define PLLQ_MAX 15

static inline int SetPLLQ(uint32_t value)
{
	if (value < PLLQ_MIN || value > PLLQ_MAX )
	{
		return -1;
	}

	uint32_t bit0 = value & 0b0001;
	uint32_t bit1 = value & 0b0010;
	uint32_t bit2 = value & 0b0100;
	uint32_t bit3 = value & 0b1000;

	uint32_t* pPLLCFGR = (uint32_t*)RCC_PLLCFGR_ADDR;

	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLQ_HIGH - 3, bit0);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLQ_HIGH - 2, bit1);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLQ_HIGH - 1, bit2);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLQ_HIGH, bit3);
	return 0;
}

#define RCC_PLLCFGR_PLLSRC_BIT 22 // default is set to 0 -> HSI

// PLLP is 2 bits, Bits 17:16 in register PLLP = (bits + 1 )* 2 e.g. bits set to 0b11 = 3, PLLP = (3+1)*2 = 81
#define RCC_PLLCFGR_PLLP_HIGH  17 // default is set to 0 (2) -> 0b00

static inline int SetPLLP(uint32_t value)
{
	uint32_t bit0 = value & 0b0001;
	uint32_t bit1 = value & 0b0010;

	uint32_t* pPLLCFGR = (uint32_t*)RCC_PLLCFGR_ADDR;

	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLP_HIGH - 1, bit0);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLP_HIGH, bit1);
	return 0;
}

// PLLN is 9 bits, Bits 14:6, PLLN must be 50 ≤ PLLR ≤ 432
#define RCC_PLLCFGR_PLLN_HIGH 14 // default is set to 0b0110 0000 0 == 192
#define PLLN_MIN 50
#define PLLN_MAX 432

static inline int SetPLLN(uint32_t value)
{
	if (value < PLLN_MIN || value > PLLN_MAX )
	{
		return -1;
	}

	uint32_t bit0 = value & 0b000000001;
	uint32_t bit1 = value & 0b000000010;
	uint32_t bit2 = value & 0b000000100;
	uint32_t bit3 = value & 0b000001000;
	uint32_t bit4 = value & 0b000010000;
	uint32_t bit5 = value & 0b000100000;
	uint32_t bit6 = value & 0b001000000;
	uint32_t bit7 = value & 0b010000000;
	uint32_t bit8 = value & 0b100000000;

	uint32_t* pPLLCFGR = (uint32_t*)RCC_PLLCFGR_ADDR;

	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLN_HIGH - 8, bit0);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLN_HIGH - 7, bit1);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLN_HIGH - 6, bit2);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLN_HIGH - 5, bit3);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLN_HIGH - 4, bit4);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLN_HIGH - 3, bit5);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLN_HIGH - 2, bit6);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLN_HIGH - 1, bit7);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLN_HIGH, bit8);
	return 0;
}

// PLLM is 6 bits, Bits 5:0, PLLM must be 2 ≤ PLLR ≤ 63
#define RCC_PLLCFGR_PLLM_HIGH 5 // default is set to 0b01 0000 = 16
#define PLLM_MIN 2
#define PLLM_MAX 63

static inline int SetPLLM(uint32_t value)
{
	if (value < PLLM_MIN || value > PLLM_MAX )
	{
		return -1;
	}

	uint32_t bit0 = value & 0b000000001;
	uint32_t bit1 = value & 0b000000010;
	uint32_t bit2 = value & 0b000000100;
	uint32_t bit3 = value & 0b000001000;
	uint32_t bit4 = value & 0b000010000;
	uint32_t bit5 = value & 0b000100000;

	uint32_t* pPLLCFGR = (uint32_t*)RCC_PLLCFGR_ADDR;

	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLM_HIGH - 5, bit0);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLM_HIGH - 4, bit1);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLM_HIGH - 3, bit2);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLM_HIGH - 2, bit3);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLM_HIGH - 1, bit4);
	ConfigBit(pPLLCFGR, RCC_PLLCFGR_PLLM_HIGH, bit5);
	return 0;
}


#define RCC_CFGR_OFFSET 0x08
#define RCC_CFGR_ADDR (RCC_BASE_REG + RCC_CFGR_OFFSET)

// SWS is 2 bits, Bits 3:2. It is the current system clock selected
#define RCC_CFGR_SWS_HIGH 3

// SW is 2 bits, Bits 1:0. 0b00: HSI, 0b01: HSE 0b10 PLL_P, 0b11: PLL_R
#define RCC_CFGR_SW_HIGH 1

static inline int SetSysClock(uint32_t value)
{

	uint32_t bit0 = value & 0b01;
	uint32_t bit1 = value & 0b10;

	uint32_t* pRCCCFGR = (uint32_t*)RCC_CFGR_ADDR;

	ConfigBit(pRCCCFGR, RCC_CFGR_SW_HIGH - 1, bit0);
	ConfigBit(pRCCCFGR, RCC_CFGR_SW_HIGH, bit1);
	return 0;
}

#define RCC_AHB1ENR_OFFSET 0x30

#define RCC_AHB1ENR_ADDR (RCC_BASE_REG + RCC_AHB1ENR_OFFSET)

#define RCC_AHB1ENR_GPIOA_BIT 0
#define RCC_AHB1ENR_GPIOC_BIT 2



#endif /* INCLUDE_SYSTEMCLOCK_H_ */
