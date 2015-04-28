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
#define FOSC			14745600							// Clock frequency

#define SCI_BAUD		9600							// BAUD rate used by EE459 Terminal
#define SCI_UBRR		FOSC/16/SCI_BAUD-1				// Value for UBRR0 register

#define BLT_BAUD		115200
#define BLT_UBRR		FOSC/16/BLT_BAUD-1				// Value for UBRR0 register





#define SERIALCOM	0
#define BLUETOOTH	1

// SCI Library:
void sci_init(void);                    // Initialize Serial Communication Interface (SCI) Port
void sci_switch(int);					// Switch between Bluetooth and Serial communication
void sci_outChar(unsigned char);        // Output 1 Byte to SCI Port
void sci_outString(unsigned char *);    // Output Null-Terminated String to SCI Port
char sci_inChar(void);                  // Input 1 Byte from SCI Port
void sci_inString(unsigned char*);		// Input a string (< 40 bytes) from sci port


#endif
