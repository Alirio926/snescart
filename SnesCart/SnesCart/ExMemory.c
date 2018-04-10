/*
 * ExMemory.c
 *
 * Created: 01/02/2018 16:36:05
 *  Author: alirio
 */ 
#define F_CPU 8000000UL

#include <stdint.h>
#include <avr/io.h>
#include "stdio.h"
#include <util/delay.h>
#include "ExMemory.h"
#include "Macros.h"


void XMEM_Init(){	
	
	setBusState(DISABLE, LOROM); // disable bus em lorom
			
	MCUCR = (1<<SRE)|(1<<SRW10);
	XMCRA = (1<<SRL2)|(0<<SRL1)|(0<<SRL0)|(0<<SRW01)|(0<<SRW00)|(1<<SRW11);
	XMCRB = (0<<XMBK)|(0<<XMM2)|(0<<XMM1)|(1<<XMM0);
	// set port pins control
	DDRC |= _BV(7);
	write(PORTC, PC7, LOW); // Select bank 0 (A15)	
	
	write(PORTB, LE_EN, LOW); // Enable LOW, Latch PB7
	
	DDRF  = 0b00111111; // A16 to A21	
	PORTF = 0b00000000;
				
	currentBank = 200;
	
	write(PORTB, CS_DEC, LOW);	// Enable CS
	
	xmem_init = 1;
}
void XMEM_DeInit(){
	XMCRB = 0x00;
	XMCRA = 0x00;
	
	write(PORTB, PB7, HIGH); // Disable HIGH, Latch PB7
	DDRF  = 0b00000000; // A16 to A21
	PORTF = 0b00000000; // no pull
	DDRC  = 0b00000000; // A0 to A15
	PORTC = 0b00000000; // no pull
	DDRA  = 0b00000000; // AD0 to AD7
	PORTA = 0b00000000; // no pull
	
	PIN_OUTPUT(G, PG0); // WR
	write(PORTG, PG0, HIGH);// disable Write
	PIN_INPUT(G, PG1);  // OE
	
	currentBank = 200;
	
	MCUCR &= ~(1<<SRE);
	
	xmem_init = 0;
}

void setBusState(uint8_t state, uint8_t mapper){
	switch(mapper){
		case LOROM:
			printf("LOROM\n");
			write(PORTG, MAPPER_MODE, LOROM);
			break;
		case HIROM:
			printf("HIROM\n");
			write(PORTG, MAPPER_MODE, HIROM);
			break;
	}
	switch(state){
		case ENABLE:
			printf("BUS ENABLE\n");
			write(PORTG, BUS_ENA, ENABLE);
			break;
		case DISABLE:
			printf("BUS DISABLE\n");
			write(PORTG, BUS_ENA, DISABLE);
			break;
	}
}
void setBank(uint8_t bank){	
	if(currentBank != bank){ 	
		if(bank & 1) write(PORTC, PC7, HIGH);
		else		 write(PORTC, PC7, LOW);
	
		/* Entende-se como bank bit 0 = A15, e os demais 16, 17, 18, 19, 20 e 21
		* então apos testar o primeiro bit no if acima, na proxima instrução
		* eu desloco para remover esse primeiro bit e testo 111111 (0x3F) 
		* para setar na porta de endereçamento como bank swap pinos PF0..PF5
		* para essa configuração temos 64 bancos de 32768 num total de 2MB(2.097.152)
		*/
		PORTF = ((bank >> 1) & 0x3F);	
		currentBank = bank;
	}
}

void setAddress(uint32_t a){
	addr = STARTADDR + a;
}
void setAddressB(uint8_t bnk, uint32_t a){
	setBank(bnk);
	setAddress(a);
}

void WriteByte(uint32_t a, uint8_t b){
	(*(uint8_t *)(STARTADDR + a) = b);
	if(currentBank > 63)
		while(READ(E, 7) == LOW){}
	//_delay_us(0.01);
	
}
void WriteByteB( uint8_t bnk, uint32_t a, uint8_t b){
	setBank(bnk);
	WriteByte(a,b);
}

char ReadByte(uint32_t a){
	return (*(uint8_t *)(STARTADDR + a));
}
char ReadByteB(uint8_t bnk, uint32_t a){
	setBank(bnk);
	return ReadByte(a);
}

void programByte(uint32_t a, uint8_t b, uint8_t bnk){
	if(bnk < 64){ // se menor que 2MB, grava na SRAM, se maior grava na flash
		setBank(bnk);
		WriteByte(a, b);
	}else{
		setBank(64);
		WriteByte(0x5555, 0xaa);
		WriteByte(0x2aaa, 0x55);
		WriteByte(0x5555, 0xA0);
		setBank(bnk);
		WriteByte(a, b);
	}
}

void eraseChip(){	
	setBank(64);
	WriteByte(0x5555, 0xaa);
	WriteByte(0x2aaa, 0x55);
	WriteByte(0x5555, 0x80);
	WriteByte(0x5555, 0xaa);
	WriteByte(0x2aaa, 0x55);
	WriteByte(0x5555, 0x10);
	while(READ(E, 7) == LOW){
		_delay_ms(200);
	}
}
void sectorErase(uint8_t s){
	setBank(64);
	WriteByte(0x5555, 0xaa);
	WriteByte(0x2aaa, 0x55);
	WriteByte(0x5555, 0x80);
	WriteByte(0x5555, 0xaa);
	WriteByte(0x2aaa, 0x55);
	WriteByte(s, 0x30);
	while(READ(E, 7) == LOW){
		_delay_ms(200);
	}
}

void AutoSelectMode(){
	setBank(64);
	WriteByte(0x5555, 0xaa);
	WriteByte(0x2aaa, 0x55);
	WriteByte(0x5555, 0x90);
}
void ResetMode(){
	setBank(64);
	WriteByte(0x5555, 0xaa);
	WriteByte(0x2aaa, 0x55);
	WriteByte(0x5555, 0xF0);
}
void ReadManufacturerAndDeviceID(){
	mID = ReadByte(0x0);
	dID = ReadByte(0x1);
}