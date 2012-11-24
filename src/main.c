#include "globals.h"
#include "cgb/gameboy.h"
#include "tools.h"

#include <stdlib.h>

int main(int argc, char** argv) {

	if(debug_mode) {
		enable_debug_console();
	}

	fprintf(stdout, "welcome to cgbemu - debug mode is %s\n", debug_mode ? "enabled" : "disabled");

	char game_filepath[] = "..\\resources\\roms\\pokemon_yellow.gbc";
    //char game_filepath[] = "..\\resources\\roms\\pokemon_red.gb";
	char bios_filepath[] = "..\\resources\\bios\\gbc_bios.bin";
	
    gameboy_load_bios(bios_filepath);
    gameboy_load_cartridge(game_filepath);
     
	gameboy_power_on();
	
    return 0;
}