 /*!****************************************************************************
* @file     gpio.c
* @author   4eef
* @version  V1.0
* @date     05.10.2020
* @brief    GPIO driver for ATtiny1617 / ATtiny3217
*/

/*!****************************************************************************
* Include
*/
#include "gpio.h"

/*!****************************************************************************
 * Local function prototypes
 */
eDrvError gppin_init(PORT_t *port, uint8_t npin, uint8_t pinDir, uint8_t initState, uint8_t pinInvEn, uint8_t pullUpEn, PORT_ISC_t inputSense);

/*!****************************************************************************
* InitAllGpio
*/
eDrvError gpio_init(pinMode_type *pinsMode, uint32_t pinNum){
    eDrvError exitStatus = drvUnknownError;
    pinMode_type *pgpios;
    pinMode_type *pgpiosEnd;
    
    //Perform check
    if(pinsMode == NULL){
        return drvBadParameter;
    }
    pgpios = pinsMode;
    pgpiosEnd = pgpios + pinNum;
    
    while(pgpios < pgpiosEnd){
        gppin_init(pgpios->p, pgpios->npin, pgpios->pinDir, pgpios->initState, pgpios->pinInvEn, pgpios->pullUpEn, pgpios->inputSense);
        pgpios++;
    }
    
    exitStatus = drvNoError;
    return exitStatus;
}

/*!****************************************************************************
*
*/
eDrvError gppin_init(PORT_t *port, uint8_t npin, uint8_t pinDir, uint8_t initState, uint8_t pinInvEn, uint8_t pullUpEn, PORT_ISC_t inputSense){
    eDrvError exitStatus = drvUnknownError;
    //Set pin direction
    if(pinDir != 0){
        port->DIRSET = (1<<npin);
    }else{
        port->DIRCLR = (1<<npin);
    }
    
    //Set pin initial state
    if(initState != 0){
        port->OUTSET = (1<<npin);
    }else{
        port->OUTCLR = (1<<npin);
    }
    
    //Set pin controls
    switch(npin){
        case 0:
            port->PIN0CTRL = (pinInvEn << PORT_INVEN_bp) | (pullUpEn << PORT_PULLUPEN_bp) | (inputSense);
            break;
        case 1:
            port->PIN1CTRL = (pinInvEn << PORT_INVEN_bp) | (pullUpEn << PORT_PULLUPEN_bp) | (inputSense);
            break;
        case 2:
            port->PIN2CTRL = (pinInvEn << PORT_INVEN_bp) | (pullUpEn << PORT_PULLUPEN_bp) | (inputSense);
            break;
        case 3:
            port->PIN3CTRL = (pinInvEn << PORT_INVEN_bp) | (pullUpEn << PORT_PULLUPEN_bp) | (inputSense);
            break;
        case 4:
            port->PIN4CTRL = (pinInvEn << PORT_INVEN_bp) | (pullUpEn << PORT_PULLUPEN_bp) | (inputSense);
            break;
        case 5:
            port->PIN5CTRL = (pinInvEn << PORT_INVEN_bp) | (pullUpEn << PORT_PULLUPEN_bp) | (inputSense);
            break;
        case 6:
            port->PIN6CTRL = (pinInvEn << PORT_INVEN_bp) | (pullUpEn << PORT_PULLUPEN_bp) | (inputSense);
            break;
        case 7:
            port->PIN7CTRL = (pinInvEn << PORT_INVEN_bp) | (pullUpEn << PORT_PULLUPEN_bp) | (inputSense);
            break;
        default:
            break;
    }
    
    exitStatus = drvNoError;
    return exitStatus;
}

/*************** (C) COPYRIGHT ************** END OF FILE ********* 4eef *****/
