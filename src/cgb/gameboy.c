#include "gameboy.h"
#include "cpu.h"
#include "joypad.h"

static void key_pressed(SDLKey key);
static void key_unpressed(SDLKey key);

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
	
	gameboy_screen_on();
	
	// TEMP FIX
	// we need to wait for the surface to be created right here
	Sleep(1000);
	
    initialize_joypad();
    
	// start with the bios
	PC.W = 0;
	hardware_registers[BLCK] = 0;
	
	cpu_start();
	
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
