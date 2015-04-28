/*
 *  libadc.h
 *  
 *
 *  Created by Student on 4/15/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _LIBADC_H_
#define _LIBADC_H_

#define HEART_MUX			((1 << MUX1) | (1 << MUX0))
#define PRESSURE_MUX		(1<<MUX2)

void adc_init();
int adc_heart_read();
int adc_pressure_read();
#endif