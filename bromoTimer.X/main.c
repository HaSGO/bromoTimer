/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>           /* Global Header File */
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include <conio.h>
#include <stdio.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */
#include "lcd.h"

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* Current encoder value set by user */
uint16_t seconds = 1;

/******************************************************************************/
/* Main Program                                                               */

/******************************************************************************/

/* This function reads system flags and acts upon them. */
void inline ProcessFlags() {
    if( system_flags & ENCODER_BUTTON_PRESSED ) {
	system_flags &= ~ENCODER_BUTTON_PRESSED; /* Reset encoder flag */
	if (status == mode_NOP) {
	    printf("\x02Turn knob to set");
	    status = mode_SET;
	} else if (status == mode_SET){
	    //Start countdown
	    status = mode_COUNTDOWN;
	    printf("\x02Remaining %us", seconds);
	    TMR1ON = 1;
	    LAMP_1 = 1;
	    LAMP_2 = 1;
            EXT_LED = 1;
	}

    }
    if (system_flags & TIMER1_OVERFLOW) {
	system_flags &= ~TIMER1_OVERFLOW;
	/* Reset timer value */
	TMR1H = 0b10000101;
	TMR1L = 0b11101110;
	/* decrement second counter */
	if (!--seconds){ /* If we are decrementing to 0, we are done */
	    LAMP_1 = 0;
	    LAMP_2 = 0;
	    TMR1ON = 0;
            EXT_LED = 0;
	    status = mode_NOP;

	    printf("\x02Done!");
	    PlayBuzzerMs(100);
	    __delay_ms(100);
	    PlayBuzzerMs(100);
	} else {
	    lcd_gotoxy(2,3);	/* Go to a space after the word "remaining" */
	    printf("%us", seconds);
	}
    }
}

uint8_t main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    if (ENC_SW == 0) {
	printf("\x02Self test...");
	__delay_ms(200);
        SelfTest();
        printf("\x2...end");
	__delay_ms(200);
	while (ENC_SW == 0) NOP();
    }
    printf("\x02HaSGO Bromograph");

    static const int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
    static uint8_t old_AB = 0;

    while (1) {
	ProcessFlags();

        switch (status) {
            case mode_SET:
                old_AB <<= 2; //remember previous state
                old_AB |= (PORTA & 0x03); //add current state
                if (enc_states[(old_AB & 0x0f)]) {
                    seconds += enc_states[(old_AB & 0x0f)];
                    lcd_command(LCD_COMMAND_CLEAR);
                    printf("\x02Timer: %us", seconds);
                }
                break;
            case mode_NOP:
                NOP();
                break;
        }
    }
    return 0;
}
