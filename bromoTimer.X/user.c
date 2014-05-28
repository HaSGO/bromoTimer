/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>            /* HiTech General Includes */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"
#include "system.h"
#include "lcd.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{

    /* Disable analog stuff */
    ANSEL = 0;
    ANSELH = 0;

    /* Only output is RA4 */
    /* Unused I/O:
     *        xx       */
    TRISA = 0b00101111;
    WPUA  = 0b00101111;
    IOCA  = 0b00101000; /* Enable interrupt on change */

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
    EXT_LED = 0;
    ENC_LR = 0;
    ENC_LG = 0;

    /* Timer1 initializations */
    TMR1CS = 0; /* Timer1 source is fosc/4 */
    T1CON |= 0b00110000;
    /* Set initial timer value at 31250 so we get 65536 - 34268 = 31250 counts. */
    /* Fosc is 1 MHz, Fosc/4 is 250kHz. We use a 1:8 prescaler, which gives us
     * 31250 kHz. If we count 34268 impulses at that frequency, we count one second. */
    TMR1H = 0b10000101;
    TMR1L = 0b11101110;

    lcd_init();

    /* Enable interrupts */
    INTCON |= 0b11001000; /* Enable global, PORTA and peripheral interrupts */
    PIE1   |= 0b00000001; /* Enable Timer1 interrupt */
}

inline void SelfTest() {
    ENC_LR = 1;
    __delay_ms(500);
    ENC_LR = 0;
    ENC_LG = 1;
    __delay_ms(500);
    ENC_LG = 0;
    EXT_LED = 1;
    __delay_ms(500);
    EXT_LED = 0;
    LAMP_1 = 1;
    __delay_ms(500);
    LAMP_1 = 0;
    LAMP_2 = 1;
    __delay_ms(500);
    LAMP_2 = 0;
    PlayBuzzerMs(500);
}
