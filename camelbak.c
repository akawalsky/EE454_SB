/*
 *  camelbak.c
 *  
 *
 *  Created by Student on 4/15/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


#include "liblcd.h"
#include "libsci.h"
#include "libadc.h"
#include "camelbak.h"

/*
 Use the "PROGMEM" attribute to store the strings in the ROM
 insteat of in RAM.
 */

#ifdef NIBBLE_HIGH
const unsigned char str1[] PROGMEM = "123";
#else
const unsigned char str1[] PROGMEM = ">> at328-5.c lo <<901234";
#endif
const unsigned char str2[] PROGMEM = "123";
const unsigned char str3[] PROGMEM = "a";

unsigned char lcd_buf[40];

int heart_rate_calc();
int water_vol_calc();
bool access_heart_rate();

unsigned int curr_volume;

int main(){
	lcd_gpio_init();
	lcd_initialize();
	sci_init();
	adc_init();
	
	curr_volume = 0;
	
	while (1){
		/*lcd_strout(LCD_LINE_I, str1);
		_delay_ms(1000);
		
		lcd_strout(LCD_LINE_II, str2);
		_delay_ms(1000);
		
		lcd_clearline(LCD_LINE_I);
		lcd_strout(LCD_LINE_I, str3);
		_delay_ms(1000);
		
		lcd_clearline(LCD_LINE_II);
		sprintf(lcd_buf, "test: LCD line %d", 100);
		lcd_sstrout(LCD_LINE_II, lcd_buf);
		sprintf(lcd_buf, "test: LCD line %d\r\n", 100);
		sci_outString(lcd_buf);
		_delay_ms(1000);

		*/
		if(access_heart_rate())
		{
			int hr = heart_rate_calc();
			sprintf(lcd_buf, "HR: %d / min", hr);
			lcd_clearline(LCD_LINE_I);
			lcd_sstrout(LCD_LINE_I, lcd_buf);
			_delay_ms(1000);
		}
		
		curr_volume = water_vol_calc();
		sprintf(lcd_buf, "Vol: %d ml", curr_volume);
		lcd_clearline(LCD_LINE_II);
		lcd_sstrout(LCD_LINE_II, lcd_buf);
		
		_delay_ms(4000);
		
		//lcd_strout(LCD_LINE_II, str3);
		lcd_clearscreen();
		_delay_ms(1000);
	}
	return 0;
}

bool access_heart_rate() {
	int i;
	int current;
	int last = 0;
	int beats = 0;
	for (i = 0; i < 10; i++) {
		current = adc_heart_read();
		if (current >= 240 && last < 240) {
			beats++;
		}
		last = current;
		_delay_ms(100);
	}
	return beats >= 1;
}

int heart_rate_calc(){
	lcd_clearline(LCD_LINE_I);
	sprintf(lcd_buf, "Calculating HR...");
	lcd_sstrout(LCD_LINE_I, lcd_buf);
	int i;
	int current;
	int last = 0;
	int beats = 0;
	for (i = 0; i < 100; i++) {
		current = adc_heart_read();
		sprintf(lcd_buf, "[Pulse Test] Pulse value is: %i\r\n", current);
		sci_outString(lcd_buf);
		if (current >= 240 && last < 240) {
			beats++;
		}
		last = current;
		_delay_ms(100);
	}
	return beats * 6;
}

int water_vol_calc(){
	/* Dummy Test*/
	lcd_clearline(LCD_LINE_II);
	sprintf(lcd_buf, "Calculating Vol...");
	lcd_sstrout(LCD_LINE_II, lcd_buf);
	
	int i = 0;
	for(;i < 20; i++){
		sprintf(lcd_buf, "[Pressure Test] Pressure value is: %i\r\n", adc_pressure_read());
		sci_outString(lcd_buf);
		_delay_ms(100);
	}
	return adc_pressure_read();
}