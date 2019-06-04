/*---------------------------------------------------------------------------------


	Flash Cart Boot Menu
	-- by Squall
	-- Using pvsneslib from alekmaul more at http://www.portabledev.com/jeux/supernes/pvsneslib/


---------------------------------------------------------------------------------*/
#include <snes.h>
#include <stdlib.h>

#include "soundbank.h"

extern char LandTiles, LandTiles_end;
extern char CloudTiles, CloudTiles_end;
extern char LandPalette,LandPalette_end, CloudPalette, CloudPalette_end;
extern char Maps, Maps_end;
extern char Mapsc, Mapsc_end;


extern char snesfont;
extern char __SOUNDBANK__;

#define HIROM 0
#define LOROM 1
#define LIST_SIZE 21

//const static unsigned int LOROM = 1;
//const static char HIROM = 0;
#define BANK_16 16
#define BANK_32 32
#define BANK_40 40
#define BANK_64 64
#define BANK_80 80
#define BANK_96 96
#define BANK_128 128

unsigned short bgcolor =0;
unsigned short pad0, tmp;
unsigned int x=0, c_idx=4, lc_idx=4;
unsigned int temp=0;
char cc;
char sz[16];

int def[21][2] = {
{LOROM, BANK_16},
{LOROM, BANK_16}, // 02
{HIROM, BANK_64}, // 03
{LOROM, BANK_64}, // 04
{HIROM, BANK_96}, // 05
{HIROM, BANK_128},// 05
{HIROM, BANK_128},// 06
{LOROM, BANK_40}, // 07
{LOROM, BANK_32}, // 08
{HIROM, BANK_16}, // 09
{LOROM, BANK_64}, // 10
{LOROM, BANK_32}, // 11
{HIROM, BANK_128},// 12
{LOROM, BANK_64}, // 13
{LOROM, BANK_32}, // 14
{LOROM, BANK_16}, // 15
{HIROM, BANK_80}, // 16
{LOROM, BANK_32}, // 17
{LOROM, BANK_32}, // 18
{HIROM, BANK_64}, // 19
{LOROM, BANK_32},
};

const static char *rom[21] = 
{
	"Super Mario Bros 3",
	"Top Gear 1", 
	"X-MEN Mutante Apocalipse",
	"Castlevania X",
	"Mortal Kombat II",	
	"Mortal Kombat 3",	
	"Ultimate Mortal Kombat 3",
	"Aladdin",			
	"Black Thorne",		
	"Super Bomberman",			
	"Demon's Crest",			
	"Sunset Riders",			
	"Donkey Kong Country 3",	
	"Metal Warriors",			
	"Contra 3 The Alien War",
	"F-Zero",					
	"Fatal Fury 2",			
	"Super Ghoul'sN Ghosts",	
	"Super Castlevania IV",	
	"Secret of Mana",			
	"The Legend of Zelda",
};

//---------------------------------------------------------------------------------
int main(void) {
    
	short scrX=0, scrY=0;
	unsigned short pad0, move;
	
	// Inicia engine de som
	spcBoot();

    // Inicia SNES 
	consoleInit(); 
	// Inicia console com minha fonte
	consoleInitText(0, 1, &snesfont);
	
	// seta sound bank
	spcSetBank(&__SOUNDBANK__);
	
	// aloca cerca de 10K de ram para som (39 256-byte blocks)
	spcAllocateSoundRegion(39);

	// carrega musica
	spcLoad(MOD_POLLEN8);
	
	// Copia tiles na VRAM
	bgInitTileSet(1, &LandTiles, &LandPalette, 1, (&LandTiles_end - &LandTiles), (&LandPalette_end - &LandPalette), BG_16COLORS, 0x1000);
	bgInitTileSet(2, &CloudTiles, &CloudPalette, 0, (&CloudTiles_end - &CloudTiles), (&CloudPalette_end - &CloudPalette),  BG_4COLORS, 0x2000);

	// Copia Map na VRAM
	bgInitMapSet(1, &Maps, (&Maps_end - &Maps),SC_32x32, 0x4000);
	bgInitMapSet(2, &Mapsc, (&Mapsc_end - &Mapsc),SC_32x32, 0x4400);

	// Now Put in 16 color mode and put cloud on top of screen
	setMode(BG_MODE1,BG3_MODE1_PRORITY_HIGH); 
	bgSetEnable(0);
	
	
	bgSetEnableSub(1);
	bgSetEnableSub(2);
	
	// enable Subscreen Color ADD/SUB and Color addition on BG1 and Backdrop color
	setColorEffect(CM_SUBBGOBJ_ENABLE, CM_MSCR_BACK | CM_MSCR_BG1);
			
	// Title
	consoleDrawText(12,1,"FLASH CART");
	consoleDrawText( 2,4,">");
	for(x=4; x<LIST_SIZE+4;x++){
		consoleDrawText( 3,x,"%s",rom[x-4]);
	}
	consoleDrawText(21,25,"-by Squall");
	// Wait for nothing :P
	setScreenOn();  
	
	// Play file from the beginning
	spcPlay(0);
	
	setBrightness(0xF);  
	
	while(1) {
		// Update music / sfx stream and wait vbl
		spcProcess();
		
		pad0 = padsCurrent(0);
		switch (pad0) { 
			case KEY_UP:	
				if(c_idx>4){
					lc_idx=c_idx;					
					c_idx=c_idx-1;					
				}				
				break;
			case KEY_DOWN	:
				if(c_idx<LIST_SIZE+3){
					lc_idx=c_idx;					
					c_idx=c_idx+1;					
				}
				break;
			case KEY_A	: 
				temp = c_idx-4;
				
				consoleCopySram((u8 *) &temp, 1);
				WaitForVBlank();
				WaitForVBlank();
				
				consoleCopySram((u8 *) &def[c_idx-4][0], 1);
				WaitForVBlank();
				WaitForVBlank();
				
				consoleCopySram((u8 *) &def[c_idx-4][1], 1);
				WaitForVBlank();
				WaitForVBlank();
				
				break; 
			case KEY_B		:  
				consoleLoadSram((u8 *) &temp, 1);
				sprintf(sz,"%04X READ",temp);
				consoleDrawText(1,25,sz);
				break;
			default:
				sprintf(sz,"%04X READ",c_idx);
				consoleDrawText(1,25,sz);
				break; 
		}
		if(c_idx != lc_idx){
			consoleDrawText(2,c_idx,">");			
			consoleDrawText(2,lc_idx," ");	
			lc_idx = c_idx;
		}
		// change scrolling 
	    scrX++; 
		bgSetScroll(2, scrX,0);
				
		WaitForVBlank();
		WaitForVBlank();
		WaitForVBlank();
		WaitForVBlank();			
		WaitForVBlank();	
		
	}
	return 0;
}
