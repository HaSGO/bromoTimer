/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>           /* HiTech General Includes */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"
#include "lcd.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void) {
    OSCCON = 0b01000001; /* Oscillatore interno, 1MHz clock*/
}

void PlayBuzzerMs(uint16_t ms) {
    for (uint8_t i = 0; i < 8; ++i) {
        uint16_t tmp = ms;
        while (--tmp) {
            __delay_us(125);
            BUZZER = ~BUZZER;
        }
    }
}

void putch(const char data) {
    static uint8_t counter = 0;
    if (data == '\r') {
        counter = 0;
        return;
    } else if (data == '\n') {
        counter = 8;
        return;
    }
    if (counter == 0)
        lcd_clear();
    else if (counter == 8) {
        lcd_gotoxy(8, 1);
    }
    counter++;
    lcd_putc(data);
}

void start_countdown(uint16_t seconds) {
    status = mode_COUNTDOWN;
    printf("\rRemaining %us", seconds);
    TMR1ON = 1;
    LAMP_1 = 1;
    LAMP_2 = 1;
    EXT_LED = 1;
}