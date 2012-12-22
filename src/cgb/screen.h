#ifndef SCREEN_H_
#define SCREEN_H_

#include "graphics.h"

extern u8 screen_buffer[SCREEN_HEIGHT][SCREEN_WIDTH][3];
void update_screen2();
void initialize_screen();

#endif // SCREEN_H_