/*
 *  libbluetooth.h
 *  EE459
 *
 *  Created by Student on 4/22/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _LIBBLUETOOTH_H_
#define _LIBBLUETOOTH_H_

/* Bluetooth using serial communication
 * Asynchronous Normal Mode
 */
#define CLK			9830400						// Clock frequency
#define BLT_BAUD	115200						// BAUD rate used by EE459 Terminal
#define BLT_UBRR	CLK/16/BLT_BAUD-1           // Value for UBRR0 register

void blt_init();


#endif