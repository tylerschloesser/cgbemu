#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "../globals.h"

typedef union
{
    u16 W;
    struct
    {
        u8 L, H;
    } B;
} Z80Register;

typedef struct {
	bool running;
	bool waiting_for_interrupt;
	
	s32 timer_counter;
	s32 scanline_counter;
	
	u32 cycles;
	u32 mode_cycles;
	u32 divider_counter;
	
	Z80Register PC;
	Z80Register SP;
	Z80Register BC;
	Z80Register DE;
	Z80Register HL;
	Z80Register AF; 
	Z80Register IR;
	
} CpuState;

/* 
 * "counter" is intentionally misspelled until the temporary
 *  cpu state definitions are removed
 */
void cpu_set_timer_countr( int timer_countr );

CpuState get_cpu_state();
void set_cpu_state( CpuState cpu_state );

void reinitialize_cpu();
void initialize_cpu();

void cpu_emulate();

void cpu_start();
void cpu_stop();
void cpu_pause();

#endif /* CPU_H_INCLUDED */
