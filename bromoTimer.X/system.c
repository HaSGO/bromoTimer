/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>           /* HiTech General Includes */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    OSCCON=0b01000001; /* Oscillatore interno, 1MHz clock*/
}

void PlayBuzzerMs(uint16_t ms) {
    for ( uint8_t i = 0; i < 8; i++) {
        uint16_t tmp = ms;
        while (--tmp) {
            __delay_us(125);
            BUZZER=~BUZZER;
        }
    }
}
