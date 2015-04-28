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

const unsigned int VOL[256] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
							  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
							  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
							  20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20,
							  40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
							  60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60,
							  80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
							  100, 100, 100, 100, 110, 110, 110, 120, 120, 120, 130, 130, 140, 140, 150, 150,
							  200, 200, 210, 210, 220, 220, 230, 230, 240, 240, 250, 250, 260, 260, 270, 270,
							  280, 280, 290, 290, 300, 300, 310, 310, 320, 320, 330, 330, 340, 340, 350, 350,
							  360, 360, 370, 370, 380, 380, 390, 390, 400, 400, 410, 410, 420, 420, 430, 430,
							  440, 440, 450, 450, 460, 460, 470, 470, 480, 480, 490, 490, 500, 500, 510, 510,
							  520, 520, 530, 530, 540, 540, 550, 550, 560, 560, 570, 570, 580, 580, 590, 590,
							  600, 600, 610, 610, 620, 620, 630, 630, 640, 640, 650, 650, 660, 660, 670, 670,
							  680, 680, 690, 690, 700, 700, 710, 710, 720, 720, 730, 730, 740, 740, 750, 750}; //Redo possibly

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

int vol_map(int pressure) {
	return VOL[pressure];
}
