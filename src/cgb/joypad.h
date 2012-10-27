#include "../globals.h"

enum {
    JOYPAD_RIGHT = 0,
    JOYPAD_LEFT,
    JOYPAD_UP,
    JOYPAD_DOWN,
    
    JOYPAD_A,
    JOYPAD_B,
    JOYPAD_SELECT,
    JOYPAD_START
};


void gb_joypad_down(int gb_key);
void gb_joypad_up(int gb_key);

void gb_select_button_keys();
void gb_select_direction_keys();

u8 get_joypad_state();