/*
 * ExMemory.h
 *
 * Created: 01/02/2018 16:36:19
 *  Author: aliri
 */ 


#ifndef EXMEMORY_H_
#define EXMEMORY_H_



void XMEM_Init();
void XMEM_DeInit();

void setBank(uint8_t bank);

void WriteByte( uint32_t a, uint8_t b);
void WriteByteB( uint8_t bnk, uint32_t a, uint8_t b);

char ReadByte(uint32_t a);
char ReadByteB(uint8_t bnk, uint32_t a);

void programByte(uint32_t a, uint8_t b, uint8_t bnk);

void eraseChip();
void sectorErase(uint8_t s);

void AutoSelectMode();

void ResetMode();
	
void ReadManufacturerAndDeviceID();

void setBusState(uint8_t state, uint8_t mapper);

#define CS_DEC	PB5 // PINO 6(CS2 ativo em LOW) 74HC139
#define LE_EN	PB7
#define BUS_ENA	PG3 // PINO 18(BUS OE e HILOW ENABLE ativo em LOW) 74HC245 e 74HC00
#define MAPPER_MODE	PG4 // PINO 19(HIROM LOWROM) 74HC00

#define STARTADDR ((uint8_t *)0x8000)
uint8_t *addr;
uint8_t currentBank;
uint8_t mID, dID;
uint8_t xmem_init;
uint8_t bus_init;

#endif /* EXMEMORY_H_ */