#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#include "globals.h"
#include "memory.h"
#include "cpu.h"

void display_cpu_values() ;

extern char opcode[][16];
extern char opcodeCB[][16];

extern bool show_opcodes;

#define BIT_0 1
#define BIT_1 2
#define BIT_2 4
#define BIT_3 8
#define BIT_4 10
#define BIT_5 20
#define BIT_6 40
#define BIT_7 80

#endif //DEBUG_H_INCLUDED