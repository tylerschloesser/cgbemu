#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "../globals.h"
#include "../debug.h"
#include "memory.h"
#include "graphics.h"


extern bool cgb_mode;

typedef union
{
    u16 W; // 16 bit word
    struct
    {
        u8 L, H; //low, high
    } B; //byte
} z80reg;

#define ZF 0x80
#define NF 0x40
#define HF 0x20
#define CF 0x10

#define REG_B BC.B.H
#define REG_C BC.B.L
#define REG_D DE.B.H
#define REG_E DE.B.L
#define REG_H HL.B.H
#define REG_L HL.B.L
#define REG_A AF.B.H
#define REG_F AF.B.L

#define HI B.H
#define LO B.L

#define READ(S) MBC_read(S)
#define WRITE(D, B) MBC_write(D, B)
#define CLOCK_CYCLES(x) return(x)

#define RES(B,R) R&=~(1<<B)
#define SET(B,R) R|=(1<<B)
#define BIT(B,R) REG_F=(R&(1<<B)?0:ZF)|HF|(REG_F&CF)

#define RLC(R) R=(R<<1)|((R&0x80)>>7);REG_F=(R?0:ZF)|(R&1?CF:0)
#define RRC(R) R=(R>>1)|((R&1)<<7);REG_F=(R?0:ZF)|(R&0x80?CF:0)
#define RL(R) TR.LO=R;R=(R<<1)|((REG_F&CF)>>4);REG_F=(R?0:ZF)|(TR.LO&0x80?CF:0)
#define RR(R) TR.LO=R; R=(R>>1)|((REG_F&CF)<<3);REG_F=(R?0:ZF)|(TR.LO&1?CF:0)

#define SLA(R) TR.LO=R;R=(R<<1);REG_F=(R?0:ZF)|(TR.LO&0x80?CF:0)
#define SRA(R) TR.LO=R;R=(R>>1)|(R&0x80);REG_F=(R?0:ZF)|(TR.LO&1?CF:0)
#define SWAP(R) R=(R<<4)|(R>>4);REG_F=(R?0:ZF)
#define SRL(R) TR.LO=R;R=(R>>1);REG_F=(R?0:ZF)|(TR.LO&1?CF:0)

#define ADD_BYTES(R1,R2) TR.LO=R1+R2; REG_F=(TR.LO?0:ZF)|(((R1^R2^TR.LO)&0x10)?HF:0)|(TR.LO<R2?CF:0);R1=TR.LO
#define ADC_BYTES(R1,R2) TR.LO=R1+R2+((REG_F&CF)>>4);REG_F=(TR.LO?0:ZF)|(((R1^R2^TR.LO)&0x10)?HF:0)|(TR.LO<R2?CF:0);R1=TR.LO

#define SUB_BYTES(R1,R2) REG_F=((R1==R2)?ZF:0)|NF|(((R1&0xF)<(R2&0xF))?HF:0)|((R1<R2)?CF:0);R1-= R2
#define SBC_BYTES(R1,R2) TR.HI=R2-((REG_F&CF)>>4);REG_F=((R1==TR.HI)?ZF:0)|NF|(((R1&0xF)<(TR.HI&0xF))?HF:0)|((R1<TR.HI)?CF:0);R1-=TR.HI

#define AND_BYTE(R) REG_A&=R;REG_F=(REG_A?0:ZF)|HF
#define XOR_BYTE(R) REG_A^=R;REG_F=(REG_A?0:ZF)
#define OR_BYTE(R) REG_A|=R;REG_F=(REG_A?0:ZF)
#define CP_BYTES(R1,R2) REG_F=((R1==R2)?ZF:0)|NF|(((R1&0xF)<(R2&0xF))?HF:0)|((R1<R2)?CF:0)

#define LD_BYTE(R1, R2) R1 = R2

#define INVALID_OPCODE(X) 	printf("Invalid opcode (%X)!\n", X);display_cpu_values();getchar();exit(1)

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
