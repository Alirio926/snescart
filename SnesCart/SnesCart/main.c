/*
 * FlashProg.cpp
 *
 * Created: 30/01/2018 22:40:54
 * Author : aliri
 */ 

#define F_CPU 8000000UL
#define baud 9600
#define _NOP() do { __asm__ __volatile__ ("nop\n\t"); } while (0)
	
#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "stdio.h"
#include "Macros.h"
#include "Serial/usart.h"
#include "ExMemory.h"
#include "mmc/mmc.h"
#include "fat16/fat.h"
#include "cart/cart.h"
#include "l74hc165/l74hc165.h"

int usart_putchar_printf(char var, FILE *stream);
void bin_prnt_byte(int x);
void AutoSelectMode();
void readFlash();
void readTitle();
void getIncomingChars();
void initInterrupt();

#define COMMANDSIZE 32
char cmdbuf[COMMANDSIZE];
uint8_t pos=0;
volatile uint8_t commandComplete = 0;

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);

uint8_t snes[3], cnt=0;
char buffer[12];

void initPins(){
	PIN_OUTPUT(G,3); //BUS_ENABLE
	PIN_OUTPUT(G,4); //HI/LO ROM
	
	DDRB |= _BV(7); // LATCH PB7 ativo em LOW( XMEN interface );
	
	PORTE |=  (1U << PE7); // RYBY FLASH
	
	PIN_OUTPUT(B, CS_DEC); // 74139 dos chips de memoria
}

int main(void)
{
	stdout = &mystdout;
	
	usart_init(115200,1);
	
	printf("Iniciando XMEN...\n");
	
	initPins();
	
	XMEM_Init();
			
	ResetMode();	
	
	printf("SD Card init...");
	uint8_t ret = mmc_init();
	
	if(ret != 0){
		printf(" fail\n");
	}else{
		printf(" done\n");
		fat_init();
		
	}
	printf("Gravando BOOT..\n");
	writeRom("boot.sfc", 262144);
	
	XMEM_DeInit();
	
	setBusState(ENABLE, LOROM);
	
	printf("Reset SNES\n");
	
	printf("init done.\nHabilitanto interruptions\n");
	
	initInterrupt();
	
	l74hc165_init();
	
	sei();
	
	while (1) 
    {		
		//printf("loop\n");
		if(commandComplete == 1){
			//printf("if in\n");
			strcpy_P(buffer, (PGM_P)pgm_read_word(&(rom_table[snes[0]])));
			printf("File %s, mapper: %02d, numBancos %03d\n",buffer,snes[1], snes[2]);
			commandComplete=0;
			if(xmem_init != 1){
				printf("Iniciando XMEN...\n");
				XMEM_Init();
				printf("init done\n");
			}
			
			if(snes[2] > 64){
				printf("Apagando flash ... ");
				eraseChip();	
				printf("concluido\n");			
			}
			
			uint32_t s = snes[2]*32768;
			writeRom(buffer, s);
			
			XMEM_DeInit();
			
			switch(snes[1]){
				case LOROM:
					setBusState(ENABLE, LOROM);
					cli();
					break;
				case HIROM:
					setBusState(ENABLE, HIROM);
					cli();
					break;
				default:
					printf("Mapper errado, fail\n");
					break;					
			}
			snes[0]=0;
			snes[1]=0;
			snes[2]=0;			
		}
		_delay_ms(500);
    }	
}

