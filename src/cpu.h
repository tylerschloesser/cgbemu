#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "globals.h"
#include "memory.h"
#include "debug.h"
#include "graphics.h"


extern bool cgb_mode;

typedef union
{
    u16 W; //word
    struct
    {
        u8 L, H; //low, high
    } B; //byte
} z80reg;

z80reg PC, SP, BC, DE, HL, AF, IR, TR;

extern int timer_counter;// = 1024; //clockspeed / frequency
//this is global because the write_memory function may need to
//modify this value

void initialize_cpu();

#endif // CPU_H_INCLUDED
