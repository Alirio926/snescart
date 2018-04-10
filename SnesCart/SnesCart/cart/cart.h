/*
 * cart.h
 *
 * Created: 20/03/2018 20:22:35
 *  Author: alirio
 */ 


#ifndef CART_H_
#define CART_H_

void loadIndexFile();
void listSdFiles();

void writeRom(char* rom, uint32_t size);
void checkCRC(char* rom, uint32_t size);

char* Identificar();

const char rom_0[] PROGMEM = "SMW.SMC";
const char rom_1[] PROGMEM = "TGEAR.SMC";
const char rom_2[] PROGMEM = "XMEN_H.SMC";
const char rom_3[] PROGMEM = "CASTLE.SMC";
const char rom_4[] PROGMEM = "MK2.SMC";
const char rom_5[] PROGMEM = "MK3.SMC";
const char rom_6[] PROGMEM = "UMK3.SMC";
const char rom_7[] PROGMEM = "ALADDIN.SMC";
const char rom_8[] PROGMEM = "BLACK.SMC";
const char rom_9[] PROGMEM = "SBOM.SMC";
const char rom_10[] PROGMEM = "DEMO.SMC";
const char rom_11[] PROGMEM = "SUNSET.SMC";
const char rom_12[] PROGMEM = "DKC3.SMC";
const char rom_13[] PROGMEM = "METALW.SMC";
const char rom_14[] PROGMEM = "CONTRA3.SMC";
const char rom_15[] PROGMEM = "F-ZERO.SMC";
const char rom_16[] PROGMEM = "FFURY2.SMC";
const char rom_17[] PROGMEM = "NGHOSTS.SMC";
const char rom_18[] PROGMEM = "SCASTLE4.SMC";
const char rom_19[] PROGMEM = "SMANA.SMC";
const char rom_20[] PROGMEM = "ZELDA.SMC";

PGM_P const rom_table[] PROGMEM =
{
	rom_0,
	rom_1,
	rom_2,
	rom_3,
	rom_4,
	rom_5,
	rom_6,
	rom_7,
	rom_8,
	rom_9,
	rom_10,
	rom_11,
	rom_12,
	rom_13,
	rom_14,
	rom_15,
	rom_16,
	rom_17,
	rom_18,
	rom_19,
	rom_20,
};

#endif /* CART_H_ */