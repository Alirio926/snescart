/*
 * cart.c
 *
 * Created: 20/03/2018 20:22:42
 *  Author: aliri
 */ 
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "../ExMemory.h"
#include "stdio.h"
#include "../mmc/mmc.h"
#include "../fat16/fat.h"
#include "../crc32.h"
#include "../Macros.h"

unsigned int Clustervar;
unsigned char Dir_Attrib = 0;
unsigned long Size = 0;
unsigned char Buffer[512];

void bin_prnt_bytee(int x);

// writeROM var
#define CLUSTER_SIZE 512
#define NUM_CLUSTER_POR_BANK 64
#define MAX_MEMORY_ADDRESS 0x8000

uint32_t flashCRC32 = 0xFFFFFFFF;
uint32_t romCRC32 = 0xFFFFFFFF; 

void writeRom(char* rom, uint32_t size){
	
	// variaveis do processo
	uint8_t bnk_cnt		=0;
	uint16_t buffer_cnt	=0;
	uint16_t n_cluster	= size / CLUSTER_SIZE;
	uint16_t cluster_cnt	=0;
	uint16_t mem_cnt	=0;
	uint32_t addr_cnt	=0;
	uint8_t nb = size / 32768;
	// variavel do sistema FAT
	Clustervar = 0;	
	
	// variavel do CRC32
	romCRC32	= 0xFFFFFFFF;
	flashCRC32  = 0xFFFFFFFF;
	
	if (fat_search_file((unsigned char *)rom, &Clustervar, &Size, &Dir_Attrib, Buffer) == 1)
	{
		printf("Processo de gravação iniciado.\n");
		printf("Gravando Bank: %d de %d\n", bnk_cnt, nb);
		for(cluster_cnt	=0; cluster_cnt < n_cluster; cluster_cnt++)
		{
			fat_read_file(Clustervar, Buffer, cluster_cnt);
			for(buffer_cnt=0; buffer_cnt < 512; buffer_cnt++){
				addr_cnt++;
				
				programByte(mem_cnt, Buffer[buffer_cnt], bnk_cnt);					
				romCRC32 = updateCRC32(Buffer[buffer_cnt], romCRC32);	
				if(mem_cnt == (MAX_MEMORY_ADDRESS - 1))		
				{
					mem_cnt=0;
					bnk_cnt = addr_cnt / MAX_MEMORY_ADDRESS;
					printf("Gravando Bank: %d de %d ( ", bnk_cnt, nb);
					bin_prnt_bytee(bnk_cnt);
					printf(" )\n");
				}else { mem_cnt++; }
			}// end for buffer
		}// end for cluster
	}// end IF fat_search_file
	
	printf("Gravados %lu\n", (unsigned long)addr_cnt);	
	/*
	printf("Init CRC 32 check.\n");
	
	flashCRC32  = 0xFFFFFFFF;
	
	for(uint8_t banco=0; banco < bnk_cnt; banco++)	
		for(mem_cnt=0; mem_cnt < MAX_MEMORY_ADDRESS; mem_cnt++)		
			flashCRC32 = updateCRC32(ReadByteB(banco, mem_cnt), flashCRC32);
		
	printf("CRC ROM FILE: 0x%08lX\n", ~romCRC32);
	printf("CRC FLASH   : 0x%08lX\n", ~flashCRC32);
	*/
}
void bin_prnt_bytee(int x)
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
