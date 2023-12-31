/*!****************************************************************************
* @file    clock.h
* @author  4eef
* @version V1.0
* @date    13.10.2020
* @brief   Clock control routines
*/
#ifndef clock_H
#define clock_H

/*!****************************************************************************
* Include
*/
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "drv_errors.h"

/*!****************************************************************************
* Prototypes for the functions
*/
eDrvError clock_init(CLKCTRL_CLKSEL_t clkSrc, CLKCTRL_PDIV_t pDiv, bool prscEn, bool runStby20M, bool runStby32K);

#endif //clock_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
