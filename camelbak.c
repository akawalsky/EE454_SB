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

// #ifdef NIBBLE_HIGH
// const unsigned char str1[] PROGMEM = "123";
// #else
// const unsigned char str1[] PROGMEM = ">> at328-5.c lo <<901234";
// #endif
// const unsigned char str2[] PROGMEM = "123";
// const unsigned char str3[] PROGMEM = "a";

unsigned char lcd_buf[40];

unsigned int curr_hr;
unsigned int curr_volume;
unsigned int prev_volume;
unsigned int volume_drank;

int main(){
	global_reset();
	lcd_gpio_init();
	lcd_initialize();
	sci_init();
	sci_switch(SERIALCOM);
	adc_init();
	
	DDRD &= ~(1<<PD2); //Set as input
	PORTD |= (1<<PD2); //Turn on pull up resistor (active low input)

	curr_hr = 0;
	curr_volume = 0;
	prev_volume = 0;
	volume_drank = 0;
	
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
		
		print_current();

		if(access_heart_rate())
		{
			curr_hr = heart_rate_calc();
			// sprintf(lcd_buf, "HR: %d / min", hr);
			// lcd_clearline(LCD_LINE_I);
			// lcd_sstrout(LCD_LINE_I, lcd_buf);
			// _delay_ms(1000);
		}
		
		if(access_vol())
		{
			curr_volume = water_vol_calc();
			calculate_water_drank();
			// sprintf(lcd_buf, "Vol: %d ml", curr_volume);
			// lcd_clearline(LCD_LINE_II);
			// lcd_sstrout(LCD_LINE_II, lcd_buf);
		}
		
		_delay_ms(4000);
		
		//lcd_strout(LCD_LINE_II, str3);
		lcd_clearscreen();
		// _delay_ms(1000);
	}
	return 0;
}

void print_current() {
	sprintf(lcd_buf, "HR: %u BPM", curr_hr);
	lcd_clearline(LCD_LINE_I);
	lcd_sstrout(LCD_LINE_I, lcd_buf);

	sprintf(lcd_buf, "Volume: %u ml", curr_volume);
	lcd_clearline(LCD_LINE_II);
	lcd_sstrout(LCD_LINE_II, lcd_buf);

	sprintf(lcd_buf, "Drank: %d ml", volume_drank);
	// lcd_clearline(LCD_LINE_III);
	lcd_sstrout(LCD_LINE_III, lcd_buf);
}

void global_reset(void){
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x00;
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
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
	// sci_outString(beats);
	return beats >= 1;
}

int heart_rate_calc(){
	// lcd_clearline(LCD_LINE_IV);
	sprintf(lcd_buf, "Calculating HR...");
	lcd_sstrout(LCD_LINE_IV, lcd_buf);
	int i;
	int current;
	int last = 0;
	int beats = 0;
	for (i = 0; i < 100; i++) {
		current = adc_heart_read();
		// sprintf(lcd_buf, "[Pulse Test] Pulse value is: %i\r\n", current);
		// sci_outString(lcd_buf);
		if (current >= 240 && last < 240) {
			beats++;
		}
		last = current;
		_delay_ms(100);
	}
	int hr = beats * 6;
	return (hr > 50 && hr < 150) ? hr : curr_hr;
}

bool access_vol() {
	int downs = 0;
	int i;
	for (i = 0; i < 10; i++) {
		if((PIND & (1<<PD2)) == 0)
		{
			downs++;
		}
		_delay_ms(100);
	}
	return downs > 8;
}

int water_vol_calc(){
	/* Dummy Test*/
	// lcd_clearline(LCD_LINE_IV);
	sprintf(lcd_buf, "Calculating Vol...");
	lcd_sstrout(LCD_LINE_IV, lcd_buf);
	
	int pressure;
	int volume = 0;
	int i = 0;
	for(;i < 20; i++){
		pressure = adc_pressure_read();
		volume += vol_map(pressure);
		// sprintf(lcd_buf, "[Pressure Test] Pressure: %i , Volume: %i\r\n", pressure, vol_map(pressure));
		// sci_outString(lcd_buf);
		_delay_ms(100);
	}
	volume = volume/20;
	//return volume;
	return abs(curr_volume - volume > 20) ? volume : curr_volume;
}

void calculate_water_drank() {
	if(curr_volume < prev_volume)
	{
		volume_drank += (prev_volume - curr_volume);
	}
	prev_volume = curr_volume;
}
