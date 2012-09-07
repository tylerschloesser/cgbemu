#include "globals.h"
#include "memory.h"
#include "cpu.h"
#include "graphics.h"
#include "tools.h"

#include <stdlib.h>

int main(int argc, char** argv) {

	if(debug_mode) {
		setup_debug_console();
	}

	fprintf(stdout, "welcome to cgbemu - ");
	if(debug_mode) {
		fprintf(stdout, "debug mode is enabled\n");
	} else {
		fprintf(stdout, "debug mode is disabled\n");
	}

	char game_filepath[] = "..\\resources\\roms\\pokemon_yellow.gbc";
	char bios_filepath[] = "..\\resources\\bios\\gbc_bios.bin";
	
	setup_gui();

	dprintf("Opening %s...", bios_filepath);
	int bytes_read = binary_read_file(bios_filepath, bios, BIOS_SIZE);
	if(bytes_read == 0) {
		fprintf(stderr, "Failed (%s)\n", get_last_error());
		fatal_error();
	}
	dprintf("Done (%s)\n", size_to_string(bytes_read));
	
	dprintf("Opening %s...", game_filepath);
	bytes_read = binary_read_file(game_filepath, cartridge_rom, CARTRIDGE_ROM_SIZE);
	if(bytes_read == 0) {
		fprintf(stderr, "Failed (%s)\n", get_last_error());
		fatal_error();
	}
	dprintf("Done (%s)\n", size_to_string(bytes_read));
		
	printf("Press enter to begin emulation...\n");
	getchar();

    initialize_cpu();

    return 0;
}
