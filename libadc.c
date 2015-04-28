/*
 *  libadc.c
 *  
 *
 *  Created by Student on 4/15/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "libadc.h"

void adc_init(){
	// Reset ADCSRA
	
	// Configure ADC
	
	// Enable interrupts function in ADC
	// ADCSRA |= 1 << ADIE;
	
	/* 8-bit / 10-bit mode
	 * ADCH for ADC9...2, ADCL for ADC1...0
	 */
	ADMUX |= 1 << ADLAR;
	
	/* Enable a prescaler - determined by the internal/external clock
	 * ADC input frequency needs to be between 50khz and 200khz
	 * We are using 10-bit model as the pressure needs accuracy
	 * 9830400/200000 = 49; 9030400 / 50000 = 196
	 * prescalar = 64 or 128 (Table 24-5) Using 64 here
	 * 64 => ADPS2...0 = 110
	 * 128 => ADPS2...0 = 111
	 */
	ADCSRA |= 1 << ADPS2;
	ADCSRA |= 1 << ADPS1;
	
	// Turn on ADC feature
	ADCSRA |= 1 << ADEN;
	
	// Set ref
	ADMUX |= 1 << REFS0;
	
}

int adc_heart_read(){
	int adc_value;
	// Set mux
	ADMUX &= ~PRESSURE_MUX;
	ADMUX |= HEART_MUX;
	
	ADCSRA |= 1 << ADSC;
	
	// Start ADC Conversion
	ADCSRA |= 1 << ADSC;
	
	// Wait for flag
	// while(! (ADCSRA & (1<<ADIF))){}
	
	while(ADCSRA & (1<<ADSC)){}
	
	adc_value = (int)ADCH;
	return adc_value;
}

int adc_pressure_read(){
	int adc_value;
	// Set mux
	ADMUX &= ~ HEART_MUX;
	ADMUX |= PRESSURE_MUX;
	
	// Start ADC Conversion
	ADCSRA |= 1 << ADSC;
	
	// Wait for flag
	// while(! (ADCSRA & (1<<ADIF))){}
	
	while(ADCSRA & (1<<ADSC)){}
	
	adc_value = (int)ADCH;
	return adc_value;
}
