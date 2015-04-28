/*
 *  libbluetooth.c
 *  EE459
 *
 *  Created by Student on 4/22/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#include "libbluetooth.h"

/* Set baud rate, enable serial communication, set frame format */
void blt_init(){
	/* Set baud */
	UBRR0H = (unsigned char) (BLT_UBRR >> 8);
	UBRR0L = (unsigned char) BLT_UBRR;
	
	/* Enable Receiver and */
}