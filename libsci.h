/*
 *  libsci.h
 *  
 *
 *  Created by Student on 4/15/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _LIBSCI_H_
#define _LIBSCI_H_

// Defined Serial Communication Variables
#define FOSC 9830400                    // Clock frequency
#define BAUD 9600                       // BAUD rate used by EE459 Terminal
#define MYUBRR FOSC/16/BAUD-1           // Value for UBRR0 register

// SCI Library:
void sci_init(void);                    // Initialize Serial Communication Interface (SCI) Port
void sci_outChar(unsigned char);        // Output 1 Byte to SCI Port
void sci_outString(unsigned char *);    // Output Null-Terminated String to SCI Port
char sci_inChar(void);                  // Input 1 Byte from SCI Port


#endif