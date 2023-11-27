/*!****************************************************************************
* @file    adc.c
* @author  4eef
* @version V1.0
* @date    28.11.2020
* @brief   ADC processing routines driver for ATtiny3217
*/

/*!****************************************************************************
* Include
*/
#include "adc.h"

/*!****************************************************************************
* Local function prototypes
*/
eDrvError adcDelay(uint16_t time);

/*!****************************************************************************
* @brief    Initialize ADC module
*/
eDrvError adc_init(ADC_t *p, ADC_RESSEL_t resolution, ADC_DUTYCYC_t dutyCyc, ADC_ASDV_t autoSmpDelay, uint8_t startEvent){
    eDrvError exitStatus = drvUnknownError;
    
    //Perform check
    if(p == NULL){
        return drvBadParameter;
    }
    //Initialize main ADC parameters
    p->CTRLA    &= ~ADC_RESSEL_bm;
    p->CTRLA    |= resolution;
    p->CALIB    &= ~ADC_DUTYCYC_bm;
    p->CALIB    |= dutyCyc;
    p->CTRLD    &= ~ADC_ASDV_bm;
    p->CTRLD    |= autoSmpDelay;
    p->EVCTRL   &= ADC_STARTEI_bm;
    p->EVCTRL   |= startEvent;
    p->DBGCTRL  &= ADC_DBGRUN_bm;
    p->DBGCTRL  |= ADC_DBG_RUN << ADC_DBGRUN_bp;
    //Initialize delay timer
    timer_initTimB(&TCB1, TCB_CNTMODE_SINGLE_gc, TCB_CLKSEL_CLKDIV2_gc);
    
    exitStatus = drvNoError;
    return exitStatus;
}

