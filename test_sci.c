/*
 *  test_sci.c
 *  EE459
 *
 *  Created by Student on 4/22/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "libsci.h"
#include "liblcd.h"

unsigned char inbuf[40];
unsigned char outbuf[40];

void blt_send_test();
void blt_recv_test();
//void blt_cmd_test();
void lcd_test();
int main (){
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	
	sci_init();
	sci_switch(BLUETOOTH);
	lcd_gpio_init();
	lcd_initialize();
	
	while(1){
		//lcd_test();
		//_delay_ms(1000);
		blt_send_test();
		//_delay_ms(1000);
		//blt_recv_test();
		sci_inString(inbuf);
		int i = atoi(inbuf);
		i = i * 10 + 5;
		sprintf(outbuf, "number is: %d", i);
		lcd_clearline(LCD_LINE_I);
		lcd_sstrout(LCD_LINE_I, outbuf);
	}
	return 0;
}

void lcd_test(){
	lcd_clearscreen();
	_delay_ms(500);
	sprintf(outbuf, "LCD Test Line %d", 1);
	lcd_clearline(LCD_LINE_I);
	lcd_sstrout(LCD_LINE_I, outbuf);
	sprintf(outbuf, "LCD Test Line %d", 2);
	lcd_clearline(LCD_LINE_II);
	lcd_sstrout(LCD_LINE_II, outbuf);
}

void blt_send_test(){
	/* Send a string */
	sprintf(outbuf, "Hello Bluetooth\n");
	sci_outString(outbuf);
	_delay_ms(100);
	/* Send another string */
	sprintf(outbuf, "Symbols: + - * / \n");
	sci_outString(outbuf);
}

void blt_recv_test(){
	/* receive a char */
	unsigned char ch = (unsigned char)sci_inChar();

	/* print on lcd */
	sprintf(outbuf, "char recv.: %c", ch);
	lcd_clearline(LCD_LINE_I);
	lcd_sstrout(LCD_LINE_I, outbuf);

	/*receive a string*/
	sci_inString(inbuf);
	sprintf(outbuf, "line recv: %s", inbuf);
	lcd_clearline(LCD_LINE_II);
	lcd_sstrout(LCD_LINE_II, outbuf);
}

