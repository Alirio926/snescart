# snescart flash
Hi, this is my 1º snes cart working full. the sch no too good, but work fine.
I do a menu in pvsneslib. so first o avr power on load a boot.sfc( 8bank x 32k ) to sram cart ( first 2MB ), in this time bus keep close.
after loaded avr open bus and is necessary reset snes. AVR keep bus open. A List menu with some roms to select.
When hit on name on list the hardware snes write on sram from cart donor mario paint some values and avr cacth this through 74165 and INT PE6( need update sch ). when MAD ative cs_sram this do too the PL from 74165. and avr got time to read serialy each byte( i tired read direct port avr but is too slow for this ). Ah this time the mad_cs reverses the 74245 direction so, 74165 can store right values.
AVR read value writed in sram and load selected rom, after finish need reset snes to start game. i dont find a way to start game without reset.
LOROM and HIROM is changed automatically fur avr.
When snes is writing a new game, bus is automatically close to no short data bus.
Full HI/LO SRAM snes cart from mario paint donor with list boot.

It's through the sdcard that records the roms in memory.

There will be other updates.

Thanks :)

http://www.portabledev.com/jeux/supernes/pvsneslib/ - alekmaul

https://github.com/aiju/snes-flash / http://aiju.de/electronics/SNES-cartridge - Aiju

http://amigaprj.blogspot.com.br/2013/09/snes-flash-cartridge.html - mc68k



Me e-mail: nyvevbsvyub926@lnubb.pbz.oe - ROT13
