#include "screen.h"
#include "joypad.h"

u8 screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH][3];

SDL_Thread *screen_thread;

static void key_unpressed(SDLKey key);
static void key_pressed(SDLKey key);

static void sdl_loop(void *param) {

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
	
	//temp
	exit(0);
	
	return 0;
}

void initialize_screen() {
	
	int i, j;
	for(i = 0; i < SCREEN_HEIGHT; ++i) {
		for(j = 0; j < SCREEN_WIDTH; ++j) {
			screen_buffer[i][j][0] = 0;
			screen_buffer[i][j][1] = 0;
			screen_buffer[i][j][2] = 0;
		}
	}
	
	screen_thread = SDL_CreateThread(sdl_loop, NULL);
	if(screen_thread == NULL) {
		fprintf(stderr, "Unable to create screen thread (%s)\n", SDL_GetError());
		fatal_error();
	}
	
}

void update_screen2() {
	int i, j;
	for(i = 0; i < SCREEN_HEIGHT; ++i) {
		for(j = 0; j < SCREEN_WIDTH; ++j) {
			u32 *pixel = (u32*)surface->pixels + ((i * SCREEN_WIDTH) + j);
			u8 r = screen_buffer[i][j][0];
			u8 g = screen_buffer[i][j][1];
			u8 b = screen_buffer[i][j][2];
			u32 color = SDL_MapRGB(surface->format, r*8, g*8, b*8);
			*pixel = color;
		}	
	}
	SDL_UpdateRect(surface, 0, 0, 0, 0); 
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