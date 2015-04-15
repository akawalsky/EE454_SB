/*
 *  pulse.c
 *  
 *
 * Read pulse sensor and then print it out use serial port using interrupt
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

// SCI Library:
void sci_init(void);                    // Initialize Serial Communication Interface (SCI) Port
void sci_outChar(unsigned char);        // Output 1 Byte to SCI Port
void sci_outString(unsigned char *);    // Output Null-Terminated String to SCI Port
char sci_inChar(void);                  // Input 1 Byte from SCI Port

// ADC Library
void adc_init();
void adc_config();
void adc_interrupt_en();
uint16_t adc_read(uint8_t adcx);

// Tests
void printTest();
void pulseSensorTest();

int heart_rate();

// Defined Serial Communication Variables
#define FOSC 9830400                    // Clock frequency
#define BAUD 9600                       // BAUD rate used by EE459 Terminal
#define MYUBRR FOSC/16/BAUD-1           // Value for UBRR0 register

// Define ADC Variables
#define ADC_PULSE	3					// PC 3




int main (void){
	// Enabling Global Interrupts
	sei();
	// Enable serial communication
	sci_init();
	
	// Initialize ADC
	adc_init();
	
	
	 // Read from PC3 
	ADMUX |= (1 << MUX1) | (1 << MUX0); // HEARTBEAT
	
	// Read From PC2
	// ADMUX |= (1 << MUX2); // PRESSURE
	
	// Set ref
	ADMUX |= 1 << REFS0;
	
	// Start ADC Conversion
	
	ADCSRA |= 1 << ADSC;
	while(1){
		unsigned char buf[40];
		sprintf(buf, "[Heart Beat] Pulse value is: %i\r\n", heart_rate());
		sci_outString(buf);
		//sci_outString("Main continues\n");
	}
	return 0;
}
// the interrupt routine
ISR(ADC_vect){
//	while(! ADCSRA & (1<<ADIF)){
//	}
//	int res = (int)ADCH;
//	unsigned char buf[40];
//	sprintf(buf, "[Pulse Test] Pulse value is: %i\r\n", res);
//	sci_outString(buf);
//	_delay_ms(100);
//	ADCSRA |= 1 << ADSC;
//	
}

int heart_rate()
{
	int i;
	int current;
	int last = 0;
	int beats = 0;
	for (i = 0; i < 100; i++) {
		ADCSRA |= 1 << ADSC;
		while(ADCSRA & (1<<ADSC)){}
		current = (int)ADCH;
		if (current >= 240 && last < 240) {
			beats++;
		}
		last = current;
		_delay_ms(100);
	}
	return beats * 6;
}





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

}

uint16_t adc_read(uint8_t adcx) {
	/* adcx is the analog pin we want to use.  ADMUX's first few bits are
	 * the binary representations of the numbers of the pins so we can
	 * just 'OR' the pin's number with ADMUX to select that pin.
	 * We first zero the four bits by setting ADMUX equal to its higher
	 * four bits. */
	ADMUX	&=	0xf0;
	ADMUX	|=	adcx;
	
	/* This starts the conversion. */
	ADCSRA |= _BV(ADSC);
	
	/* This is an idle loop that just wait around until the conversion
	 * is finished.  It constantly checks ADCSRA's ADSC bit, which we just
	 * set above, to see if it is still set.  This bit is automatically
	 * reset (zeroed) when the conversion is ready so if we do this in
	 * a loop the loop will just go until the conversion is ready. */
	while ( (ADCSRA & _BV(ADSC)) );
	
	/* Finally, we return the converted value to the calling function. */
	return ADC;
}








// Tests Begin

void printTest(){
	sci_outString("Begin of print Test\r\n");
	unsigned char buf[40];
	int i = 0;
	for (; i < 10; i++){
		sprintf(buf, "%u\r\n", i);
		sci_outString(buf);
		_delay_ms(500);
	}
}

void pulseSensorTest(){
	uint16_t res = adc_read(ADC_PULSE);
	//uint16_t res = 0;
	unsigned char buf[80];
	while(1){
		sprintf(buf, "[Pulse Test] Pulse value is: %u\r\n", res);
		sci_outString(buf);
		_delay_ms(500);
	}
}



