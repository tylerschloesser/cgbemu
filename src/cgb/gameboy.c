#include "gameboy.h"
#include "cpu.h"
#include "joypad.h"
#include "memory.h"
#include "../tools.h"

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>


SDL_Surface *surface;

static void key_pressed(SDLKey key);
static void key_unpressed(SDLKey key);


bool gameboy_verify_cartridge() {
	
	char title[0x10];
	title[0x10 - 1] = '\0';
	int i;
	for(i = 0x134; i < 0x144; ++i) {
		title[i - 0x134] = cartridge_rom[i];
	}
	printf("Title: %s\n", title);
	printf("CartridgeType: %X\n", cartridge_rom[0x147]);
	printf("Rom Size: %X\n", cartridge_rom[0x148]);
	
	// calculate checksum
	u16 sum;
	for(i = 0; i < CARTRIDGE_ROM_SIZE; ++i) {
		sum += cartridge_rom[i];
	}
	sum -= (cartridge_rom[0x14e] + cartridge_rom[0x14f]);
	
	printf("Sum: %X\n", sum);
	
	printf("checksum: %X %X\n", cartridge_rom[0x14e], cartridge_rom[0x14f]);
	
	return true;
}

void gameboy_load_cartridge(char* cartridge_filepath) {
	dprintf("Opening %s...", cartridge_filepath);
	int bytes_read = binary_read_file(cartridge_filepath, cartridge_rom, CARTRIDGE_ROM_SIZE);
	if(bytes_read == 0) {
		fprintf(stderr, "Failed (%s)\n", get_last_error());
		fatal_error();
	}
	
	gameboy_verify_cartridge();
	
	dprintf("Done (%s)\n", size_to_string(bytes_read));
}

void gameboy_load_bios(char *bios_filepath) {
	dprintf("Opening %s...", bios_filepath);
    int bytes_read = binary_read_file(bios_filepath, bios, BIOS_SIZE);
	if(bytes_read == 0) {
		fprintf(stderr, "Failed (%s)\n", get_last_error());
		fatal_error();
	}
    dprintf("Done (%s)\n", size_to_string(bytes_read));
}


void gameboy_power_on() {

	initialize_memory();
	initialize_joypad();
	initialize_screen();
	
	char game_filepath[] = "..\\resources\\roms\\pokemon_yellow.gbc";
	//char game_filepath[] = "..\\resources\\roms\\pokemon_red.gbc";
	char bios_filepath[] = "..\\resources\\bios\\gbc_bios.bin";
	
    gameboy_load_bios(bios_filepath);
    gameboy_load_cartridge(game_filepath);
    
	// TEMP FIX
	// we need to wait for the surface to be created right here
	Sleep(1000);
	
    
    
	// start with the bios
	PC.W = 0;
	hardware_registers[BLCK] = 0;
	
	cpu_start();
	
}

//temp for debugging
void cpu_debug() {
	//display_cpu_values();
	//show_opcodes = true;
}
