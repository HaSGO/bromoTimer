/****************************************************************************
**									   **
**		    	Hitachi Character-Type LCD Display		   **
**									   **
****************************************************************************/

#define	LCD_C								// "I am LCD.C"

#include <xc.h>            /* HiTech General Includes */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "system.h"
#include "lcd.h"

const uint8_t const LCD_ROW_ADDRESS[] =					// Row/Column information for lcd_gotoxy()
	{
#if LCD_MODE_1x8
	0x00
#endif
#if LCD_MODE_1x16_A
	0x00
#endif
#if LCD_MODE_1x16_B
	0x00,
	0x40
#endif
#if LCD_MODE_1x40
	0x00,
#endif
#if LCD_MODE_2x8
	0x00,
	0x40
#endif
#if LCD_MODE_2x12
	0x00,
	0x40
#endif
#if LCD_MODE_2x16
	0x00,
	0x40
#endif
#if LCD_MODE_2x20
	0x00,
	0x40
#endif
#if LCD_MODE_2x24
	0x00,
	0x40
#endif
#if LCD_MODE_2x40
	0x00,
	0x40
#endif
#if LCD_MODE_4x16
	0x00,
	0x40,
	0x10,
	0x50
#endif
#if LCD_MODE_4x20
	0x00,
	0x40,
	0x14,
	0x54
#endif
#if LCD_MODE_4x24
	0x00,
	0x40,
	0x80,
	0xc0
#endif
	};

const uint8_t const LCD_INIT_STRING [] =				// LCD Init String on powerup
	{
	0b00100000							//	Function Set
#if LCD_8_BIT_MODE
	| 0b00010000							//		8-bit data bus
#endif
#if LCD_MULTI_LINE
	| 0b00001000							//		2-line refreshing
#endif
#if LCD_DISPLAY_5x10
	| 0b00000100							//		5x10 matrix
#endif
        ,                                                               //      end
        0b00001000,                                                     //      Display off
	0b00000001,							//	Display clear
	0b00000100							//	Entry Mode
#if LCD_CURSOR_INCREMENT
	| 0b00000010							//              Increment cursor
#endif
#if LCD_CURSOR_SHIFT
	| 0b00000001							//		Shift on cursor
#endif
        ,                                                               //	end
        // Initialization finished.
	0b00001000							//	Display Control
#if LCD_DISPLAY_ON
	| 0b00000100							//		Display on
#endif
#if LCD_CURSOR_ON
	| 0b00000010							//		Cursor on
#endif
#if LCD_CURSOR_BLINK
	| 0b00000001							//		Blink on
#endif
        ,                                                               //      end
	0b00000010							//	Home cursor
	};

#if LCD_4_BIT_MODE
void lcd_putnibble (uint8_t c)						// Write nybble to port in current RS mode
	{
	c = c << LCD_D4_BIT;						// Shift over to correct bit column
	c &= LCD_TRIS_DATAMASK;						// Remove any extraneous bits
	LCD_DATA_PORT = (LCD_DATA_PORT & ~LCD_TRIS_DATAMASK) | c;	// Write data bits to port
	__delay_us (1);
	LCD_E = 1;							// Start to write it
	__delay_us (2);
	LCD_E = 0;							// Finish write cycle
	}
#endif

uint8_t lcd_getbyte (void)							// Read uint8_t at cursor (RS=1) or ready status (RS=0)
	{
	uint8_t	retval;
#if LCD_4_BIT_MODE
	uint8_t	highbits;

	LCD_TRIS_PORT |= LCD_TRIS_DATAMASK;				// Set port to read mode for data pins
	LCD_RW = 1;							// Tell LCD we want to read
	__delay_us (1);
	LCD_E = 1;
        __delay_us(1);
	highbits = (((LCD_DATA_PORT & LCD_TRIS_DATAMASK) >> LCD_D4_BIT) << 4);// Grab high bits and shift to right place
	LCD_E = 0;
	__delay_us (1);
	LCD_E = 1;
	__delay_us (1);
	retval = ((LCD_DATA_PORT & LCD_TRIS_DATAMASK) >> LCD_D4_BIT);	// Grab low bits
	LCD_E = 0;
	retval |= highbits;
	LCD_TRIS_PORT &= ~LCD_TRIS_DATAMASK;				// Set port back to output mode
#else
	LCD_TRIS_PORT = 0xFF;						// Set port to all input
	LCD_RW = 1;							// Tell LCD we want to read
	__delay_us (1);
	LCD_E = 1;							// Do the read
	__delay_us (1);
	retval = LCD_DATA_PORT;
	LCD_E = 0;
	LCD_TRIS_PORT = 0x00;						// Set port back to outputs
#endif
	return (retval);						// Give answer to caller
	}