void readTitle(){
	uint8_t  c=1;
	uint16_t i=0x7FC0;
	
	setBank(0);
	printf("Antes bank (%d) ", currentBank); bin_prnt_byte(currentBank); printf("\n\n");
	printf("ADDRESS| 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
	printf("-------|------------------------------------------------\n");
	printf("0x%04X | ", i);
	for(; i < 0x7FD5; i++){
		
		printf("%c ",ReadByteB(64, i));
		if(c==0x10){
			printf("\n");
			printf("0x%04X | ", i);
			c=0;
		}
		c++;
	}
	
	printf("\n\n");
}
void readFlash(){
	uint8_t  c=1;
	uint16_t i=0;
	
	setBank(0);
	printf("Antes bank (%d) ", currentBank); bin_prnt_byte(currentBank); printf("\n\n");
	printf("ADDRESS| 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
	printf("-------|------------------------------------------------\n");
	printf("0x%04X | ", i);
	for(; i < 0XFF; i++){
		
		printf("%02X ",ReadByteB(0, i));
		if(c==0x10){
			printf("\n");
			printf("0x%04X | ", i);
			c=0;
		}
		c++;
	}
	
	printf("\n\n");
}
int usart_putchar_printf(char var, FILE *stream) {
	if (var == '\n') usart_write_char('\r');
	usart_write_char(var);
	return 0;
}

void bin_prnt_byte(int x)
{
	int n;
	for(n=0; n<8; n++)
	{
		if((x & 0x80) !=0)
		{
			usart_write_char('1');
		}
		else
		{
			usart_write_char('0');
		}
		if (n==3)
		{
			usart_write_char(' '); /* insert a space between nybbles */
		}
		x = x<<1;
	}
}

void getIncomingChars(){
	for(uint8_t i=0; i< COMMANDSIZE;i++) cmdbuf[i] = 0;
	//initialize variables
	char c = ' ';
	int idx = 0;
	//now read serial data until linebreak or buffer is full
	do {
		c = uart_getc();
		cmdbuf[idx++] = c;
	}
	while (c != '\n' && idx < (COMMANDSIZE)); //save the last '\0' for string end
	//change last newline to '\0' termination
	cmdbuf[idx - 1] = 0;
	commandComplete = 1;
	
}
void processCommand(){
	
}

void initInterrupt(){
	// Change pin 6 on bus E to an input by changing bit 6 to zero
	DDRE &= ~(1 << PIN6);
	
	// Defining a pull-up resistor to to pin 6 on bus E
	// to prevent input floating
	PORTE |= (1 << PIN6);
	
	// Set the interrupt mode to logical change for interrupt 6
	// in the external interrupt configuration register
	EICRB = (1 << ISC61);
	
	// Allow external interrupt 7
	EIMSK |= (1 << INT6);
	
}
// The code of the external interrupt
ISR(INT6_vect)
{
	cli();
		
	uint8_t b = l74hc165_shiftin();
	snes[cnt++] = b;
	printf("Read %d \n",b);
	if(cnt>=3){
		printf("Complete %d, %d\n",cnt,commandComplete);
		cnt=0;
		commandComplete = 1;
	}
	sei();
}
void listFiless(){
	unsigned char Buffer[512];
	unsigned int Clustervar;
	unsigned char Dir_Attrib = 0;
	unsigned long Size = 0;
	printf("\r\nDirectory\r\n");
	for (char a = 1;a < 240;a++)
	{
		Clustervar = fat_read_dir_ent(0,a,&Size,&Dir_Attrib,Buffer);
		if (Clustervar == 0xffff)
		{
			break;
		}
		printf("Cluster = %4x DirA = %2x Size = %07lu FileName = ",Clustervar,Dir_Attrib, Size);
		printf("%s",Buffer);
		printf("\r\n");
	}
	usart_write("\r\nDirectory Ende\r\n");
}
void oldloop(){
	//printf("Read %c\n", uart_getc());
		//data = uart_getc();
		/*getIncomingChars();
		if(commandComplete == 1){
			printf("Recebido: %s\n",cmdbuf);
			commandComplete = 0;	
		}*/
		/*if (data=='I'){
			AutoSelectMode();
			ReadManufacturerAndDeviceID();
			printf("Manufacturer Code: 0x%02X\n",mID);
			printf("Am29F016 Device:   0x%02X\n\n",dID);
			
			ResetMode();
		}
		else if (data=='E'){
			printf("Erasing..");
			eraseChip();
			printf(" Done\n");
		}	
		else if(data=='W'){
			writeRom("SMW.SMC", 524288);
		}
		else if(data=='Q'){
			writeRom("XMEN_H.SMC", 2097152);
			
			printf("\nXMEM_DeInit\n");
			XMEM_DeInit();
			printf("LOROM\n");
			write(PORTG, MAPPER_MODE, LOROM);
			printf("BUS ENABLE\n");
			write(PORTG, BUS_ENA, ENABLE);
			// Allow global interrupts
			sei();
		}
		else if(data=='w'){
			writeRom("boot.sfc", 262144);
		}
		else if(data=='T'){
			readTitle();
		}
		else if(data=='R'){
			readFlash();
		}
		else if(data=='l'){
			printf("LOROM\n");
			write(PORTG, MAPPER_MODE, LOROM);
		}
		else if(data=='h'){
			printf("HIROM\n");
			write(PORTG, MAPPER_MODE, HIROM);
		}
		else if(data=='e'){
			
				printf("\nXMEM_DeInit\n");
				XMEM_DeInit();
				printf("LOROM\n");
				write(PORTG, MAPPER_MODE, LOROM);
				printf("BUS ENABLE\n");
				write(PORTG, BUS_ENA, ENABLE);
				// Allow global interrupts
				sei();
		}
		else if(data=='d'){
			
				printf("\nBUS DISABLE\n");
				write(PORTG, BUS_ENA, DISABLE);
				printf("XMEM_Init\n");
				XMEM_Init();
				// Disable global interrupts
				cli();
		}*/
		/*
		else
			if(data != 0)
				printf("Read %c\n",data);
		data=0;*/
}