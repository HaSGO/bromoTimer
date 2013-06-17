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

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */

/******************************************************************************/


inline void SelfTest() {
    ENC_LR = 1;
    __delay_ms(200);
    ENC_LR = 0;
    ENC_LG = 1;
    __delay_ms(200);
    ENC_LG = 0;
    EXT_LED = 1;
    __delay_ms(200);
    EXT_LED = 0;
    LAMP_1 = 1;
    __delay_ms(200);
    LAMP_1 = 0;
    LAMP_2 = 1;
    __delay_ms(200);
    LAMP_2 = 0;
    PlayBuzzerMs(200);
}

uint8_t main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize I/O and Peripherals for application */
    InitApp();

    if (ENC_SW == 0) {
	printf("\x2Self test...");
	__delay_ms(200);
        SelfTest();
        printf("\x2...end");
	__delay_ms(200);
	while (ENC_SW == 0) NOP();
    }
    printf("\x2HaSGO Bromograph");

    static const int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
    volatile uint16_t enc_val = 0;
    static uint8_t old_AB = 0;

    while (1) {
        switch (status) {
            case mode_MENU:
                printf("\x02Menu");
                status = mode_NOP;
                break;
            case mode_VALUE:
                old_AB <<= 2; //remember previous state
                old_AB |= (PORTA & 0x03); //add current state
                if (enc_states[(old_AB & 0x0f)]) {
                    enc_val += enc_states[(old_AB & 0x0f)];
                    lcd_command(LCD_COMMAND_CLEAR);
                    printf("\x02Value: %d", enc_val);
                }
                break;
            case mode_NOP:
                NOP();
                break;
        }
    }
    return 0;
}
