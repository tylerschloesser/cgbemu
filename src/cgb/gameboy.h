#ifndef GAMEBOY_H_
#define GAMEBOY_H_

#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>

SDL_Thread *screen_thread;
SDL_Surface *surface;

void gameboy_power_on();

#endif // GAMEBOY_H_