#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include <stdbool.h>

#include "globals.h"
#include "cgb/memory.h"
#include "cgb/cpu.h"

#define DEBUG_MBC_READ 0
#define DEBUG_MBC_WRITE 1

extern bool debug_console_opened;

void display_cpu_values() ;

void open_debug_console();
void close_debug_console();

#ifdef _WIN32
void dprintf( const char* format, ... );
#endif

extern char opcode[][16];
extern char opcodeCB[][16];

#endif //DEBUG_H_INCLUDED
