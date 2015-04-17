/*
 *  liblcd.h
 *  
 *
 *	EE 459 Project
 *	LCD related functions
 */

/*
 The NIBBLE_HIGH lscondition determines whether data bits 4-7 or 0-3 are used
 to send the four bit nibble to the LCD.
 If NIBBLE_HIGH is declared, use data bits 4-7.
 If NIBBLE_HIGH not declared, use data bits 0-3.
 */
#ifndef _LIBLCD_H_
#define _LIBLCD_H_

#define NIBBLE_HIGH                 // Use bits 4-7 for talking to LCD



#define LCD_RS          0x80    // PB7 for RS    
#define LCD_RW          0x02    // PB1 for RW
#define LCD_E           0x04    // PB2 for EN
#define LCD_Bits        (LCD_RS|LCD_RW|LCD_E)

#ifdef NIBBLE_HIGH
#define LCD_Data_D     0xf0     // Bits in Port D for LCD data
#define LCD_Status     0x80     // Bit in Port D for LCD busy status
#else
#define LCD_Data_B     0x03     // Bits in Port B for LCD data
#define LCD_Data_D     0x0c     // Bits in Port D for LCD data
#define LCD_Status     0x08     // Bit in Port D for LCD busy status
#endif

#define WAIT           1
#define NOWAIT         0

#define LCD_LINE_I		0x00
#define LCD_LINE_II		0x40
#define LCD_LINE_III	0x14
#define LCD_LINE_IV		0x54




void lcd_gpio_init();
void lcd_initialize(void);
void lcd_strout(int, unsigned char *);
void lcd_sstrout(int, unsigned char*);
void lcd_cmdout(unsigned char, unsigned char);
void lcd_datout(unsigned char);
void lcd_nibout(unsigned char, unsigned char);
void lcd_busywt(void);
void lcd_clearline(int x);
void lcd_clearscreen();

#endif