/*
 *  libsci.c
 *  
 *
 *  Created by Student on 4/15/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#include "libsci.h"

// Initialize Serial Communication Interface (SCI) Port
void sci_init(void)
{
    DDRD |= (1<<DDD3);			// Set PD3 as selection output
    UCSR0B |= (1 << TXEN0);     // Turn on transmitter
    UCSR0B |= (1 << RXEN0);     // Turn on receiver
	UCSR0C &= ~(1<<USBS0);
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01); // 8 bit mode
   // UCSR0C = (3 << UCSZ00);     // Set for asynchronous operation, no parity, one stop bit, 8 data bits
}

void sci_switch(int device){
	if (device == 0){
		// Set Serial Communication
	    UBRR0 = SCI_UBRR;
        //UBRR0H = (unsigned char) (SCI_UBRR >> 8);
        //UBRR0L = (unsigned char) SCI_UBRR;

		PORTD &= ~(1<<PORTD3);	// Turn off Select Bit
	}else {
		// Set Bluetooth
		UBRR0 = BLT_UBRR;
        //UBRR0H = (unsigned char) (BLT_UBRR >> 8);
        //UBRR0L = (unsigned char) BLT_UBRR;
		
        PORTD |= 1<<PORTD3;	// Turn off Select Bit
	}
}

// Output 1 Byte to SCI Port
void sci_outChar(unsigned char inChar)
{
    while ( ( UCSR0A & (1 << UDRE0) ) == 0 )
    {
        
    }
    UDR0 = inChar;
}


// Output Null-Terminated String to SCI Port
void sci_outString(unsigned char* inString)
{
    unsigned char inChar;
    while ( ( inChar = *inString++ ) != (unsigned char)'\0' )
    {
        sci_outChar(inChar);
    }
}


// Input 1 Byte from SCI Port
char sci_inChar(void)
{
    while ( !( UCSR0A & (1 << RXC0) ) )
    {
        
    }
    return UDR0;
}

void sci_inString(unsigned char* buf){
    int i = 0;
    unsigned char ch = (unsigned char)sci_inChar();
    while (i < 40 && ch != '>'){
        buf[i] = ch;
        i++;
        ch = (unsigned char)sci_inChar();
    }
    buf[i] = (unsigned char)'\0';
}
