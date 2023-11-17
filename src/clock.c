/*!****************************************************************************
* @file    clock.c
* @author  4eef
* @version V1.0
* @date    13.10.2020
* @brief   Clock control routines
*/

/*!****************************************************************************
* Include
*/
#include "clock.h"

/*!****************************************************************************
* @brief    System clock settings initializer
*/
eDrvError clock_init(CLKCTRL_CLKSEL_t clkSrc, CLKCTRL_PDIV_t pDiv, bool prscEn, bool runStby20M, bool runStby32K){
    eDrvError exitStatus = drvUnknownError;
    uint8_t temp;
    
    temp = 0;
    temp &= ~CLKCTRL_CLKOUT_bm;                                                 //Disable CLKOUT
    temp &= ~CLKCTRL_CLKSEL_gm;
    temp |= clkSrc;
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, temp);
    
    temp = 0;
    temp &= ~CLKCTRL_PDIV_gm;
    temp |= pDiv;
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, temp);
    
    temp = 0;
    if(prscEn){
        temp |= CLKCTRL_PEN_bm;
    }else{
        temp &= ~CLKCTRL_PEN_bm;
    }
    _PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, temp);
    
    temp = 0;
    if(runStby20M){
        temp |= 1 << CLKCTRL_RUNSTDBY_bp;
    }else{
        temp &= ~CLKCTRL_RUNSTDBY_bm;
    }
    _PROTECTED_WRITE(CLKCTRL.OSC20MCTRLA, temp);
    
    temp = 0;
    if(runStby32K){
        temp |= 1 << CLKCTRL_RUNSTDBY_bp;
    }else{
        temp &= ~CLKCTRL_RUNSTDBY_bm;
    }
    _PROTECTED_WRITE(CLKCTRL.OSC32KCTRLA, temp);
    
    exitStatus = drvNoError;
    return exitStatus;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
