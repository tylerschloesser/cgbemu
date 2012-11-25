#include "gameboy.h"
#include "cpu.h"
#include "joypad.h"
#include "memory.h"
#include "../tools.h"

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

SDL_Thread *screen_thread;
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

static int gameboy_screen(void *param) {

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Unable to initialize SDL (%s)\n", SDL_GetError());
		fatal_error();
	}
	
	if((surface = SDL_SetVideoMode(160, 144, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		fprintf(stderr, "Unable to setup SDL surface (%s)\n", SDL_GetError());
		fatal_error();
	}
	
	SDL_WM_SetCaption("CGB", "");

	bool screen_on = true;
	while(screen_on == true) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
                {
					screen_on = false;
					break;
                }
                case SDL_KEYDOWN:  
                {
                    SDLKey key = event.key.keysym.sym;
                    key_pressed(key);
                    break;
                }
                case SDL_KEYUP:
                {
                    SDLKey key = event.key.keysym.sym;
                    key_unpressed(key);
                    break;
                }
			}
		}
	}
	
	return 0;
}

static void gameboy_screen_on() {

		
	screen_thread = SDL_CreateThread(gameboy_screen, NULL);
	if(screen_thread == NULL) {
		fprintf(stderr, "Unable to create screen thread (%s)\n", SDL_GetError());
		fatal_error();
	}
		
}

void gameboy_power_on() {

	initialize_memory();
	initialize_joypad();
	
	char game_filepath[] = "..\\resources\\roms\\pokemon_yellow.gbc";
	//char game_filepath[] = "..\\resources\\roms\\pokemon_red.gbc";
	char bios_filepath[] = "..\\resources\\bios\\gbc_bios.bin";
	
    gameboy_load_bios(bios_filepath);
    gameboy_load_cartridge(game_filepath);
     
	
	gameboy_screen_on();
	
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
	int i, j;
	printf("newly executed opcodes\n");
	int ex = 0;
	for(i = 0; i < 0xFF; ++i) {
		if(executed[0][i] == 1) {
			executed[0][i] = 2;
			printf("%s\n", opcode[i]);
			++ex;
		}
	}
	for(i = 0; i < 0xFF; ++i) {
		if(executed[1][i] == 1) {
			executed[1][i]= 2;
			printf("%s\n", opcodeCB[i]);
			++ex;
		}
	}
	printf("Total: %i\n", ex);
}

static void key_pressed(SDLKey key) {
    int gb_key = -1;

    switch(key) {
    case SDLK_UP:
        gb_key = JOYPAD_UP;
        break;
    case SDLK_DOWN:
        gb_key = JOYPAD_DOWN;
        break;
    case SDLK_RIGHT:
        gb_key = JOYPAD_RIGHT;
        break;
    case SDLK_LEFT:
        gb_key = JOYPAD_LEFT;
        break;
    case SDLK_RETURN:
        gb_key = JOYPAD_START;
        break;
    case SDLK_SPACE:
        gb_key = JOYPAD_SELECT;
        break;
    case SDLK_a:
        gb_key = JOYPAD_A;
        break;
    case SDLK_z:
        gb_key = JOYPAD_B;
        break;
	case SDLK_d:
		//debug
		cpu_debug();
    }
    
    if(gb_key != -1) {
        gb_joypad_down(gb_key);
    }
}

static void key_unpressed(SDLKey key) {
    int gb_key = -1;
    
    switch(key) {
    case SDLK_UP:
        gb_key = JOYPAD_UP;
        break;
    case SDLK_DOWN:
        gb_key = JOYPAD_DOWN;
        break;
    case SDLK_RIGHT:
        gb_key = JOYPAD_RIGHT;
        break;
    case SDLK_LEFT:
        gb_key = JOYPAD_LEFT;
        break;
    case SDLK_RETURN:
        gb_key = JOYPAD_START;
        break;
    case SDLK_SPACE:
        gb_key = JOYPAD_SELECT;
        break;
    case SDLK_a:
        gb_key = JOYPAD_A;
        break;
    case SDLK_z:
        gb_key = JOYPAD_B;
        break;
    }
    
    if(gb_key != -1) {
        gb_joypad_up(gb_key);
    }
}
