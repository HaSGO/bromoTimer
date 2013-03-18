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
