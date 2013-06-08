/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>            /* HiTech General Includes */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "system.h"

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

void interrupt isr(void)
{
    if (RABIF){
        RABIF = 0;
        __delay_ms(20);
        if(ENC_SW == 0){
            if (status == mode_MENU) {
		status = mode_VALUE;
            } else {
		status = mode_MENU;
            }
        }
    }
}
