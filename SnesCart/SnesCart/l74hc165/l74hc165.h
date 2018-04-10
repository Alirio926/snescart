/*
 * _74hc165.h
 *
 * Created: 07/04/2018 18:12:25
 *  Author: aliri
 */ 

#include <avr/io.h>

#ifndef L74HC165_H_
#define L74HC165_H_

//setup ports
#define L74HC165_DDR DDRD
#define L74HC165_PORT PORTD
#define L74HC165_PIN PIND
#define L74HC165_CLOCKPIN PD6
#define L74HC165_DATAPIN PD7

//setup number of chip attached to the board
#define L74HC165_ICNUMBER 1

extern void l74hc165_init();
extern uint8_t l74hc165_shiftin();


#endif /* 74HC165_H_ */