/*!****************************************************************************
* @brief    Getting a sample from single channel routine. Works in blocking mode
*/
eDrvError adc_getSample(adcChannel_type adcChannel, uint16_t *pData, uint8_t *pOverSampled, uint16_t *pRefVolt){
    eDrvError exitStatus = drvUnknownError, drvExStatus;
    uint8_t i;
    
    //Perform checks
    if((pData == NULL) || (pOverSampled == NULL) || (pRefVolt == NULL)){
        return drvBadParameter;
    }
    
    //VREF
    if(adcChannel.refSel == ADC_REFSEL_INTREF_gc){
        if((adcChannel.p == &ADC0) && (VREF.CTRLA != adcChannel.intRefSrc)){
            VREF.CTRLB &= ~VREF_ADC0REFEN_bm;
            VREF.CTRLA &= ~VREF_ADC0REFSEL_gm;
            VREF.CTRLA |= adcChannel.intRefSrc;
            VREF.CTRLB |= 1 << VREF_ADC0REFEN_bp;
        }else if((adcChannel.p == &ADC1) && (VREF.CTRLC != adcChannel.intRefSrc)){
            VREF.CTRLD &= ~VREF_ADC1REFEN_bm;
            VREF.CTRLC &= ~VREF_ADC1REFSEL_gm;
            VREF.CTRLC |= adcChannel.intRefSrc;
            VREF.CTRLD |= 1 << VREF_ADC1REFEN_bp;
        }
        //Perform delay to stabilize ADC
        drvExStatus = adcDelay(ADC_STABILIZE_TIME);
        if(drvExStatus != drvNoError) return drvHwError;
        //Return reference voltage
        switch(adcChannel.intRefSrc){
            case VREF_ADC_REFSEL_0V55_gc:
                *pRefVolt = ADC_VREF_0V55;
                break;
            case VREF_ADC_REFSEL_1V1_gc:
                *pRefVolt = ADC_VREF_1V1;
                break;
            case VREF_ADC_REFSEL_2V5_gc:
                *pRefVolt = ADC_VREF_2V5;
                break;
            case VREF_ADC_REFSEL_4V34_gc:
                *pRefVolt = ADC_VREF_4V34;
                break;
            case VREF_ADC_REFSEL_1V5_gc:
                *pRefVolt = ADC_VREF_1V5;
                break;
            default:
                *pRefVolt = ADC_ERR;
                break;
        }
    }else if(adcChannel.refSel == ADC_REFSEL_VDDREF_gc){
        *pRefVolt = ADC_VREF_VDD;
    }else{
        //Not implemented yet!
        *pRefVolt = ADC_ERR;
    }
    //Return oversampling size
    switch(adcChannel.smpAccNum){
        case ADC_SAMPNUM_ACC1_gc:
            *pOverSampled = ADC_OVERSAMPLING_0;
            break;
        case ADC_SAMPNUM_ACC2_gc:
            *pOverSampled = ADC_OVERSAMPLING_1;
            break;
        case ADC_SAMPNUM_ACC4_gc:
            *pOverSampled = ADC_OVERSAMPLING_2;
            break;
        case ADC_SAMPNUM_ACC8_gc:
            *pOverSampled = ADC_OVERSAMPLING_3;
            break;
        case ADC_SAMPNUM_ACC16_gc:
            *pOverSampled = ADC_OVERSAMPLING_4;
            break;
        case ADC_SAMPNUM_ACC32_gc:
            *pOverSampled = ADC_OVERSAMPLING_5;
            break;
        case ADC_SAMPNUM_ACC64_gc:
            *pOverSampled = ADC_OVERSAMPLING_6;
            break;
        default:
            *pOverSampled = ADC_ERR;
            break;
    }
    //Initialize rest of ADC
    adcChannel.p->CTRLA &= ~ADC_ENABLE_bm;
    adcChannel.p->CTRLD &= ~ADC_INITDLY_gm;
    adcChannel.p->CTRLD |= adcChannel.initDelay;
    adcChannel.p->MUXPOS &= ~ADC_MUXPOS_gm;
    adcChannel.p->MUXPOS |= adcChannel.numCh;
    adcChannel.p->SAMPCTRL &= ~ADC_SAMPLEN_gm;
    adcChannel.p->SAMPCTRL |= adcChannel.smpLen << ADC_SAMPLEN_gp;
    adcChannel.p->SAMPCTRL &= ~ADC_SAMPCAP_bm;
    adcChannel.p->SAMPCTRL |= adcChannel.smpCap << ADC_SAMPCAP_bp;
    adcChannel.p->CTRLA &= ~ADC_FREERUN_bm;
    adcChannel.p->CTRLA |= adcChannel.freerun << ADC_FREERUN_bp;
    adcChannel.p->CTRLC &= ~ADC_PRESC_gm;
    adcChannel.p->CTRLC |= adcChannel.prescaler;
    adcChannel.p->CTRLC &= ~ADC_REFSEL_gm;
    adcChannel.p->CTRLC |= adcChannel.refSel;
    adcChannel.p->CTRLB &= ~ADC_SAMPNUM_gm;
    adcChannel.p->CTRLB |= adcChannel.smpAccNum;
    adcChannel.p->CTRLE &= ~ADC_WINCM_gm;
    adcChannel.p->CTRLE |= adcChannel.wndCmp;
    adcChannel.p->CTRLA |= 1 << ADC_ENABLE_bp;
    //Perform measurement
    for(i = 0; i < ADC_MEASUREMENTS_NUM; i++){
        adcChannel.p->COMMAND |= 1 << ADC_STCONV_bp;
        while(!(adcChannel.p->INTFLAGS & ADC_RESRDY_bm));          //TO DO: add a timer
        *pData = adcChannel.p->RES;
    }
    
    exitStatus = drvNoError;
    return exitStatus;
}

/*!****************************************************************************
* @brief    Delaying mechanism in microsecond resolution for ADC peripherals
* @param    time - delay time in 0.1 x microseconds
*/
eDrvError adcDelay(uint16_t time){
    eDrvError exitStatus = drvUnknownError, drvExStatus;
    uint16_t cycLen;
    bool cycBrkn;
    
    //Check input
    if(time > ADC_DELAY_MAX){
        return drvBadParameter;
    }
    drvExStatus = timer_startTimB(&TCB1, time);
    if(drvExStatus != drvNoError) return drvHwError;
    drvExStatus = timer_waitOvfTimB(&TCB1, &cycLen, &cycBrkn);
    if((drvExStatus != drvNoError) || (cycBrkn == true)) return drvHwError;
    
    exitStatus = drvNoError;
    return exitStatus;
}

/***************** (C) COPYRIGHT ************** END OF FILE ******** 4eef ****/
