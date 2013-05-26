#ifndef SYSTEM_H
#define SYSTEM_H
/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* Encoder */
/* Segnali quadratura encoder */
#define ENC_A RA0
#define ENC_B RA1
/* Pulsante encoder */
#define ENC_SW RA5
/* LED Rosso */
#define ENC_LR RC6
/* LED Verde */
#define ENC_LG RC7

/* Comandi frontalino */
/* Pulsante di accensione esterno */
#define EXT_SW RA2
/* Led esterno */
#define EXT_LED RA4

/* Sensore stato coperchio */
#define EXT_LID RA3

/* Comando buzzer */
#define BUZZER RC5

/* Comandi display LCD */
/* Display enable */
#define DISPLAY_EN RC0
/* Display register select */
#define DISPLAY_RS RC1
/* Display Read/Write */
#define DISPLAY_RW RC2
#define DISPLAY_B0 RB4
#define DISPLAY_B1 RB5
#define DISPLAY_B2 RB6
#define DISPLAY_B3 RB7

/* Controllo rele lampade */
#define LAMP_1 RC4
#define LAMP_2 RC3

/* Microcontroller MIPs (FCY) */
#define _XTAL_FREQ      1e6L
#define FCY             _XTAL_FREQ/4

/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */

void PlayBuzzerMs(uint16_t duration);

void putch(char data);

#endif