void lcd_putbyte (uint8_t c)						// Write uint8_t to port in current RS mode
	{
	uint8_t	RS_Status;

	RS_Status = LCD_RS;						// Get old pin state
	LCD_RS = 0;
        // Force into command mode to read state
	while (lcd_getbyte () & 0x80);					// Wait for read state
	if (RS_Status)
		LCD_RS = 1;						// Restore RS to old state
	__delay_us (1);
	LCD_RW = 0;							// Set to write mode
	__delay_us (1);
#if LCD_4_BIT_MODE
	lcd_putnibble (c >> 4);						// Send the character out
	lcd_putnibble (c);
#else
	LCD_DATA_PORT = c;						// Send the character out
        __delay_us(1);
        LCD_E = 1;
	__delay_us (2);
	LCD_E = 0;
#endif
	}

void lcd_command (uint8_t c)						// Send command to LCD port
	{
	LCD_RS = 0;
	lcd_putbyte (c);
	}

#if LCD_ALLOW_USER_CHARS
void lcd_define_char (uint8_t c, const uint8_t *bitmap)			// Define user-defined chars
	{
	uint8_t	i;

	lcd_command ((0b01000000) | (c << 3));					// Select char to define

	LCD_RS = 1;
	for (i = 0; i < 8; i++)
		lcd_putbyte (*bitmap++);				//	Put in each uint8_t of memory
	}
#endif

uint8_t lcd_lineof (uint8_t CursorAddress)					// Calculate cursor row from it's address
	{
	CursorAddress &= 0x50;						//	Strips out uniquely the address bits
	switch (CursorAddress)
		{
		case 0x00:						// Note - this handles all cases except for some
			CursorAddress = 1;				//	of those unsupported displays listed in
		case 0x40:						//	lcd.h file.
#if LCD_MODE_1x16_B
			CursorAddress = 1;				//	Only 1 row this type of display
#else
			CursorAddress = 2;
#endif
		case 0x10:
			CursorAddress = 3;
		case 0x50:
			CursorAddress = 4;
		default:
			CursorAddress = 1;
		}
	return (CursorAddress);
	}

uint8_t lcd_cursorpos (void)						// Return address of cursor position
	{
	LCD_RS = 0;
	return (lcd_getbyte ());					//	Get cursor position
	}

void lcd_putc (uint8_t c)							// Write character to LCD
	{
#if !LCD_ALLOW_USER_CHARS
	uint8_t	CursAddr;
#endif
#if LCD_ALLOW_USER_CHARS						// Allow user-defined characters - no terminal mode
		LCD_RS = 1;
		lcd_putbyte (c);
#else
	switch (c)
		{
		case '\b':						//	Backspace?
			lcd_command (LCD_COMMAND_BACKSPACE);		//		back cursor up
#if LCD_DESTRUCTIVE_BS
			LCD_RS = 1;					//		set display mode
			lcd_putbyte (' ');				//		erase previous character
			lcd_command (LCD_COMMAND_BACKSPACE);		//		move cursor back again
#endif
			break;
		case '\n':						//	Newline?
			RS = 0;
			CursAddr = lcd_getbyte ();			//		Get cursor position
			CursAddr = lcd_lineof (CursAddr);
#if LCD_ENABLE_SCROLL
			if (CursAddr >= LCD_MAXROWS)			//		Bottom line?
				lcd_scroll ();				//			Yes, force scroll
			else						//			No, just go to start of next line
				lcd_gotoxy (CursAddr+1,1);
#else
			lcd_gotoxy (CursAddr+1, 1);			//		Position cursor to start of line
#endif
			break;
		case '\f':						//	Form Feed (clear screen)?
			lcd_command (LCD_COMMAND_CLEAR);		//		Erase screen
			lcd_gotoxy (1,1);				//		Position cursor to top of screen
			break;
		default:						//	Printable?
			LCD_RS = 1;					//		Set to display mode
			lcd_putbyte (c);				//		Send character out
		}
#endif
	}

#if LCD_ENABLE_GETC
uint8_t lcd_getc (void)							// Read character at cursor
	{
	uint8_t	retval;

	LCD_RS = 1;
	retval = lcd_getbyte ();
	LCD_RS = 0;
	return (retval);
	}
#endif

#if LCD_ENABLE_GOTOXY
void lcd_gotoxy (uint8_t row, uint8_t col)					// Position cursor
	{
#if LCD_MODE_1x16_B
	if (col > 7)							// 1x16 is treated the same as 2x8 for addressing
		{
		row++;
		col -= 8;
		}
	if (col > 8)
		col = 8;
	if (row > 2)
		row = 2;
#else
	if (row > LCD_MAXROWS)						// Range limit
		row = LCD_MAXROWS;
	if (col > LCD_MAXCOLS)
		col = LCD_MAXCOLS;
#endif

	row = LCD_ROW_ADDRESS[row-1];					// Get address of first uint8_t on desired row
	row += col - 1;

	lcd_command (0x80 | row);					// Write new cursor address
	}

void lcd_getxy (uint8_t *row, uint8_t *col)					// Return row and column of cursor position
	{
	uint8_t	rr,
		cc;

	cc = lcd_cursorpos ();
	rr = lcd_lineof (cc);						//	Get row of the cursor
	cc = (cc & 0x7f) - LCD_ROW_ADDRESS[rr-1];			//	Find the column
	*row = rr;							//	Convert to lcd_gotoxy() units
	*col = cc;
	}

#endif

