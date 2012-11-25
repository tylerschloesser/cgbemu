#include "joypad.h"
#include "memory.h"
#include "cpu.h"

#include "../debug.h"

//bool joypad_pressed[8];

u8 joypad_state;

/* 1 = not pressed
 * 0 = pressed
 */

void initialize_joypad() {
    // set all to not pressed
    joypad_state = 0xFF;
    hardware_registers[P1] |= 0xF;
}


u8 get_joypad_state() {

    u8 joypad_select = hardware_registers[P1] & 0x30;
    
    if((joypad_select & 0x30) == 0) {
        // invalid setting (select both button and dir keys?)
        return (joypad_select | 0x0F);
    } else if((joypad_select & 0x10) == 0) {
        // direction keys
        return (joypad_select | ((joypad_state >> 0) & 0x0F));
    } else if((joypad_select & 0x20) == 0) {
        // button keys
        return (joypad_select | ((joypad_state >> 4) & 0x0F));
    }
    return 0xFF;
}


void gb_joypad_down(int gb_key) {
  
    joypad_state &= ~(1 << gb_key);
    
    hardware_registers[P1] |= (get_joypad_state() & 0x0F);
    
    //cpu_interrupt(JOYPAD_INTERRUPT);
}

void gb_joypad_up(int gb_key) {

    assert(gb_key < 8); 

    joypad_state |= (1 << gb_key);
     
    hardware_registers[P1] |= (get_joypad_state() & 0x0F);
}

/**
 * Set the joypad to recognize the joypad buttons (A, B, start, select)
 */
void gb_select_button_keys() {
    // clear last 4 bits
    //hardware_registers[P1] &= 0xF0;
    hardware_registers[P1] = ~0x20;
    // update the joypad register with the current state of the buttons
    hardware_registers[P1] |= ((joypad_state >> 4) & 0x0F);

}

/**
 * Set the joypad to recognize the joypad direction keys (down, up, left, right)
 */
void gb_select_direction_keys() {
    // clear the last 4 bits
    //hardware_registers[P1] &= 0xF0;
    hardware_registers[P1] = ~0x10;
    
    hardware_registers[P1] |= ((joypad_state >> 0) & 0x0F);  
}