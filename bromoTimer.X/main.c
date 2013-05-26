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

    printf("\x2HaSGO Bromograph");
    if (ENC_SW == 0) {
        SelfTest();
    }

    while(1)
    {
        lcd_command(LCD_COMMAND_CLEAR);
        printf("\x2%u", enc_val);
        __delay_ms(50);
    }
    return 0;
}
