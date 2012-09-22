#include "gameboy.h"
#include "cpu.h"

void gameboy_power_on() {
	
	// start with the bios
	PC.W = 0;
	hardware_registers[BLCK] = 0;
	
	start_cpu();
	
}