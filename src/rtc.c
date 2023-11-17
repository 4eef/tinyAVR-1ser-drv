/*!****************************************************************************
 * @file    rtc.c
 * @author  4eef
 * @version V1.0
 * @date    November 16, 2023
 * @brief   Real-Time Counter driver
 */

/*!****************************************************************************
 * Include
 */
#include "rtc.h"

/*!****************************************************************************
 * @brief    Initialize basic settings for Real-Time Counter
 * @param    clkSel - clock source
 * @param    compare - value to generate CMP flag at INTFLAGS
 * @param    period - overflow value
 * @param    pDiv - prescaler setting
 * @param    runStby - enables running in Standby sleep mode
 */
eDrvError rtc_init(RTC_CLKSEL_t clkSel, uint16_t compare, uint16_t period, RTC_PRESCALER_t pDiv, bool runStby){
    eDrvError exitStatus = drvUnknownError;
    
    //Select clock source
    RTC.CLKSEL &= ~RTC_CLKSEL_gm;
    RTC.CLKSEL |= clkSel;
    //Set compare value
    while((RTC.STATUS & RTC_CMPBUSY_bm) != 0);
    RTC.CMP = compare;
    //Set overflow value
    while((RTC.STATUS & RTC_PERBUSY_bm) != 0);
    RTC.PER = period;
    //Select prescaler
    while((RTC.STATUS & RTC_CTRLABUSY_bm) != 0);
    RTC.CTRLA &= ~RTC_PRESCALER_gm;
    RTC.CTRLA |= pDiv;
    //Set RUNSTDBY
    while((RTC.STATUS & RTC_CTRLABUSY_bm) != 0);
    if(runStby){
        RTC.CTRLA |= 1 << RTC_RUNSTDBY_bp;
    }else{
        RTC.CTRLA &= ~RTC_RUNSTDBY_bm;
    }
    //Clear counter
    while((RTC.STATUS & RTC_CNTBUSY_bm) != 0);
    RTC.CNT = 0;
    //Kick it off!
    while((RTC.STATUS & RTC_CTRLABUSY_bm) != 0);
    RTC.CTRLA |= 1 << RTC_RTCEN_bp;
    //And clear flags
    if((RTC.INTFLAGS & RTC_OVF_bm) != 0) RTC.INTFLAGS |= 1 << RTC_OVF_bp;
    if((RTC.INTFLAGS & RTC_CMP_bm) != 0) RTC.INTFLAGS |= 1 << RTC_CMP_bp;
    
    exitStatus = drvNoError;
    return exitStatus;
}

/*!****************************************************************************
 * @brief    Wait until Real-Time Counter overflows
 * @param    pCntElapsed - pointer where time elapsed since last overflow will be stored
 * @param    pIsCycBroken - set if there was overflow event beyond this function
 */
eDrvError rtc_waitOvf(uint16_t *pCntElapsed, bool *pIsCycBroken){
    eDrvError exitStatus = drvUnknownError;
    
    //Check inputs
    if((pCntElapsed == NULL) || (pIsCycBroken == NULL)){
        return drvBadParameter;
    }
    //Check timer
    if((RTC.INTFLAGS & RTC_OVF_bm) == 0){
        while((RTC.STATUS & RTC_CNTBUSY_bm) != 0);
        *pCntElapsed = RTC.CNT;
        *pIsCycBroken = false;
        while((RTC.INTFLAGS & RTC_OVF_bm) == 0);
        RTC.INTFLAGS |= 1 << RTC_OVF_bp;
    }else{
        *pCntElapsed = 0;
        *pIsCycBroken = true;
        RTC.INTFLAGS |= 1 << RTC_OVF_bp;
    }
    
    exitStatus = drvNoError;
    return exitStatus;
}

/*!****************************************************************************
 * @brief    
 * @param    
 */
eDrvError rtc_waitCmp(void){
    eDrvError exitStatus = drvUnknownError;
    return exitStatus;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