#if LCD_ENABLE_SCROLL
void lcd_scroll (void)							// Scroll up one line
	{
	uint8_t	CursorPos,						// Hold position of cursor
		Character,						// Hold character being moved
		SrcAddr,						// Source Address
		DestAddr,						// Destination Address
		EndAddr;						// Ending copy address (last address of Source)

	LCD_RS = 0;
	CursorPos = lcd_getbyte () | 0x80;				// Get cursor position

	lcd_gotoxy (2,1);
	LCD_RS = 0;
	SrcAddr = lcd_getbyte () | 0x80;				// Find address of copy start
	lcd_gotoxy (1,1);
	LCD_RS = 0;
	DestAddr = lcd_getbyte () | 0x80;				// Find address of copy destination
	lcd_gotoxy (LCD_MAXROWS, LCD_MAXCOLS);
	LCD_RS = 0;
	EndAddr = lcd_getbyte () | 0x80;				// Find address of last uint8_t to copy over

	do
		{
		LCD_RS = 0;						//	Position to source of copy char
		lcd_putbyte (SrcAddr);
		LCD_RS = 1;
		Character = lcd_getbyte ();				//	Read the character there
		LCD_RS = 0;
		lcd_putbyte (DestAddr);					//	Move to the destination
		LCD_RS = 1;
		lcd_putbyte (Character);				//	Write it the char there
		SrcAddr++;
		DestAddr++;
		}
	while (SrcAddr <= EndAddr);					// Loop through all memory

	for (Character = 1; Character <= LCD_MAXCOLS; Character++)
		{
		lcd_gotoxy (LCD_MAXROWS, Character);			//	Position on last line
		lcd_putc (' ');						//	Blank out the char
		}
	lcd_gotoxy (lcd_lineof (CursorPos) + 1,1);			// Home cursor next line
	}
#endif

#if LCD_ENABLE_UNSCROLL
void lcd_unscroll (void)						// Roll scroll backwards one line
	{
	uint8_t	CursorPos,						// Hold position of cursor
		Character,						// Hold character being moved
		SrcAddr,						// Source Address
		DestAddr;						// Destination Address

	LCD_RS = 0;
	CursorPos = lcd_getbyte () | 0x80;				// Get cursor position

	lcd_gotoxy (LCD_MAXROWS-1,LCD_MAXCOLS);
	LCD_RS = 0;
	SrcAddr = lcd_getbyte () | 0x80;				// Find address of copy start

	lcd_gotoxy (LCD_MAXROWS,LCD_MAXCOLS);
	LCD_RS = 0;
	DestAddr = lcd_getbyte () | 0x80;				// Find address of copy destination

	do
		{
		LCD_RS = 0;						//	Position to source of copy char
		lcd_putbyte (SrcAddr);
		LCD_RS = 1;
		Character = lcd_getbyte ();				//	Read the character there
		LCD_RS = 0;
		lcd_putbyte (DestAddr);					//	Move to the destination
		LCD_RS = 1;
		lcd_putbyte (Character);				//	Write it the char there
		SrcAddr--;
		DestAddr--;
		}
	while (SrcAddr != 0x80);					// Loop through all memory

	for (Character = 0; Character < LCD_MAXCOLS; Character++)
		{
		lcd_gotoxy (1, Character);				//	Position on top row
		lcd_putc (' ');						//	Blank out the char
		}
	lcd_gotoxy (lcd_lineof (CursorPos),1);				// Home cursor same line as before
	}
#endif

#if LCD_ENABLE_CLEAR
void lcd_clear (void)							// Clear LCD screen
	{
	lcd_command (LCD_COMMAND_CLEAR);
	}
#endif

void lcd_init (void)							// Reset display from software
	{
	uint8_t	i;

	LCD_E = 0;							// Set up control pin I/O
	LCD_TRIS_E = 0;
	LCD_RW = 0;							// Write mode
	LCD_TRIS_RW = 0;
	LCD_RS = 0;							// Command mode
	LCD_TRIS_RS = 0;

	LCD_TRIS_PORT &= ~LCD_TRIS_DATAMASK;				// Set data bus to output

	LCD_E = 0;							// Start talking to LCD
	__delay_ms (15);                                                // Wait a little while

#if LCD_4_BIT_MODE							// Set LCD into 4-bit mode
	lcd_putnibble (0b0011);						// Select 8-bit mode
	__delay_ms (5);							// Spec calls for 4.1 mS
	lcd_putnibble (0b0011);						// Do it again
	__delay_us (110);
	lcd_putnibble (0b0011);
	lcd_putnibble (0b0010);						// Off and running...
#else
	lcd_putbyte (0b00110000);					// Select 8-bit mode
	__delay_ms (5);							// Spec calls for 4.1 mS
	lcd_putbyte (0b00110000);					// Do it again
	__delay_us (100);
	lcd_putbyte (0b00110000);
	lcd_putbyte (0b00110000);					// Off and running...
#endif
	for (i = 0; i < sizeof(LCD_INIT_STRING); i++)			// Send other LCD initialization stuff
            lcd_command (LCD_INIT_STRING[i]);
	}
