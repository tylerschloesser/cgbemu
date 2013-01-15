#ifndef SCREEN_H_
#define SCREEN_H_

#include "graphics.h"

//temp
#include "../gui/gtksdl.h"

extern u8 screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH][3];

extern SDL_Surface *surface;

void update_screen();

void initialize_screen();
void reinitialize_screen();

void update_screen_buffer( int row, int column, u8 r, u8 g, u8 b );

#endif // SCREEN_H_