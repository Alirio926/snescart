/*
 * Macros.h
 *
 * Created: 14/7/2012 22:05:02
 *  Author: Administrador
 */ 


#ifndef MACROS_H_
#define MACROS_H_

#define DISABLE  1
#define ENABLE 0
#define LOROM  1
#define HIROM 0

#define READING  0
#define WRITING 1
#define INPUT  0
#define OUTPUT 1
#define LOW 0
#define HIGH 1
#define bitSet(p,n) ((p) &= ~((1) << (n)))
#define bitClear(p,n) ((p) &= (~(1) << (n)))
#define bitGet(p,m) ((p) & (m))

#define write(port, pin, value) do { \
  if (value) \
    port |= 1 << pin; \
  else \
    port &= ~(1 << pin); \
} while (0)
#define GPIO_Tris(port, pin, direction) \
	(direction == INPUT ? port |=  (1U << pin) \
						  : port &= ~(1U << pin))                           

#define PIN_INPUT(port,pin) DDR ## port &= ~(1<<pin)
#define PIN_OUTPUT(port,pin) DDR ## port |= (1<<pin)
#define CLEAR(port,pin) PORT ## port &= ~(1<<pin)
#define SET(port,pin) PORT ## port |= (1<<pin)
#define TOGGLE(port,pin) PORT ## port ^= (1<<pin)
#define READ(port,pin) (PIN ## port & (1<<pin))
#endif /* MACROS_H_ */