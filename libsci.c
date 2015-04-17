/*
 *  libsci.c
 *  
 *
 *  Created by Student on 4/15/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

#include "libsci.h"

// Initialize Serial Communication Interface (SCI) Port
void sci_init(void)
{
    UBRR0 = MYUBRR;             // Set baud rate
    UCSR0B |= (1 << TXEN0);     // Turn on transmitter
    UCSR0B |= (1 << RXEN0);     // Turn on receiver
    UCSR0C = (3 << UCSZ00);     // Set for asynchronous operation, no parity, one stop bit, 8 data bits
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
