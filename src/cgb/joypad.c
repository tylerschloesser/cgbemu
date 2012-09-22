#include "joypad.h"
#include "memory.h"
#include "cpu.h"

bool joypad_pressed[8];

void initialize_joypad() {
    int i = 0;
    for( ; i < 8; ++i) {
        joypad_pressed[i] = false;
    }
}

void gb_joypad_down(int gb_key) {
    if(gb_key == JOYPAD_START) {
        dprintf("start pressed\n");
    }
    assert(gb_key < 8);
    joypad_pressed[gb_key] = true;
    cpu_interrupt(JOYPAD_INTERRUPT);
}

void gb_joypad_up(int gb_key) {
    assert(gb_key < 8); 
    joypad_pressed[gb_key] = false;
}

void gb_select_button_keys() {
    int i;
    for(i = 4; i < 8; ++i) {
        if(joypad_pressed[i]) {
            hardware_registers[P1] |= (1 << (i - 4));
        }
    }
}

void gb_select_direction_keys() {
    int i;
    for(i = 0; i < 4; ++i) {
        if(joypad_pressed[i]) {
            hardware_registers[P1] |= (1 << i);
        }
    }   
}