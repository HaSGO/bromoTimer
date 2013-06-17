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
    if (RABIF){ /* PORTA/B interrupt */
        RABIF = 0;
        __delay_ms(20);
        if(ENC_SW == 0){
            /* Set appropriate system flag */
            system_flags = (system_flags | ENCODER_BUTTON_PRESSED);
        }
    }
    if (TMR1IF) { /* Timer1 interrupt */
	TMR1IF = 0;
	system_flags = (system_flags | TIMER1_OVERFLOW);
    }
}
