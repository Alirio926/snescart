/*
 * Led.c
 *
 * Created: 21/03/2018 00:18:21
 *  Author: aliri
 */ 

#include "led.h"

void Init(volatile uint8_t  *ddr,  volatile uint8_t  *port, uint8_t  dq)  {
	// save globals
	m_pddr = ddr;
	m_pport = port;
	m_dq = dq;
	// set for output
	*m_pddr |=(1<<m_dq);
}

void Set(uint8_t st) {
	m_ledstate = st;
	if (st)
	*m_pport |= (1<<m_dq);
	else
	*m_pport &= ~(1<<m_dq);
}


void Toggle() {
	Set(1 - m_ledstate);
}
