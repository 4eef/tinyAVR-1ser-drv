/*!****************************************************************************
 * @file    rtc.h
 * @author  4eef
 * @version V1.0
 * @date    November 16, 2023
 * @brief   Real-Time Counter driver
 */

#ifndef rtc_H
#define	rtc_H

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
eDrvError rtc_init(RTC_CLKSEL_t clkSel, uint16_t compare, uint16_t period, RTC_PRESCALER_t pDiv, bool runStby);
eDrvError rtc_waitOvf(uint16_t *pCntElapsed, bool *pIsCycBroken);
eDrvError rtc_waitCmp(void);

#endif //rtc_H
/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
