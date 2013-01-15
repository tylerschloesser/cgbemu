#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <stdbool.h>

void gameboy_power_on();
void gameboy_power_off();

void gameboy_load_cartridge(char* cartridge_filepath);
void gameboy_load_bios(char *bios_filepath);

void gameboy_save_state( char* save_state_filepath );
void gameboy_load_state( char* save_state_filepath );

enum BUTTON {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_RIGHT,
	BUTTON_LEFT,
	BUTTON_START,
	BUTTON_SELECT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_INVALID,
};

void gameboy_toggle_button( enum BUTTON button, bool pressed );

#endif // GAMEBOY_H_