/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

/* Encoder */
#define ENC_A RA0 /* Segnali quadratura encoder */
#define ENC_B RA1
#define ENC_SW RA5 /* Pulsante encoder */
#define ENC_LR RC6 /* LED Rosso */
#define ENC_LG RC7 /* LED Verde */

/* Comandi frontalino */
#define EXT_SW RA2 /* Pulsante di accensione esterno */
#define EXT_LED RA4 /* Led esterno */

/* Sensore stato coperchio */
#define EXT_LID RA3

/* Comando buzzer */
#define BUZZER RC5

/* Comandi display LCD */
#define DISPLAY_EN RC0 /* Display enable */
#define DISPLAY_RS RC1 /* Display register select */
#define DISPLAY_RW RC2 /* Display Read/Write */
#define DISPLAY_B0 RB4
#define DISPLAY_B1 RB5
#define DISPLAY_B2 RB6
#define DISPLAY_B3 RB7

/* Controllo relè lampade */
#define LAMP_1 RC4
#define LAMP_2 RC3


/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

/* TODO User level functions prototypes (i.e. InitApp) go here */

void InitApp(void);         /* I/O and Peripheral Initialization */
