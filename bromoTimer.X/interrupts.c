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

static const int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
volatile uint16_t enc_val = 0;

void interrupt isr(void)
{
    static uint8_t old_AB = 0;
    ENC_LR = 1;
    di();
    if (RABIF){
        old_AB <<= 2; //remember previous state
        old_AB |= (PORTA & 0x03); //add current state
        enc_val += enc_states[(old_AB & 0x0f)];
        RABIF = 0;
    }
    ei();
}
