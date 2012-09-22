#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "../globals.h"
#include "../debug.h"
#include "memory.h"
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

#define ZF 0x80
#define NF 0x40
#define HF 0x20
#define CF 0x10

extern z80reg PC, SP, BC, DE, HL, AF, IR, TR;

extern int timer_counter;// = 1024; //clockspeed / frequency
//this is global because the write_memory function may need to
//modify this value

void cpu_run();
void cpu_start();
void cpu_stop();
int cpu_execute();

enum {
    VBLANK_INTERRUPT,
    LCD_STAT_INTERRUPT,
    TIMER_INTERRUPT,
    SERIAL_INTERRUPT,
    JOYPAD_INTERRUPT
};

#endif // CPU_H_INCLUDED
