/*
 *  reset.c
 *  EE459
 *
 *  Created by Student on 4/22/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */


#include <avr/io.h>

int main(){
	DDRD = 0xff;
	PORTD = 0x00;
	while(1){
	}
	return 0;
}