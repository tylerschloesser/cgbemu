#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#ifdef _WIN32

#include <windows.h>

#endif

#include <fcntl.h>
#include <errno.h>

#include "../globals.h"
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include "memory.h"

extern SDL_Surface *surface;

int render_screen();
//int render_background();
int setup_GUI();



#endif //GRAPHICS_H_INCLUDED