A M68k Multi-Board Computer design. The design basics is as follows:
1) A backplane PCB contains the majority of the bus, such as the address and data lines.
2) Cards slot into the backplane. Cards other then the CPU card should have bus buffers to lower the bus load. (I might give the CPU card buffers too).
3) The 68k's 16MB memory map is split into 16 1MB regions. This is because the two memory chips used, the sst39f040 flash and AS6C4008 ram are both 0.5MB. (So when you put two of them together to get 16 bits, you end up with 1MB per bank).
4) Address decoding is done by a pair of decoders on the CPU card. The CS lines from a header on the side of the CPU card to headers on the sides of the other cards, ideally with a twisted ground for each CS line. In the future these lines could be moved into the backplane. 
5) If the card is less then 10x10 cm, I make it 4-layer. 4-layer boards bigger then 10x10cm are too expensive. 
6) the connectors are DIN41612
7) Line the 16 CS lines, the 7 interrupt lines are also jumpers you plug into the side of the board. There are also 8 interrupt enable jumpers that need to go from the IO card to the CPU card if you want to use one of the parallel output ports as an interrupt enable status port. 

IO and CPU cards are currently completely untested, and the memory card is missing a chip to enable the card's data transmitters. 

/Schematic/:
The actual MBC schematics
Note: LS components are sometimes used when the library has no corresponding HC part. All parts are meant to be HC, with the exception of LVC in 5v->3v translation (output to SD card) and HCT in 3v->5v translation (input from SD card). You can use ACT (or AC if you are feeling brave) but at <16Mhz there should not be a need for fast ACT/AC buffers/gluelogic, especially due to the 68k's slow bus cycles (in comparison to say a 6502 or a z80).

Schematics/BP - design for the backplane (TESTED)
Schematics/CPU - design for CPU card. (NOT TESTED)
Schematics/IO - design for IO card with SD card, PIO, PS2, RTC. (NOT TESTED)
Schematics/MEMv2 - second version of the memory card, doesn't even have the layout done. (NOT FINISHED)
Schematics/MEMv1 - first version of the memory card, missing 74HC20 and gate to enable the card bus buffers (TESTED)
Schematics/Footpp - various footprints
Schematics/Lib - various symbols

/Code/
WIP code, just testing stuff, will eventually be OS

/sst39f040_inBB/:
Allows you to connect an arduino mega to the backplane and program the flash chips on the memory card in-circuit. 
TODO: document this better
sst39f040_inBB/avr - the arduino mega code
sst39f040_inBB/send.py - script file to send the data, requires serial lib