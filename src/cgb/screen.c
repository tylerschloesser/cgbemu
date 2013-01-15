#include "screen.h"
#include "joypad.h"

#include <assert.h>

u8 screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH][3];

static bool screen_initialized = false;

void initialize_screen() {
	
	assert( screen_initialized == false );
	
	fprintf( stdout, "initializing screen\n" );
	
	int i, j;
	for(i = 0; i < SCREEN_HEIGHT; ++i) {
		for(j = 0; j < SCREEN_WIDTH; ++j) {
			screen_buffer[i][j][0] = 0;
			screen_buffer[i][j][1] = 0;
			screen_buffer[i][j][2] = 0;
		}
	}
	
	screen_initialized = true;
}

void reinitialize_screen()
{
	assert( screen_initialized == true );
	screen_initialized = false;
	initialize_screen();
}


void update_screen() {

	//assert(surface != NULL);
	
	//surface = SDL_SetVideoMode(160,144,32,SDL_SWSURFACE);
	//if(!surface) return;

	if(surface == NULL) {
	
		static bool getting_video_surface = false;
	
		if( getting_video_surface == false ) {
			fprintf( stdout, "getting video surface\n" );
		}		
		
		getting_video_surface = true;
		surface = SDL_GetVideoSurface();
				
		if(surface == NULL) {
			return;
		} else {
			fprintf( stdout, "got video surface\n" );
		}
	}
	
		
	u32* sdl_pixel = (u32*)surface->pixels;

	int i, j;
	for(i = 0; i < SCREEN_HEIGHT; ++i) {
		for(j = 0; j < SCREEN_WIDTH; ++j) {
		
			u8 r, g, b;
			r = screen_buffer[i][j][0];
			g = screen_buffer[i][j][1];
			b = screen_buffer[i][j][2];
			
			u32 sdl_color = SDL_MapRGB(surface->format, r*8, g*8, b*8);
			*(sdl_pixel++) = sdl_color;
		}	
	}
	
	SDL_UpdateRect(surface, 0, 0, 0, 0); 
	
//gtk_sdl_display_update ( GTK_SDL ( SDL ) ) ;
}

void update_screen_buffer( int row, int column, u8 r, u8 g, u8 b )
{
	assert( row >= 0 && row < SCREEN_HEIGHT );
	assert( column >= 0 && column < SCREEN_WIDTH );
	
	screen_buffer[row][column][0] = r;
	screen_buffer[row][column][1] = g;
	screen_buffer[row][column][2] = b;
}
