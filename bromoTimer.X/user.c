/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* HiTech General Includes */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"
#include "system.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    /* Only output is RA4 */
    /* Unused I/O:
     *        xx       */
    TRISA = 0b00101111;
    WPUA  = 0b00100111;

    /* All RB* are outputs used for the display */
    /* Unused I/O:
     *            xxxx */
    TRISB = 0b00000000;
    WPUB  = 0b00000000; /* Disable pull-ups on PORTB */

    /* All the RC* are outputs */
    TRISC = 0b00000000;

    /* enable weak pull-ups globally */
    OPTION_REG &= 0b01111111;

    /* Initialize peripherals */
    BUZZER = 0;
    LAMP_1 = 0;
    LAMP_2 = 0;

    /* Enable interrupts */
}

