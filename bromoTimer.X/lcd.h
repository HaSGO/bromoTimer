#ifndef LCD_H
#define	LCD_H

#ifndef PIN
#define PIN(n,x,y)	static volatile bit n @ (unsigned)&x*8+y
#endif

// Set exactly one of these screen types to true so that code is generated properly.
// If no type is enabled, you will get errors!
//
//	Format			Status		Notes			All part numbers are Hantronix (www.hantronix.com)
//	===============		=======		=====================	==================================================>>
#define	LCD_MODE_1x8		(false)					// HDM08111H-1
#define	LCD_MODE_1x16_A		(false)		// Row=1, Col=1..16	// HDM16116H-7, HDM16116H-L, HDM16116L*, HDM16116L-7, HDM16116L-L, HDM16116L-1, HDM16116H-2, HDM16116L-2
#define	LCD_MODE_1x16_B		(true)		// Row=1..2, Col=1..8	// HDM16116H, HDM16116L* note that this part number could be either A or B
#define	LCD_MODE_1x40		(false)					// HDM40108H-2
#define	LCD_MODE_2x8		(false)					// HDM08216H-1, HDM08216L, HDM16216H-2
#define	LCD_MODE_2x12		(false)					// HDM12216H, HDM12216L
#define	LCD_MODE_2x16		(false)					// HDM16216H-2, HDM16216H-4, HDM16216H-5, HDM16216H-B, HDM16216H-D, HDM16216L-2, HDM16216L-5, HDM16216L-7, HDM16216L-D, HDM16216L-B, HDM16216H-I
#define	LCD_MODE_2x20		(false)					// HDM20216H-3, HDM20216L, HDM20216L-1, HDM20216L-L, HDM20216H-L
#define	LCD_MODE_2x24		(false)					// HDM24216H-2 HDM24216L-2
#define	LCD_MODE_2x40		(false)					// HDM40216H-4, HDM40216L, HDM40216L-1
#define	LCD_MODE_4x16		(false)					// HDM16416H, HDM16416L
#define	LCD_MODE_4x20		(false)					// HDM40216H-4, HDM40216L, HDM40216L-1
#define LCD_MODE_4x24		(false)		// UNSUPPORTED!		// HDM24416H, HDM24416L
#define	LCD_MODE_4x40		(false)		// UNSUPPORTED!		// HDM40416H, HDM40416L-4, HDM40416H-5

#define	LCD_CURSOR_BLINK	(false)					// Blink/Noblink cursor mode
#define	LCD_CURSOR_ON		(false)					// Cursor visible
#define	LCD_CURSOR_INCREMENT	(true)					// Set left-to-right cursor movement
#define	LCD_CURSOR_SHIFT	(false)					// Shift display on entry

#define	LCD_DISPLAY_5x10	(false)					// Select 5x7 or 5x10 character matrix
#define	LCD_DISPLAY_ON		(true)					// Turn display on/off

#define	LCD_ALLOW_USER_CHARS	(true)					// Controls whether display uses ASCII for control chars or uses user-defined chars in lcd_putc()
#define	LCD_ENABLE_GETC		(true)					// Save code space by setting to false
#define	LCD_ENABLE_GOTOXY	(true)					//   any functions which you will not
#define	LCD_ENABLE_UNSCROLL	(true)
#define	LCD_ENABLE_SCROLL	(true)
#define	LCD_ENABLE_CLEAR	(true)

#if (LCD_ENABLE_SCROLL)
#define	LCD_ENABLE_GOTOXY	(true)					// Over-ride turning it off because these function
#endif
#if (LCD_ENABLE_UNSCROLL)
#define	LCD_ENABLE_GOTOXY	(true)					//   need access to the gotoxy function.
#endif
#if (!LCD_ALLOW_USER_CHARS)
#define	LCD_ENABLE_GOTOXY	(true)					//   need access to the gotoxy function.
#endif

#define	LCD_DATA_PORT		PORTB					// Port on which LCD data lines are connected
#define	LCD_TRIS_PORT		TRISB					// Need to specify the corresponding TRIS

#define	LCD_4_BIT_MODE		(true)					// 4-bit or 8-bit interface selection

#if LCD_4_BIT_MODE
#define	LCD_D4_BIT		4					// Attachment of D0 to data port bus - note
#endif									//   that they MUST be contiguous and in order!

#if LCD_4_BIT_MODE
#define	LCD_TRIS_DATAMASK	((uint8_t)(0b11110000))			// Define the bitmask used to read/write the data bits
#else
#define	LCD_TRIS_DATAMASK	((uint8_t)(0b11111111))			// Define all bits used for 8-bit mode
#endif

#if LCD_ALLOW_USER_CHARS
#else
#define	LCD_DESTRUCTIVE_BS	(true)					// Backspace '\b' erases character
#endif

PIN (LCD_E,		PORTC, 0);					// Pin for LCD /E signal
PIN (LCD_RS,		PORTC, 1);					// Pin for LCD RS signal
PIN (LCD_RW,		PORTC, 2);					// Pin for LCD RW signal
PIN (LCD_TRIS_E,	TRISC, 0);					// Add definitions for direction control
PIN (LCD_TRIS_RS,	TRISC, 1);
PIN (LCD_TRIS_RW,	TRISC, 2);

