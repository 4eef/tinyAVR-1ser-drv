/*!****************************************************************************
* @file     gpio.c
* @author   4eef
* @version  V1.0
* @date     05.10.2020
* @brief    GPIO driver for ATtiny1617 / ATtiny3217
*/

#ifndef gpio_H
#define gpio_H

/*!****************************************************************************
* Include
*/
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include "drv_errors.h"

/*!****************************************************************************
* User define
*/
//DIR
#define PIN_INPUT                           0x00
#define PIN_OUTPUT                          0x01
//INV
#define INV_DIS                             0x00
#define INV_EN                              0x01
//PULLUP
#define PUP_DIS                             0x00
#define PUP_EN                              0x01

/*!****************************************************************************
* User typedef
*/
typedef struct{
    PORT_t          *p;                                                         //Port
    uint8_t         npin;                                                       //Pin number
    uint8_t         pinDir;                                                     //Pin direction
    uint8_t         initState;                                                  //Initial state
    uint8_t         pinInvEn;                                                   //Enable pin invert pin
    uint8_t         pullUpEn;                                                   //Enable pull-up resistor
    PORT_ISC_t      inputSense;                                                 //Input sense control
}pinMode_type;

/*!****************************************************************************
* Macro functions
*/
#define makepin(port, pin, pinDir, initState, pinInvEn, pullUpEn, inputSense)    {&port, pin, pinDir, initState, pinInvEn, pullUpEn, inputSense}
                                    
#define _gppin_set(port, npin)      (port->OUTSET = (1 << npin))
#define _gppin_reset(port, npin)    (port->OUTCLR = (1 << npin))
#define _gppin_togle(port, npin)    (port->OUTTGL = (1 << npin))
#define _gppin_get(port, npin)      ((port->IN & (1 << npin)) >> npin)
#define _gppin_getFlag(port, npin)  ((port->INTFLAGS & (1 << npin)) >> npin)
#define _gppin_clrFlag(port, npin)  (port->INTFLAGS = (1 << npin))
#define _gppin_dirOut(port, npin)   (port->DIRSET = (1<<npin))
#define _gppin_dirIn(port, npin)    (port->DIRCLR = (1<<npin))

/*!****************************************************************************
* Prototypes for the functions
*/
eDrvError gpio_init(pinMode_type *pinsMode, uint32_t pinNum);

#endif //gpio_H
/*************** (C) COPYRIGHT ************** END OF FILE ********* 4eef *****/
