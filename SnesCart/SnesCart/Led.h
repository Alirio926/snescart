/*
 * Led.h
 *
 * Created: 21/03/2018 00:18:29
 *  Author: aliri
 */ 


#ifndef LED_H_
#define LED_H_

#pragma once

#include <avr/io.h>

volatile uint8_t *m_pddr, *m_pport;
uint8_t m_ledstate, m_dq;

void Init(volatile uint8_t  *ddr,  volatile uint8_t  *port, uint8_t  dq) ;
void Set(uint8_t st);
void Toggle();

#endif /* LED_H_ */