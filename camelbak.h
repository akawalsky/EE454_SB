/*
 *  camelbak.h
 *  
 *
 *  Main functions. General declares. 
 *
 */

#ifndef _CAMELBAK_H_
#define _CAMELBAK_H_

#ifndef NULL
#define NULL 0L
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

void print_current();

void global_reset(void);
int heart_rate_calc();
int water_vol_calc();
bool access_heart_rate();
bool access_vol();

void calculate_water_drank();

#endif
