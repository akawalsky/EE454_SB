/*
 *  liblcd.c
 *  
 *
 *  Created by Student on 4/15/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "liblcd.h"
#include "libsci.h"

const unsigned char clear[] PROGMEM = "                                        ";

void lcd_gpio_init(){
#ifdef NIBBLE_HIGH
    DDRD |= LCD_Data_D;         // Set PORTD bits 4-7 for output
#else
    DDRB |= LCD_Data_B;         // Set PORTB bits 0-1 for output
    DDRD |= LCD_Data_D;         // Set PORTD bits 2-3 for output
#endif
	
    DDRB |= LCD_Bits;           // Set PORTB bits 2, 3 and 4 for output
	
	lcd_cmdout(0x01, NOWAIT);         // Clear the LED
}


/*
 initialize - Do various things to force a initialization of the LCD
 display by instructions, and then set up the display parameters and
 turn the display on.
 */
void lcd_initialize()
{
    _delay_ms(15);              // Delay at least 15ms
	
    lcd_nibout(0x30, NOWAIT);       // Send a 0x30
    _delay_ms(4);               // Delay at least 4msec
	
    lcd_nibout(0x30, NOWAIT);       // Send a 0x30
    _delay_us(120);             // Delay at least 100usec
	
    lcd_nibout(0x30, NOWAIT);       // Send a 0x30
	
    lcd_nibout(0x20, WAIT);         // Function Set: 4-bit interface
    
    lcd_cmdout(0x28, WAIT);         // Function Set: 4-bit interface, 2 lines
	
    lcd_cmdout(0x0f, WAIT);         // Display and cursor on
}


/*
 cmdout - Output a byte to the LCD display instruction register.  If
 "wait" is non-zero, wait for the busy flag to reset before returning.
 If "wait" is zero, return immediately since the BUSY flag isn't
 working during initialization.
 */
void lcd_cmdout(unsigned char x, unsigned char wait)
{
    PORTB &= ~LCD_Bits;         // Set R/W=0, E=0, RS=0
    lcd_nibout(x, NOWAIT);
    lcd_nibout(x << 4, wait);
}


/*
 datout - Output a byte to the LCD display data register (the display)
 and wait for the busy flag to reset.
 */
void lcd_datout(unsigned char x)
{
    PORTB &= ~(LCD_RW|LCD_E);   // Set R/W=0, E=0, RS=1
    PORTB |= LCD_RS;
    lcd_nibout(x, NOWAIT);
    lcd_nibout(x << 4, WAIT);
}

/*
 nibout - Puts bits 4-7 from x into the four bits that we're
 using to talk to the LCD.  The other bits of the port are unchanged.
 Toggle the E control line low-high-low.
 */
void lcd_nibout(unsigned char x, unsigned char wait)
{
#ifdef NIBBLE_HIGH
    PORTD |= (x & LCD_Data_D);  // Put high 4 bits of data in PORTD
    PORTD &= (x | ~LCD_Data_D);
#else
    PORTB |= (x & LCD_Data_B);  // Put low 2 bits of data in PORTB
    PORTB &= (x | ~LCD_Data_B);
    PORTD |= (x & LCD_Data_D);  // Put high 2 bits of data in PORTD
    PORTD &= (x | ~LCD_Data_D);
#endif
    PORTB |= LCD_E;             // Set E to 1
    PORTB &= ~LCD_E;            // Set E to 0
    if (wait)
        lcd_busywt();               // Wait for BUSY flag to reset
}

/*
 strout - Print the contents of the character string "s" starting at LCD
 RAM location "x".  The string must be terminated by a zero byte.
 */
void lcd_strout(int x, unsigned char *s)
{
    unsigned char ch;
	
    lcd_cmdout(x | 0x80, WAIT);     // Make A contain a Set Display Address command
	
    /* Use the "pgm_read_byte()" routine to read the date from ROM */
    while ((ch = pgm_read_byte(s++)) != (unsigned char) '\0') {
        lcd_datout(ch);             // Output the next character
    }
}


/*
 * sstrout - Print the string, from stack
 */
void lcd_sstrout(int x, unsigned char *s){
	unsigned char *idx = s;
	lcd_cmdout(x|0x80, WAIT);
	while(*idx != (unsigned char)'\0'){
		lcd_datout(*idx);
		idx++;
	}
}



/*
 busywt - Wait for the BUSY flag to reset
 */
void lcd_busywt()
{
    unsigned char bf;
	
#ifdef NIBBLE_HIGH
    PORTD &= ~LCD_Data_D;       // Set for no pull ups
    DDRD &= ~LCD_Data_D;        // Set for input
#else
    PORTB &= ~LCD_Data_B;       // Set for no pull ups
    PORTD &= ~LCD_Data_D;
    DDRB &= ~LCD_Data_B;        // Set for input
    DDRD &= ~LCD_Data_D;
#endif
	
    PORTB &= ~(LCD_E|LCD_RS);   // Set E=0, R/W=1, RS=0
    PORTB |= LCD_RW;
	
    do {
        PORTB |= LCD_E;         // Set E=1
        _delay_us(1);           // Wait for signal to appear
        bf = PIND & LCD_Status; // Read status register high bits
        PORTB &= ~LCD_E;        // Set E=0
		PORTB |= LCD_E;         // Need to clock E a second time to fake
		PORTB &= ~LCD_E;        //   getting the status register low bits
    } while (bf != 0);          // If Busy (PORTD, bit 7 = 1), loop
	
#ifdef NIBBLE_HIGH
    DDRD |= LCD_Data_D;         // Set PORTD bits for output
#else
    DDRB |= LCD_Data_B;         // Set PORTB, PORTD bits for output
    DDRD |= LCD_Data_D;
#endif
}

void lcd_clearline(int x){
	lcd_strout(x, clear);
}


void lcd_clearscreen(){
	lcd_cmdout(0x01, NOWAIT);
}













