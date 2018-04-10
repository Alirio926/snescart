/*
 * _74hc165.c
 *
 * Created: 07/04/2018 18:12:16
 *  Author: aliri
 */ 
#define F_CPU 8000000UL

#include <string.h>
#include <util/delay.h>

#include "l74hc165.h"

/*
 * init the shift register
 */
void l74hc165_init() {
	//output
	L74HC165_DDR |= (1 << L74HC165_CLOCKPIN);
	//L74HC165_DDR |= (1 << L74HC165_LOADPIN);
	//input
	L74HC165_DDR &= ~(1 << L74HC165_DATAPIN);
	//low
	L74HC165_PORT &= ~(1 << L74HC165_CLOCKPIN);
	//L74HC165_PORT &= ~(1 << L74HC165_LOADPIN);
}

/*
 * shift in data
 */
uint8_t l74hc165_shiftin() {
	//parallel load to freeze the state of the data lines
	//L74HC165_PORT &= ~(1 << L74HC165_LOADPIN);
	//_delay_us(5);
	//L74HC165_PORT |= (1 << L74HC165_LOADPIN);
	uint8_t currentbyte = 0;
		for(uint8_t j = 0; j < 8; j++){
			currentbyte |= ((L74HC165_PIN & (1 << L74HC165_DATAPIN))>>L74HC165_DATAPIN)<<(7-j);
	        //get next
			L74HC165_PORT |= (1 << L74HC165_CLOCKPIN);
	        _delay_us(5);
	        L74HC165_PORT &= ~(1 << L74HC165_CLOCKPIN);
		}
		return currentbyte;	
}