/////////////////// DO NOT CHANGE ANYTHING BELOW THIS LINE! //////////////

#define	LCD_8_BIT_MODE		(!LCD_4_BIT_MODE)

#if LCD_MODE_1x8
#define LCD_MAXROWS		((uint8_t)(1))				// Number of LCD rows
#define	LCD_MAXCOLS		((uint8_t)(8))				// Number of LCD columns
#define	LCD_MULTI_LINE		(false)					// Operate in 1-line or 2-line mode for controller
#endif
#if LCD_MODE_1x16_A
#define LCD_MAXROWS		((uint8_t)(1))
#define	LCD_MAXCOLS		((uint8_t)(16))
#define	LCD_MULTI_LINE		(false)
#endif
#if LCD_MODE_1x16_B
#define LCD_MAXROWS		((uint8_t)(1))
#define	LCD_MAXCOLS		((uint8_t)(16))
#define	LCD_MULTI_LINE		(true)
#endif
#if LCD_MODE_1x40
#define LCD_MAXROWS		((uint8_t)(1))
#define	LCD_MAXCOLS		((uint8_t)(40))
#define	LCD_MULTI_LINE		(true)					// Not sure about this
#endif
#if LCD_MODE_2x8
#define LCD_MAXROWS		((uint8_t)(2))
#define	LCD_MAXCOLS		((uint8_t)(8))
#define	LCD_MULTI_LINE		(true)
#endif
#if LCD_MODE_2x12
#define LCD_MAXROWS		((uint8_t)(2))
#define	LCD_MAXCOLS		((uint8_t)(12))
#define	LCD_MULTI_LINE		(true)
#endif
#if LCD_MODE_2x16
#define LCD_MAXROWS		((uint8_t)(2))
#define	LCD_MAXCOLS		((uint8_t)(16))
#define	LCD_MULTI_LINE		(true)
#endif
#if LCD_MODE_2x20
#define LCD_MAXROWS		((uint8_t)(2))
#define	LCD_MAXCOLS		((uint8_t)(20))
#define	LCD_MULTI_LINE		(true)
#endif
#if LCD_MODE_2x24
#define LCD_MAXROWS		((uint8_t)(2))
#define	LCD_MAXCOLS		((uint8_t)(24))
#define	LCD_MULTI_LINE		(true)
#endif
#if LCD_MODE_2x40
#define LCD_MAXROWS		((uint8_t)(2))
#define	LCD_MAXCOLS		((uint8_t)(40))
#define	LCD_MULTI_LINE		(true)
#endif
#if LCD_MODE_4x16
#define LCD_MAXROWS		((uint8_t)(4))
#define	LCD_MAXCOLS		((uint8_t)(16))
#define	LCD_MULTI_LINE		(true)
#endif
#if LCD_MODE_4x20
#define LCD_MAXROWS		((uint8_t)(4))
#define	LCD_MAXCOLS		((uint8_t)(20))
#define	LCD_MULTI_LINE		(true)
#endif

#define	LCD_COMMAND_CLEAR	((uint8_t)(0x01))				// Clear screen, home cursor, unshift display
#define	LCD_COMMAND_HOME	((uint8_t)(0x02))				// Home cursor, unshift display
#define	LCD_COMMAND_BACKSPACE	((uint8_t)(0x10))				// Move cursor left one (destructive based on LCD_DESTRUCTIVE_BS)
#define	LCD_COMMAND_FWDSPACE	((uint8_t)(0x14))				// Move cursor right one
#define	LCD_COMMAND_PANLEFT	((uint8_t)(0x18))				// Move screen left one
#define	LCD_COMMAND_PANRIGHT	((uint8_t)(0x1C))				// Move screen right one

#ifndef LCD_MAXROWS
	#error "NO LCD MODE SPECIFIED - You need to give a 1x8, 2x8 etc. format!"
#endif

// Mandatory functions:

void lcd_init (void);							// Initialize LCD bus and Mode
void lcd_putc (uint8_t c);							// Write character to LCD
void lcd_command (uint8_t c);						// Write command to LCD controller
uint8_t lcd_lineof (uint8_t CursorAddress);					// Calculate cursor row from it's address
uint8_t lcd_cursorpos (void);						// Return address of cursor position

// Optional functions:

#if LCD_ENABLE_GETC
uint8_t lcd_getc (void);							// Read character at cursor
#endif
#if LCD_ENABLE_GOTOXY
void lcd_gotoxy (uint8_t row, uint8_t col);					// Position cursor
void lcd_getxy (uint8_t *row, uint8_t *col);					// Return row and column of cursor position
#endif
#if LCD_ENABLE_UNSCROLL
void lcd_unscroll (void);						// Roll scroll backwards one line
#endif
#if LCD_ENABLE_SCROLL
void lcd_scroll (void);							// Scroll up one line
#endif
#if LCD_ENABLE_CLEAR
void lcd_clear (void);							// Clear LCD screen
#endif
#if LCD_ALLOW_USER_CHARS
void lcd_define_char (uint8_t c, const uint8_t *bitmap);			// Define user-defined char
#endif

#endif
