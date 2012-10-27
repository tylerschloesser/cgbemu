#ifndef OPCODES_H_INCLUDED
#define OPCODES_H_INCLUDED

case 0x00: //NOP
	return 4;
case 0x01: //LD BC, d16
	BC.B.L = MBC_read(PC.W++);
	BC.B.H = MBC_read(PC.W++);
	return 12;
case 0x02: //LD (BC), A
	MBC_write(BC.W, AF.B.H);
	return 8;
case 0x03: //INC BC
	BC.W++;
	return 8; 
case 0x04: //INC B
	BC.B.H++;
	//AF.B.L = (BC.B.H ? 0 : ZF) | ((BC.B.H & 0xF) ? 0 : HF); 5/15/2012
	AF.B.L = (BC.B.H ? 0 : ZF) | ((BC.B.H & 0xF) ? 0 : HF) | (AF.B.L & CF);
	return 4;
case 0x05: //DEC B
	BC.B.H--;
	//AF.B.L = (BC.B.H ? 0 : ZF) | NF | ((BC.B.H & 0xF) == 0xF ? HF : 0); 5/15/2012
	AF.B.L = (BC.B.H ? 0 : ZF) | NF | ((BC.B.H & 0xF) == 0xF ? HF : 0) | (AF.B.L & CF);
	return 4;
case 0x06: //LD B, d8
	BC.B.H = MBC_read(PC.W++);
	return 8;
case 0x07: //RLCA
	AF.B.L = (AF.B.H & 0x80) ? CF : 0;
	AF.B.H = (AF.B.L & CF) ? ((AF.B.H << 1) | 1) : (AF.B.H << 1);
	return 4;
case 0x08: //LD (a16), SP
	TR.W = MBC_read(PC.W++);
	TR.W |= (MBC_read(PC.W++) << 8);
	MBC_write(TR.W++, SP.B.L);
	MBC_write(TR.W, SP.B.H);
	return 20;
case 0x09: //ADD HL, BC
	TR.W = HL.W + BC.W;
	AF.B.L = (AF.B.L & ZF) | (((HL.W ^ BC.W ^ TR.W) & 0x1000) ? HF : 0) | (TR.W < BC.W ? CF : 0);
	HL.W = TR.W;
	return 8;
case 0x0A: //LD A, (BC)
	AF.B.H = MBC_read(BC.W);
	return 8;
case 0x0B: //DEC BC
	BC.W--;
	return 8;
case 0x0C: //INC C
	BC.B.L++;
	//AF.B.L = (BC.B.L ? 0 : ZF) | ((BC.B.L & 0xF) ? 0 : HF); 5/15/2012
	AF.B.L = (BC.B.L ? 0 : ZF) | ((BC.B.L & 0xF) ? 0 : HF) | (AF.B.L & CF);
	return 4;
case 0x0D: //DEC C
	BC.B.L--;
	//AF.B.L = (BC.B.L ? 0 : ZF) | NF | ((BC.B.L & 0xF) == 0xF ? HF : 0); 5/15/2012
	AF.B.L = (BC.B.L ? 0 : ZF) | NF | ((BC.B.L & 0xF) == 0xF ? HF : 0) | (AF.B.L & CF);
	return 4;
case 0x0E: //LD C, d8
	BC.B.L = MBC_read(PC.W++);
	return 8;
case 0x0F: //RRCA
	AF.B.L = (AF.B.H & 1) ? CF : 0;
	AF.B.H = (AF.B.L & CF) ? ((AF.B.H >> 1) | 0x80) : (AF.B.H >> 1);
	return 4;
case 0x10: //STOP 0
	printf("STOP 0\n");
	getchar();
	exit(1);
case 0x11: //LD DE, d16
	DE.B.L = MBC_read(PC.W++);
	DE.B.H = MBC_read(PC.W++);
	return 12;
case 0x12: //LD (BC), A
	MBC_write(DE.W, AF.B.H);
	return 8;
case 0x13: //INC DE
	DE.W++;
	return 8;
case 0x14: //INC D
	DE.B.H++;
	AF.B.L = (DE.B.H ? 0 : ZF) | ((DE.B.H & 0xF) ? 0 : HF) | (AF.B.L & CF);
	return 4;
case 0x15: //DEC D
	DE.B.H--;
	AF.B.L = (DE.B.H ? 0 : ZF) | NF | ((DE.B.H & 0xF) == 0xF ? HF : 0) | (AF.B.L & CF);
	return 4;
case 0x16: //LD D, d8
	DE.B.H = MBC_read(PC.W++);
	return 8;
case 0x17: //RLA
	TR.B.L = (AF.B.L & CF);
	AF.B.L = (AF.B.H & 0x80) ? CF : 0;
	AF.B.H = (AF.B.H << 1) | (TR.B.L ? 1 : 0);
	return 4;
case 0x18: //JR r8
	PC.W += (signed char)MBC_read(PC.W) + 1;
	return 12;
case 0x19: //ADD HL, DE
	TR.W = HL.W + DE.W;
	AF.B.L = (AF.B.L & ZF) | (((HL.W ^ DE.W ^ TR.W) & 0x1000) ? HF : 0) | (TR.W < DE.W ? CF : 0);
	HL.W = TR.W;
	return 8;
case 0x1A: //LD A, (DE)
	AF.B.H = MBC_read(DE.W);
	return 8;
case 0x1B: //DEC DE
	DE.W--;
	return 8;
case 0x1C: //INC E
	DE.B.L++;
	AF.B.L = (DE.B.L ? 0 : ZF) | ((DE.B.L & 0xF) ? 0 : HF) | (AF.B.L & CF);
	return 4;
case 0x1D: //DEC E
	DE.B.L--;
	AF.B.L = (DE.B.L ? 0 : ZF) | NF | ((DE.B.L & 0xF) == 0xF ? HF : 0) | (AF.B.L & CF);
	return 4;
case 0x1E: //LD E, d8
	DE.B.L = MBC_read(PC.W++);
	return 8;
case 0x1F: //RRA
	TR.B.L = (AF.B.L & CF);
	AF.B.L = (AF.B.H & 1) ? CF : 0;
	AF.B.H = (AF.B.H >> 1) | (TR.B.L ? 0x80 : 0);
	return 4;
case 0x20: //JR NZ, r8
	if(AF.B.L & ZF) {
		PC.W++;
		return 8;
	} else {
		PC.W += (signed char)MBC_read(PC.W) + 1;
		return 12;
	}
case 0x21: //LD HL, d16
	HL.B.L = MBC_read(PC.W++);
	HL.B.H = MBC_read(PC.W++);
	return 12;
case 0x22: //LD(HL+), A
	MBC_write(HL.W++, AF.B.H);
	return 8;
case 0x23: //INC HL
	HL.W++;
	return 8;
case 0x24: //INC H
	HL.B.H++;
	AF.B.L = (HL.B.H ? 0 : ZF) | ((HL.B.H & 0xF) ? 0 : HF) | (AF.B.L & CF);
	return 4;
case 0x25: //DEC H
	HL.B.H--;
	AF.B.L = (HL.B.H ? 0 : ZF) | NF | ((HL.B.H & 0xF) == 0xF ? HF : 0) | (AF.B.L & CF);
	return 4;
case 0x26: //LD H, d8
	HL.B.H = MBC_read(PC.W++);
	return 8;
case 0x27: //DAA
	printf("DAA\n");
	getchar();
	exit(1);
case 0x28: //JR Z, r8
	if(AF.B.L & ZF) {
		PC.W += (signed char)MBC_read(PC.W) + 1;
		return 12;
	} else {
		PC.W++;
		return 8;
	}
case 0x29: //ADD HL, HL
	TR.W = HL.W + HL.W;
	AF.B.L = (AF.B.L & ZF) | (((HL.W ^ HL.W ^ TR.W) & 0x1000) ? HF : 0) | (TR.W < HL.W ? CF : 0);
	HL.W = TR.W;
	return 8;
case 0x2A: //LD A, (HL+)
	AF.B.H = MBC_read(HL.W++);
	return 8;
case 0x2B: //DEC HL
	HL.W--;
	return 8;
case 0x2C: //INC L
	HL.B.L++;
	AF.B.L = (HL.B.L ? 0 : ZF) | ((HL.B.L & 0xF) ? 0 : HF) | (AF.B.L & CF);
	return 4;
case 0x2D: //DEC L
	HL.B.L--;
	AF.B.L = (HL.B.L ? 0 : ZF) | NF | ((HL.B.L & 0xF) == 0xF ? HF : 0) | (AF.B.L & CF);
	return 4;
case 0x2E: //LD L, d8
	HL.B.L = MBC_read(PC.W++);
	return 8;
case 0x2F: //CPL
	AF.B.H ^= 0xFF;
	AF.B.L = (AF.B.L & ZF) | NF | HF | (AF.B.L & CF);
	return 4;
case 0x30: //JR NC, r8
	if(AF.B.L & CF) {
		PC.W++;
		return 8;
	} else {
		PC.W += (signed char)MBC_read(PC.W) + 1;
		return 12;
	}
case 0x31: //LD SP, d16
	SP.B.L = MBC_read(PC.W++);
	SP.B.H = MBC_read(PC.W++);
	return 12;
case 0x32: //LD (HL-), A
	MBC_write(HL.W--, AF.B.H);
	return 8;
case 0x33: //INC SP
	SP.W++;
	return 8;
case 0x34: //INC (HL)
	TR.B.L = MBC_read(HL.W) + 1;
	MBC_write(HL.W, TR.B.L);
	AF.B.L = (TR.B.L ? 0 : ZF) | ((TR.B.L & 0xF) ? 0 : HF) | (AF.B.L & CF);
	return 12;
case 0x35: //DEC (HL)
	TR.B.L = MBC_read(HL.W) - 1;
	MBC_write(HL.W, TR.B.L);
	AF.B.L = (TR.B.L ? 0 : ZF) | NF | ((TR.B.L & 0xF) == 0xF ? HF : 0) | (AF.B.L & CF);
	return 12;
case 0x36: //LD (HL), d8
	MBC_write(HL.W, MBC_read(PC.W++));
	return 12;
case 0x37: //SCF
	AF.B.L = (AF.B.L & ZF) | CF;
	return 4;
case 0x38: //JR C, r8
	if(AF.B.L & CF) {
		PC.W += (signed char)MBC_read(PC.W) + 1;
		return 12;
	} else {
		PC.W++;
		return 8;
	}
case 0x39: //ADD HL, SP
	TR.W = HL.W + SP.W;
	AF.B.L = (AF.B.L & ZF) | (((HL.W ^ SP.W ^ TR.W) & 0x1000) ? HF : 0) | (TR.W < SP.W ? CF : 0);
	HL.W = TR.W;
	return 8;
case 0x3A: //LD A, (HL-)
	AF.B.H = MBC_read(HL.W--);	
	return 8;
case 0x3B: //DEC SP
	SP.W--;
	return 8;
case 0x3C: //INC A
	AF.B.H++;
	AF.B.L = (AF.B.H ? 0 : ZF) | ((AF.B.H & 0xF) ? 0 : HF) | (AF.B.L & CF);
	return 4;
case 0x3D: //DEC A
	AF.B.H--;
	AF.B.L = (AF.B.H ? 0 : ZF) | NF | ((AF.B.H & 0xF) == 0xF ? HF : 0) | (AF.B.L & CF);
	return 4;
case 0x3E: //LD A, d8
	AF.B.H = MBC_read(PC.W++);
	return 8;
case 0x3F: //CCF
	AF.B.L = (AF.B.L & ZF) | (~(AF.B.L & CF) & CF);
	return 4;
	
case 0x40: //LD B, B
	BC.B.H = BC.B.H;
	return 4;
case 0x41: //LD B, C
	BC.B.H = BC.B.L;
	return 4;
case 0x42: //LD B, D
	BC.B.H = DE.B.H;
	return 4;
case 0x43: //LD B, E
	BC.B.H = DE.B.L;
	return 4;
case 0x44: //LD B, H
	BC.B.H = HL.B.H;
	return 4;
case 0x45: //LD B, L
	BC.B.H = HL.B.L;
	return 4;
case 0x46: //LD B, (HL)
	BC.B.H = MBC_read(HL.W);
	return 8;
case 0x47: //LD B, A
	BC.B.H = AF.B.H;
	return 4;
case 0x48: //LD C, B
	BC.B.L = BC.B.H;
	return 4;
case 0x49: //LD C, C
	BC.B.L = BC.B.L;
	return 4;
case 0x4A: //LD C, D
	BC.B.L = DE.B.H;
	return 4;
case 0x4B: //LD C, E
	BC.B.L = DE.B.L;
	return 4;
case 0x4C: //LD C, H
	BC.B.L = HL.B.H;
	return 4;
case 0x4D: //LD C, L
	BC.B.L = HL.B.L;
	return 4;
case 0x4E: //LD C, (HL)
	BC.B.L = MBC_read(HL.W);
	return 8;
case 0x4F: //LD C, A
	BC.B.L = AF.B.H;
	return 4;
case 0x50: //LD D, B
	DE.B.H = BC.B.H;
	return 4;
case 0x51: //LD D, C
	DE.B.H = BC.B.L;
	return 4;
case 0x52: //LD D, D
	DE.B.H = DE.B.H;
	return 4;
case 0x53: //LD D, E
	DE.B.H = DE.B.L;
	return 4;
case 0x54: //LD D, H
	DE.B.H = HL.B.H;
	return 4;
case 0x55: //LD D, L
	DE.B.H = HL.B.L;
	return 4;
case 0x56: //LD D, (HL)
	DE.B.H = MBC_read(HL.W);
	return 8;
case 0x57: //LD D, A
	DE.B.H = AF.B.H;
	return 4;
case 0x58: //LD E, B
	DE.B.L = BC.B.H;
	return 4;
case 0x59: //LD E, C
	DE.B.L = BC.B.L;
	return 4;
case 0x5A: //LD E, D
	DE.B.L = DE.B.H;
	return 4;
case 0x5B: //LD E, E
	DE.B.L = DE.B.L;
	return 4;
case 0x5C: //LD E, H
	DE.B.L = HL.B.H;
	return 4;
case 0x5D: //LD E, L
	DE.B.L = HL.B.L;
	return 4;
case 0x5E: //LD E, (HL)
	DE.B.L = MBC_read(HL.W);
	return 8;
case 0x5F: //LD E, A
	DE.B.L = AF.B.H;
	return 4;
case 0x60: //LD H, B
	HL.B.H = BC.B.H;
	return 4;
case 0x61: //LD H, C
	HL.B.H = BC.B.L;
	return 4;
case 0x62: //LD H, D
	HL.B.H = DE.B.H;
	return 4;
case 0x63: //LD H, E
	HL.B.H = DE.B.L;
	return 4;
case 0x64: //LD H, H
	HL.B.H = HL.B.H;
	return 4;
case 0x65: //LD H, L
	HL.B.H = HL.B.L;
	return 4;
case 0x66: //LD H, (HL)
	HL.B.H = MBC_read(HL.W);
	return 8;
case 0x67: //LD H, A
	HL.B.H = AF.B.H;
	return 4;
case 0x68: //LD L, B
	HL.B.L = BC.B.H;
	return 4;
case 0x69: //LD L, C
	HL.B.L = BC.B.L;
	return 4;
case 0x6A: //LD L, D
	HL.B.L = DE.B.H;
	return 4;
case 0x6B: //LD L, E
	HL.B.L = DE.B.L;
	return 4;
case 0x6C: //LD L, H
	HL.B.L = HL.B.H;
	return 4;
case 0x6D: //LD L, L
	HL.B.L = HL.B.L;
	return 4;
case 0x6E: //LD H, (HL)
	HL.B.L = MBC_read(HL.W);
	return 8;
case 0x6F: //LD H, A
	HL.B.L = AF.B.H;
	return 4;
case 0x70: //LD (HL), B
	MBC_write(HL.W, BC.B.H);
	return 8;
case 0x71: //LD (HL), C
	MBC_write(HL.W, BC.B.L);
	return 8;
case 0x72: //LD (HL), D
	MBC_write(HL.W, DE.B.H);
	return 8;
case 0x73: //LD (HL), E
	MBC_write(HL.W, DE.B.L);
	return 8;
case 0x74: //LD (HL), H
	MBC_write(HL.W, HL.B.H);
	return 8;
case 0x75: //LD (HL), L
	MBC_write(HL.W, HL.B.L);
	return 8;
case 0x76: //HALT
	//printf("HALT (not sure how to handle this yet)\n");
	//getchar();
	//exit(1);
	return 4;
case 0x77: //LD (HL), A
	MBC_write(HL.W, AF.B.H);
	return 8;
case 0x78: //LD A, B
	AF.B.H = BC.B.H;
	return 4;
case 0x79: //LD A, C
	AF.B.H = BC.B.L;
	return 4;
case 0x7A: //LD A, D
	AF.B.H = DE.B.H;
	return 4;
case 0x7B: //LD A, E
	AF.B.H = DE.B.L;
	return 4;
case 0x7C: //LD A, H
	AF.B.H = HL.B.H;
	return 4;
case 0x7D: //LD A, L
	AF.B.H = HL.B.L;
	return 4;
case 0x7E: //LD A, (HL)
	AF.B.H = MBC_read(HL.W);
	return 8;
case 0x7F: //LD A, A
	AF.B.H = AF.B.H;
	return 4;

case 0x80: //ADD A, B
	TR.B.L = AF.B.H + BC.B.H;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ BC.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < BC.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x81: //ADD A, C
	TR.B.L = AF.B.H + BC.B.L;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ BC.B.L ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < BC.B.L ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x82: //ADD A, D
	TR.B.L = AF.B.H + DE.B.H;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ DE.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < DE.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x83: //ADD A, E
	TR.B.L = AF.B.H + DE.B.L;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ DE.B.L ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < DE.B.L ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x84: //ADD A, H
	TR.B.L = AF.B.H + HL.B.H;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ HL.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < HL.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x85: //ADD A, L
	TR.B.L = AF.B.H + HL.B.L;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ HL.B.L ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < HL.B.L ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x86: //ADD A, (HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.L = AF.B.H + TR.B.H;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ TR.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < TR.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 8;
case 0x87: //ADD A, A
	TR.B.L = AF.B.H + AF.B.H;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ AF.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < AF.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
	
case 0x88: //ADC A, B
	TR.B.L = AF.B.H + BC.B.H + ((AF.B.L & CF) >> 4);
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ BC.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < BC.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x89: //ADC A, C
	TR.B.L = AF.B.H + BC.B.L + ((AF.B.L & CF) >> 4);
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ BC.B.L ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < BC.B.L ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x8A: //ADC A, D
	TR.B.L = AF.B.H + DE.B.H + ((AF.B.L & CF) >> 4);
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ DE.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < DE.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x8B: //ADC A, E
	TR.B.L = AF.B.H + DE.B.L + ((AF.B.L & CF) >> 4);
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ DE.B.L ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < DE.B.L ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x8C: //ADC A, H
	TR.B.L = AF.B.H + HL.B.H + ((AF.B.L & CF) >> 4);
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ HL.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < HL.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x8D: //ADC A, L
	TR.B.L = AF.B.H + HL.B.L + ((AF.B.L & CF) >> 4);
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ HL.B.L ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < HL.B.L ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
case 0x8E: //ADC A, (HL)
	TR.B.H = MBC_read(HL.W) + ((AF.B.L & CF) >> 4);
	TR.B.L = AF.B.H + TR.B.H;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ TR.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < TR.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 8;
case 0x8F: //ADC A, A
	TR.B.L = AF.B.H + AF.B.H + ((AF.B.L & CF) >> 4);
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ AF.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < AF.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 4;
	
case 0x90: //SUB A, B
	AF.B.L = ((AF.B.H == BC.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (BC.B.H & 0xF)) ? HF : 0) | ((AF.B.H < BC.B.H) ? CF : 0);
	AF.B.H -= BC.B.H;
	return 4;
case 0x91: //SUB A, C
	AF.B.L = ((AF.B.H == BC.B.L) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (BC.B.L & 0xF)) ? HF : 0) | ((AF.B.H < BC.B.L) ? CF : 0);
	AF.B.H -= BC.B.L;
	return 4;
case 0x92: //SUB A, D
	AF.B.L = ((AF.B.H == DE.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (DE.B.H & 0xF)) ? HF : 0) | ((AF.B.H < DE.B.H) ? CF : 0);
	AF.B.H -= DE.B.H;
	return 4;
case 0x93: //SUB A, E
	AF.B.L = ((AF.B.H == DE.B.L) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (DE.B.L & 0xF)) ? HF : 0) | ((AF.B.H < DE.B.L) ? CF : 0);
	AF.B.H -= DE.B.L;
	return 4;
case 0x94: //SUB A, H
	AF.B.L = ((AF.B.H == HL.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (HL.B.H & 0xF)) ? HF : 0) | ((AF.B.H < HL.B.H) ? CF : 0);
	AF.B.H -= HL.B.H;
	return 4;
case 0x95: //SUB A, L
	AF.B.L = ((AF.B.H == HL.B.L) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (HL.B.L & 0xF)) ? HF : 0) | ((AF.B.H < HL.B.L) ? CF : 0);
	AF.B.H -= HL.B.L;
	return 4;
case 0x96: //SUB A, (HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 8;
case 0x97: //SUB A, A
	AF.B.L = ((AF.B.H == AF.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (AF.B.H & 0xF)) ? HF : 0) | ((AF.B.H < AF.B.H) ? CF : 0);
	AF.B.H -= AF.B.H;
	return 4;
case 0x98: //SBC A, B
	TR.B.H = BC.B.H - ((AF.B.L & CF) >> 4);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 4;
case 0x99: //SBC A, C
	TR.B.H = BC.B.L - ((AF.B.L & CF) >> 4);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 4;
case 0x9A: //SBC A, D
	TR.B.H = DE.B.H - ((AF.B.L & CF) >> 4);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 4;
case 0x9B: //SBC A, E
	TR.B.H = DE.B.L - ((AF.B.L & CF) >> 4);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 4;
case 0x9C: //SBC A, H
	TR.B.H = HL.B.H - ((AF.B.L & CF) >> 4);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 4;
case 0x9D: //SBC A, L
	TR.B.H = DE.B.L - ((AF.B.L & CF) >> 4);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 4;
case 0x9E: //SBC A, (HL)
	TR.B.H = MBC_read(HL.W) - ((AF.B.L & CF) >> 4);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 8;
case 0x9F: // SBC A, A
	TR.B.H = AF.B.H - ((AF.B.L & CF) >> 4);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 4;
	
case 0xA0: //AND B
	AF.B.H &= BC.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF) | HF;
	return 4;
case 0xA1: //AND C
	AF.B.H &= BC.B.L;
	AF.B.L = (AF.B.H ? 0 : ZF) | HF;
	return 4;
case 0xA2: //AND D
	AF.B.H &= DE.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF) | HF;
	return 4;
case 0xA3: //AND E
	AF.B.H &= DE.B.L;
	AF.B.L = (AF.B.H ? 0 : ZF) | HF;
	return 4;
case 0xA4: //AND H
	AF.B.H &= HL.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF) | HF;
	return 4;
case 0xA5: //AND L
	AF.B.H &= HL.B.L;
	AF.B.L = (AF.B.H ? 0 : ZF) | HF;
	return 4;
case 0xA6: //AND (HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.H &= TR.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF) | HF;
	return 8;
case 0xA7: //AND A
	AF.B.H &= AF.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF) | HF;
	return 4;
case 0xA8: //XOR B
	AF.B.H ^= BC.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xA9: //XOR C
	AF.B.H ^= BC.B.L;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xAA: //XOR D
	AF.B.H ^= DE.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xAB: //XOR E
	AF.B.H ^= DE.B.L;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xAC: //XOR H
	AF.B.H ^= HL.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xAD: //XOR L
	AF.B.H ^= HL.B.L;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xAE: //XOR (HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.H ^= TR.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 8;
case 0xAF: //XOR A
	AF.B.H ^= AF.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xB0: //OR B
	AF.B.H |= BC.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xB1: //OR C
	AF.B.H |= BC.B.L;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xB2: //OR D
	AF.B.H |= DE.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xB3: //OR E
	AF.B.H |= DE.B.L;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xB4: //OR H
	AF.B.H |= HL.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xB5: //OR L
	AF.B.H |= HL.B.L;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
case 0xB6: //OR (HL)
	AF.B.H |= MBC_read(HL.W);
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 8;
case 0xB7: //OR A
	AF.B.H |= AF.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 4;
	
case 0xB8: //CP A, B
	AF.B.L = ((AF.B.H == BC.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (BC.B.H & 0xF)) ? HF : 0) | ((AF.B.H < BC.B.H) ? CF : 0);
	return 4;
case 0xB9: //CP A, C
	AF.B.L = ((AF.B.H == BC.B.L) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (BC.B.L & 0xF)) ? HF : 0) | ((AF.B.H < BC.B.L) ? CF : 0);
	return 4;
case 0xBA: //CP A, D
	AF.B.L = ((AF.B.H == DE.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (DE.B.H & 0xF)) ? HF : 0) | ((AF.B.H < DE.B.H) ? CF : 0);
	return 4;
case 0xBB: //CP A, E
	AF.B.L = ((AF.B.H == DE.B.L) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (DE.B.L & 0xF)) ? HF : 0) | ((AF.B.H < DE.B.L) ? CF : 0);
	return 4;
case 0xBC: //CP A, H
	AF.B.L = ((AF.B.H == HL.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (HL.B.H & 0xF)) ? HF : 0) | ((AF.B.H < HL.B.H) ? CF : 0);
	return 4;
case 0xBD: //CP A, L
	AF.B.L = ((AF.B.H == HL.B.L) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (HL.B.L & 0xF)) ? HF : 0) | ((AF.B.H < HL.B.L) ? CF : 0);
	return 4;
case 0xBE: //CP A, (HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	return 8;
case 0xBF: //CP A, A
	AF.B.L = ((AF.B.H == AF.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (AF.B.H & 0xF)) ? HF : 0) | ((AF.B.H < AF.B.H) ? CF : 0);
	return 4;
	
case 0xC0: //RET NZ
	if(AF.B.L & ZF) {
		return 8;
	} else {
		//printf("Make sure this is in the right order!\n");
		PC.B.L = MBC_read(SP.W++);
		PC.B.H = MBC_read(SP.W++);
		//printf("Popped %X from the stack.\n", PC.W);
		return 20;
	}
case 0xC1: //POP BC
	BC.B.L = MBC_read(SP.W++);
	BC.B.H = MBC_read(SP.W++);
	//printf("Make sure this is in the right order!\n");
	//printf("Popped %X from the stack.\n", PC.W);
	return 12;
case 0xC2: //JP NZ, a16
	if(AF.B.L & ZF) {
		PC.W += 2;
		return 12;
	} else {
		TR.B.L = MBC_read(PC.W++);
		TR.B.H = MBC_read(PC.W);
		PC.W = TR.W;
		return 16;
	}
case 0xC3: //JP a16
	TR.B.L = MBC_read(PC.W++);
	TR.B.H = MBC_read(PC.W);
	PC.W = TR.W;
	return 16;
case 0xC4: //CALL NZ, a16
	if(AF.B.L & ZF) {
		PC.W += 2;
		return 12;
	} else {
		TR.B.L = MBC_read(PC.W++);
		TR.B.H = MBC_read(PC.W++);
		MBC_write(--SP.W, PC.B.H);
		MBC_write(--SP.W, PC.B.L);
		PC.W = TR.W;
		return 24;
	}
case 0xC5: //PUSH BC
	MBC_write(--SP.W, BC.B.H);
	MBC_write(--SP.W, BC.B.L);
	return 16;
case 0xC6: //ADD A, d8
	TR.B.H = MBC_read(PC.W++);
	TR.B.L = AF.B.H + TR.B.H;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ TR.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < TR.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 8;
case 0xC7: //RST 00H
	MBC_write(--SP.W, PC.B.H);
	MBC_write(--SP.W, PC.B.L);
	PC.B.H = 0;
	PC.B.L = 0;
	return 16;
case 0xC8: //RET Z
	if(AF.B.L & ZF) {
		PC.B.L = MBC_read(SP.W++);
		PC.B.H = MBC_read(SP.W++);
		return 20;
	} else {
		return 8;
	}
case 0xC9: //RET
	PC.B.L = MBC_read(SP.W++);
	PC.B.H = MBC_read(SP.W++);
	return 16;
case 0xCA: //JP Z, a16
	if(AF.B.L & ZF) {
		TR.B.L = MBC_read(PC.W++);
		TR.B.H = MBC_read(PC.W);
		PC.W = TR.W;
		return 16;
	} else {
		PC.W += 2;
		return 12;
	}
case 0xCB: //PREFIX CB
{
	IR.W = MBC_read(PC.W++);
	switch(IR.W) {
case 0x00: //RLC B
	BC.B.H = (BC.B.H << 1) | ((BC.B.H & 0x80) >> 7);
	AF.B.L = (BC.B.H ? 0 : ZF) | (BC.B.H & 1 ? CF : 0);
	return 8;
case 0x01: //RLC C
	BC.B.L = (BC.B.L << 1) | ((BC.B.L & 0x80) >> 7);
	AF.B.L = (BC.B.L ? 0 : ZF) | (BC.B.L & 1 ? CF : 0);
	return 8;
case 0x02: //RLC D
	DE.B.H = (DE.B.H << 1) | ((DE.B.H & 0x80) >> 7);
	AF.B.L = (DE.B.H ? 0 : ZF) | (DE.B.H & 1 ? CF : 0);
	return 8;
case 0x03: //RLC E
	DE.B.L = (DE.B.L << 1) | ((DE.B.L & 0x80) >> 7);
	AF.B.L = (DE.B.L ? 0 : ZF) | (DE.B.L & 1 ? CF : 0);
	return 8;
case 0x04: //RLC H
	HL.B.H = (HL.B.H << 1) | ((HL.B.H & 0x80) >> 7);
	AF.B.L = (HL.B.H ? 0 : ZF) | (HL.B.H & 1 ? CF : 0);
	return 8;
case 0x05: //RLC L
	HL.B.L = (HL.B.L << 1) | ((HL.B.L & 0x80) >> 7);
	AF.B.L = (HL.B.L ? 0 : ZF) | (HL.B.L & 1 ? CF : 0);
	return 8;
case 0x06: //RLC (HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H = (TR.B.H << 1) | ((TR.B.H & 0x80) >> 7);
	AF.B.L = (TR.B.H ? 0 : ZF) | (TR.B.H & 1 ? CF : 0);
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x07: //RLC A
	AF.B.H = (AF.B.H << 1) | ((AF.B.H & 0x80) >> 7);
	AF.B.L = (AF.B.H ? 0 : ZF) | (AF.B.H & 1 ? CF : 0);
	return 8;
case 0x08: //RRC B
	BC.B.H = (BC.B.H >> 1) | ((BC.B.H & 1) << 7);
	AF.B.L = (BC.B.H ? 0 : ZF) | (BC.B.H & 0x80 ? CF : 0);
	return 8;
case 0x09: //RRC C
	BC.B.L = (BC.B.L >> 1) | ((BC.B.L & 1) << 7);
	AF.B.L = (BC.B.L ? 0 : ZF) | (BC.B.L & 0x80 ? CF : 0);
	return 8;
case 0x0A: //RRC D
	DE.B.H = (DE.B.H >> 1) | ((DE.B.H & 1) << 7);
	AF.B.L = (DE.B.H ? 0 : ZF) | (DE.B.H & 0x80 ? CF : 0);
	return 8;
case 0x0B: //RRC E
	DE.B.L = (DE.B.L >> 1) | ((DE.B.L & 1) << 7);
	AF.B.L = (DE.B.L ? 0 : ZF) | (DE.B.L & 0x80 ? CF : 0);
	return 8;
case 0x0C: //RRC H
	HL.B.H = (HL.B.H >> 1) | ((HL.B.H & 1) << 7);
	AF.B.L = (HL.B.H ? 0 : ZF) | (HL.B.H & 0x80 ? CF : 0);
	return 8;
case 0x0D: //RRC L
	HL.B.L = (HL.B.L >> 1) | ((HL.B.L & 1) << 7);
	AF.B.L = (HL.B.L ? 0 : ZF) | (HL.B.L & 0x80 ? CF : 0);
	return 8;
case 0x0E: //RRC (HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H = (TR.B.H >> 1) | ((TR.B.H & 1) << 7);
	AF.B.L = (TR.B.H ? 0 : ZF) | (TR.B.H & 0x80 ? CF : 0);
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x0F: //RRC A
	AF.B.H = (AF.B.H >> 1) | ((AF.B.H & 1) << 7);
	AF.B.L = (AF.B.H ? 0 : ZF) | (AF.B.H & 0x80 ? CF : 0);
	return 8;
case 0x10: //RL B
	TR.B.L = BC.B.H;
	BC.B.H = (BC.B.H << 1) | ((AF.B.L & CF) >> 4);
	AF.B.L = (BC.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x11: //RL C
	TR.B.L = BC.B.L;
	BC.B.L = (BC.B.L << 1) | ((AF.B.L & CF) >> 4);
	AF.B.L = (BC.B.L ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x12: //RL D
	TR.B.L = DE.B.H;
	DE.B.H = (DE.B.H << 1) | ((AF.B.L & CF) >> 4);
	AF.B.L = (DE.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x13: //RL E
	TR.B.L = DE.B.L;
	DE.B.L = (DE.B.L << 1) | ((AF.B.L & CF) >> 4);
	AF.B.L = (DE.B.L ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x14: //RL H
	TR.B.L = HL.B.H;
	HL.B.H = (HL.B.H << 1) | ((AF.B.L & CF) >> 4);
	AF.B.L = (HL.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x15: //RL L
	TR.B.L = HL.B.L;
	HL.B.L = (HL.B.L << 1) | ((AF.B.L & CF) >> 4);
	AF.B.L = (HL.B.L ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x16: //RL (HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.L = TR.B.H;
	TR.B.H = (TR.B.H << 1) | ((AF.B.L & CF) >> 4);
	AF.B.L = (TR.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x17: //RL A
	TR.B.L = AF.B.H;
	AF.B.H = (AF.B.H << 1) | ((AF.B.L & CF) >> 4);
	AF.B.L = (AF.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x18: //RR B
	TR.B.L = BC.B.H;
	BC.B.H = (BC.B.H >> 1) | ((AF.B.L & CF) << 3);
	AF.B.L = (BC.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x19: //RR C
	TR.B.L = BC.B.L;
	BC.B.L = (BC.B.L >> 1) | ((AF.B.L & CF) << 3);
	AF.B.L = (BC.B.L ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x1A: //RR D
	TR.B.L = DE.B.H;
	DE.B.H = (DE.B.H >> 1) | ((AF.B.L & CF) << 3);
	AF.B.L = (DE.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x1B: //RR E
	TR.B.L = DE.B.L;
	DE.B.L = (DE.B.L >> 1) | ((AF.B.L & CF) << 3);
	AF.B.L = (DE.B.L ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x1C: //RR H
	TR.B.L = HL.B.H;
	HL.B.H = (HL.B.H >> 1) | ((AF.B.L & CF) << 3);
	AF.B.L = (HL.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x1D: //RR L
	TR.B.L = HL.B.L;
	HL.B.L = (HL.B.L >> 1) | ((AF.B.L & CF) << 3);
	AF.B.L = (HL.B.L ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x1E: //RR (HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.L = TR.B.H;
	TR.B.H = (TR.B.H >> 1) | ((AF.B.L & CF) << 3);
	AF.B.L = (TR.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x1F: //RR A
	TR.B.L = AF.B.H;
	AF.B.H = (AF.B.H >> 1) | ((AF.B.L & CF) << 3);
	AF.B.L = (AF.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x20: //SLA B
	TR.B.L = BC.B.H;
	BC.B.H = (BC.B.H << 1);
	AF.B.L = (BC.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x21: //SLA C
	TR.B.L = BC.B.L;
	BC.B.L = (BC.B.L << 1);
	AF.B.L = (BC.B.L ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x22: //SLA D
	TR.B.L = DE.B.H;
	DE.B.H = (DE.B.H << 1);
	AF.B.L = (DE.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x23: //SLA E
	TR.B.L = DE.B.L;
	DE.B.L = (DE.B.L << 1);
	AF.B.L = (DE.B.L ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x24: //SLA H
	TR.B.L = HL.B.H;
	HL.B.H = (HL.B.H << 1);
	AF.B.L = (HL.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x25: //SLA L
	TR.B.L = HL.B.L;
	HL.B.L = (HL.B.L << 1);
	AF.B.L = (HL.B.L ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x26: //SLA (HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.L = TR.B.H;
	TR.B.H = (TR.B.H << 1);
	AF.B.L = (TR.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x27: //SLA A
	TR.B.L = AF.B.H;
	AF.B.H = (AF.B.H << 1);
	AF.B.L = (AF.B.H ? 0 : ZF) | (TR.B.L & 0x80 ? CF : 0);
	return 8;
case 0x28: //SRA B
	TR.B.L = BC.B.H;
	BC.B.H = (BC.B.H >> 1) | (BC.B.H & 0x80);
	AF.B.L = (BC.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x29: //SRA C
	TR.B.L = BC.B.L;
	BC.B.L = (BC.B.L >> 1) | (BC.B.L & 0x80);
	AF.B.L = (BC.B.L ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x2A: //SRA D
	TR.B.L = DE.B.H;
	DE.B.H = (DE.B.H >> 1) | (DE.B.H & 0x80);
	AF.B.L = (DE.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;	
case 0x2B: //SRA E
	TR.B.L = DE.B.L;
	DE.B.L = (DE.B.L >> 1) | (DE.B.L & 0x80);
	AF.B.L = (DE.B.L ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;	
case 0x2C: //SRA H
	TR.B.L = HL.B.H;
	HL.B.H = (HL.B.H >> 1) | (HL.B.H & 0x80);
	AF.B.L = (HL.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;	
case 0x2D: //SRA L
	TR.B.L = HL.B.L;
	HL.B.L = (HL.B.L >> 1) | (HL.B.L & 0x80);
	AF.B.L = (HL.B.L ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x2E: //SRA (HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.L = TR.B.H;
	TR.B.H = (TR.B.H >> 1) | (TR.B.H & 0x80);
	AF.B.L = (TR.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x2F: //SRA A
	TR.B.L = AF.B.H;
	AF.B.H = (AF.B.H >> 1) | (AF.B.H & 0x80);
	AF.B.L = (AF.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x30: //SWAP B
	BC.B.H = (BC.B.H << 4) | (BC.B.H >> 4);
	AF.B.L = (BC.B.H ? 0 : ZF);
	return 8;
case 0x31: //SWAP C
	BC.B.L = (BC.B.L << 4) | (BC.B.L >> 4);
	AF.B.L = (BC.B.L ? 0 : ZF);
	return 8;
case 0x32: //SWAP D
	DE.B.H = (DE.B.H << 4) | (DE.B.H >> 4);
	AF.B.L = (DE.B.H ? 0 : ZF);
	return 8;
case 0x33: //SWAP E
	DE.B.L = (DE.B.L << 4) | (DE.B.L >> 4);
	AF.B.L = (DE.B.L ? 0 : ZF);
	return 8;
case 0x34: //SWAP H
	HL.B.H = (HL.B.H << 4) | (HL.B.H >> 4);
	AF.B.L = (HL.B.H ? 0 : ZF);
	return 8;
case 0x35: //SWAP L
	HL.B.L = (HL.B.L << 4) | (HL.B.L >> 4);
	AF.B.L = (HL.B.L ? 0 : ZF);
	return 8;
case 0x36: //SWAP (HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H = (TR.B.H << 4) | (TR.B.H >> 4);
	AF.B.L = (TR.B.H ? 0 : ZF);
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x37: //SWAP A
	AF.B.H = (AF.B.H << 4) | (AF.B.H >> 4);
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 8;
case 0x38: //SRL B
	TR.B.L = BC.B.H;
	BC.B.H = (BC.B.H >> 1);
	AF.B.L = (BC.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x39: //SRL C
	TR.B.L = BC.B.L;
	BC.B.L = (BC.B.L >> 1);
	AF.B.L = (BC.B.L ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x3A: //SRL D
	TR.B.L = DE.B.H;
	DE.B.H = (DE.B.H >> 1);
	AF.B.L = (DE.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x3B: //SRL E
	TR.B.L = DE.B.L;
	DE.B.L = (DE.B.L >> 1);
	AF.B.L = (DE.B.L ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x3C: //SRL H
	TR.B.L = HL.B.H;
	HL.B.H = (HL.B.H >> 1);
	AF.B.L = (HL.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x3D: //SRL L
	TR.B.L = HL.B.L;
	HL.B.L = (HL.B.L >> 1);
	AF.B.L = (HL.B.L ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x3E: //SRL (HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.L = TR.B.H;
	TR.B.H = (TR.B.H >> 1);
	AF.B.L = (TR.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x3F: //SRL A
	TR.B.L = AF.B.H;
	AF.B.H = (AF.B.H >> 1);
	AF.B.L = (AF.B.H ? 0 : ZF) | (TR.B.L & 1 ? CF : 0);
	return 8;
case 0x40: //BIT 0,B
	AF.B.L = (BC.B.H & 0x1 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x41: //BIT 0,C
	AF.B.L = (BC.B.L & 0x1 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x42: //BIT 0,D
	AF.B.L = (DE.B.H & 0x1 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x43: //BIT 0,E
	AF.B.L = (DE.B.L & 0x1 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x44: //BIT 0,H
	AF.B.L = (HL.B.H & 0x1 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x45: //BIT 0,L
	AF.B.L = (HL.B.L & 0x1 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x46: //BIT 0,(HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = (TR.B.H & 0x1 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 16;
case 0x47: //BIT 0,A
	AF.B.L = (AF.B.H & 0x1 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x48: //BIT 1,B
	AF.B.L = (BC.B.H & 0x2 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x49: //BIT 1,C
	AF.B.L = (BC.B.L & 0x2 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x4A: //BIT 1,D
	AF.B.L = (DE.B.H & 0x2 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x4B: //BIT 1,E
	AF.B.L = (DE.B.L & 0x2 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x4C: //BIT 1,H
	AF.B.L = (HL.B.H & 0x2 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x4D: //BIT 1,L
	AF.B.L = (HL.B.L & 0x2 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x4E: //BIT 1,(HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = (TR.B.H & 0x2 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 16;
case 0x4F: //BIT 1,A
	AF.B.L = (AF.B.H & 0x2 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x50: //BIT 2,B
	AF.B.L = (BC.B.H & 0x4 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x51: //BIT 2,C
	AF.B.L = (BC.B.L & 0x4 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x52: //BIT 2,D
	AF.B.L = (DE.B.H & 0x4 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x53: //BIT 2,E
	AF.B.L = (DE.B.L & 0x4 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x54: //BIT 2,H
	AF.B.L = (HL.B.H & 0x4 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x55: //BIT 2,L
	AF.B.L = (HL.B.L & 0x4 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x56: //BIT 2,(HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = (TR.B.H & 0x4 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 16;
case 0x57: //BIT 2,A
	AF.B.L = (AF.B.H & 0x4 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x58: //BIT 3,B
	AF.B.L = (BC.B.H & 0x8 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x59: //BIT 3,C
	AF.B.L = (BC.B.L & 0x8 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x5A: //BIT 3,D
	AF.B.L = (DE.B.H & 0x8 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x5B: //BIT 3,E
	AF.B.L = (DE.B.L & 0x8 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x5C: //BIT 3,H
	AF.B.L = (HL.B.H & 0x8 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x5D: //BIT 3,L
	AF.B.L = (HL.B.L & 0x8 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x5E: //BIT 3,(HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = (TR.B.H & 0x8 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 16;
case 0x5F: //BIT 3,A
	AF.B.L = (AF.B.H & 0x8 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x60: //BIT 4,B
	AF.B.L = (BC.B.H & 0x10 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x61: //BIT 4,C
	AF.B.L = (BC.B.L & 0x10 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x62: //BIT 4,D
	AF.B.L = (DE.B.H & 0x10 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x63: //BIT 4,E
	AF.B.L = (DE.B.L & 0x10 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x64: //BIT 4,H
	AF.B.L = (HL.B.H & 0x10 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x65: //BIT 4,L
	AF.B.L = (HL.B.L & 0x10 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x66: //BIT 4,(HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = (TR.B.H & 0x10 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 16;
case 0x67: //BIT 4,A
	AF.B.L = (AF.B.H & 0x10 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x68: //BIT 5,B
	AF.B.L = (BC.B.H & 0x20 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x69: //BIT 5,C
	AF.B.L = (BC.B.L & 0x20 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x6A: //BIT 5,D
	AF.B.L = (DE.B.H & 0x20 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x6B: //BIT 5,E
	AF.B.L = (DE.B.L & 0x20 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x6C: //BIT 5,H
	AF.B.L = (HL.B.H & 0x20 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x6D: //BIT 5,L
	AF.B.L = (HL.B.L & 0x20 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x6E: //BIT 5,(HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = (TR.B.H & 0x20 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 16;
case 0x6F: //BIT 5,A
	AF.B.L = (AF.B.H & 0x20 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x70: //BIT 6,B
	AF.B.L = (BC.B.H & 0x40 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x71: //BIT 6,C
	AF.B.L = (BC.B.L & 0x40 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x72: //BIT 6,D
	AF.B.L = (DE.B.H & 0x40 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x73: //BIT 6,E
	AF.B.L = (DE.B.L & 0x40 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x74: //BIT 6,H
	AF.B.L = (HL.B.H & 0x40 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x75: //BIT 6,L
	AF.B.L = (HL.B.L & 0x40 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x76: //BIT 6,(HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = (TR.B.H & 0x40 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 16;
case 0x77: //BIT 6,A
	AF.B.L = (AF.B.H & 0x40 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x78: //BIT 7,B
	AF.B.L = (BC.B.H & 0x80 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x79: //BIT 7,C
	AF.B.L = (BC.B.L & 0x80 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x7A: //BIT 7,D
	AF.B.L = (DE.B.H & 0x80 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x7B: //BIT 7,E
	AF.B.L = (DE.B.L & 0x80 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x7C: //BIT 7,H
	AF.B.L = (HL.B.H & 0x80 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x7D: //BIT 7,L
	AF.B.L = (HL.B.L & 0x80 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
case 0x7E: //BIT 7,(HL)
	TR.B.H = MBC_read(HL.W);
	AF.B.L = (TR.B.H & 0x80 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 16;
case 0x7F: //BIT 7,A
	AF.B.L = (AF.B.H & 0x80 ? 0 : ZF) | HF | (AF.B.L & CF);
	return 8;
	
case 0x80: //RES 0,B
	BC.B.H &= ~0x1;
	return 8;
case 0x81: //RES 0,C
	BC.B.L &= ~0x1;
	return 8;
case 0x82: //RES 0,D
	DE.B.H &= ~0x1;
	return 8;
case 0x83: //RES 0,E
	DE.B.L &= ~0x1;
	return 8;
case 0x84: //RES 0,H
	HL.B.H &= ~0x1;
	return 8;
case 0x85: //RES 0,L
	HL.B.L &= ~0x1;
	return 8;
case 0x86: //RES 0,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H &= ~0x1;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x87: //RES 0,A
	AF.B.H &= ~0x1;
	return 8;
case 0x88: //RES 1,B
	BC.B.H &= ~0x2;
	return 8;
case 0x89: //RES 1,C
	BC.B.L &= ~0x2;
	return 8;
case 0x8A: //RES 1,D
	DE.B.H &= ~0x2;
	return 8;
case 0x8B: //RES 1,E
	DE.B.L &= ~0x2;
	return 8;
case 0x8C: //RES 1,H
	HL.B.H &= ~0x2;
	return 8;
case 0x8D: //RES 1,L
	HL.B.L &= ~0x2;
	return 8;
case 0x8E: //RES 1,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H &= ~0x2;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x8F: //RES 1,A
	AF.B.H &= ~0x2;
	return 8;
case 0x90: //RES 2,B
	BC.B.H &= ~0x4;
	return 8;
case 0x91: //RES 2,C
	BC.B.L &= ~0x4;
	return 8;
case 0x92: //RES 2,D
	DE.B.H &= ~0x4;
	return 8;
case 0x93: //RES 2,E
	DE.B.L &= ~0x4;
	return 8;
case 0x94: //RES 2,H
	HL.B.H &= ~0x4;
	return 8;
case 0x95: //RES 2,L
	HL.B.L &= ~0x4;
	return 8;
case 0x96: //RES 2,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H &= ~0x4;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x97: //RES 2,A
	AF.B.H &= ~0x4;
	return 8;
case 0x98: //RES 3,B
	BC.B.H &= ~0x8;
	return 8;
case 0x99: //RES 3,C
	BC.B.L &= ~0x8;
	return 8;
case 0x9A: //RES 3,D
	DE.B.H &= ~0x8;
	return 8;
case 0x9B: //RES 3,E
	DE.B.L &= ~0x8;
	return 8;
case 0x9C: //RES 3,H
	HL.B.H &= ~0x8;
	return 8;
case 0x9D: //RES 3,L
	HL.B.L &= ~0x8;
	return 8;
case 0x9E: //RES 3,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H &= ~0x8;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0x9F: //RES 3,A
	AF.B.H &= ~0x8;
	return 8;
case 0xA0: //RES 4,B
	BC.B.H &= ~0x10;
	return 8;
case 0xA1: //RES 4,C
	BC.B.L &= ~0x10;
	return 8;
case 0xA2: //RES 4,D
	DE.B.H &= ~0x10;
	return 8;
case 0xA3: //RES 4,E
	DE.B.L &= ~0x10;
	return 8;
case 0xA4: //RES 4,H
	HL.B.H &= ~0x10;
	return 8;
case 0xA5: //RES 4,L
	HL.B.L &= ~0x10;
	return 8;
case 0xA6: //RES 4,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H &= ~0x10;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xA7: //RES 4,A
	AF.B.H &= ~0x10;
	return 8;
case 0xA8: //RES 5,B
	BC.B.H &= ~0x20;
	return 8;
case 0xA9: //RES 5,C
	BC.B.L &= ~0x20;
	return 8;
case 0xAA: //RES 5,D
	DE.B.H &= ~0x20;
	return 8;
case 0xAB: //RES 5,E
	DE.B.L &= ~0x20;
	return 8;
case 0xAC: //RES 5,H
	HL.B.H &= ~0x20;
	return 8;
case 0xAD: //RES 5,L
	HL.B.L &= ~0x20;
	return 8;
case 0xAE: //RES 5,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H &= ~0x20;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xAF: //RES 5,A
	AF.B.H &= ~0x20;
	return 8;
case 0xB0: //RES 6,B
	BC.B.H &= ~0x40;
	return 8;
case 0xB1: //RES 6,C
	BC.B.L &= ~0x40;
	return 8;
case 0xB2: //RES 6,D
	DE.B.H &= ~0x40;
	return 8;
case 0xB3: //RES 6,E
	DE.B.L &= ~0x40;
	return 8;
case 0xB4: //RES 6,H
	HL.B.H &= ~0x40;
	return 8;
case 0xB5: //RES 6,L
	HL.B.L &= ~0x40;
	return 8;
case 0xB6: //RES 6,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H &= ~0x40;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xB7: //RES 6,A
	AF.B.H &= ~0x40;
	return 8;
case 0xB8: //RES 7,B
	BC.B.H &= ~0x80;
	return 8;
case 0xB9: //RES 7,C
	BC.B.L &= ~0x80;
	return 8;
case 0xBA: //RES 7,D
	DE.B.H &= ~0x80;
	return 8;
case 0xBB: //RES 7,E
	DE.B.L &= ~0x80;
	return 8;
case 0xBC: //RES 7,H
	HL.B.H &= ~0x80;
	return 8;
case 0xBD: //RES 7,L
	HL.B.L &= ~0x80;
	return 8;
case 0xBE: //RES 7,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H &= ~0x80;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xBF: //RES 7,A
	AF.B.H &= ~0x80;
	return 8;
	

case 0xC0: //SET 0,B
	BC.B.H |= 0x1;
	return 8;
case 0xC1: //SET 0,C
	BC.B.L |= 0x1;
	return 8;
case 0xC2: //SET 0,D
	DE.B.H |= 0x1;
	return 8;
case 0xC3: //SET 0,E
	DE.B.L |= 0x1;
	return 8;
case 0xC4: //SET 0,H
	HL.B.H |= 0x1;
	return 8;
case 0xC5: //SET 0,L
	HL.B.L |= 0x1;
	return 8;
case 0xC6: //SET 0,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H |= 0x1;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xC7: //SET 0,A
	AF.B.H |= 0x1;
	return 8;
case 0xC8: //SET 1,B
	BC.B.H |= 0x2;
	return 8;
case 0xC9: //SET 1,C
	BC.B.L |= 0x2;
	return 8;
case 0xCA: //SET 1,D
	DE.B.H |= 0x2;
	return 8;
case 0xCB: //SET 1,E
	DE.B.L |= 0x2;
	return 8;
case 0xCC: //SET 1,H
	HL.B.H |= 0x2;
	return 8;
case 0xCD: //SET 1,L
	HL.B.L |= 0x2;
	return 8;
case 0xCE: //SET 1,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H |= 0x2;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xCF: //SET 1,A
	AF.B.H |= 0x2;
	return 8;
case 0xD0: //SET 2,B
	BC.B.H |= 0x4;
	return 8;
case 0xD1: //SET 2,C
	BC.B.L |= 0x4;
	return 8;
case 0xD2: //SET 2,D
	DE.B.H |= 0x4;
	return 8;
case 0xD3: //SET 2,E
	DE.B.L |= 0x4;
	return 8;
case 0xD4: //SET 2,H
	HL.B.H |= 0x4;
	return 8;
case 0xD5: //SET 2,L
	HL.B.L |= 0x4;
	return 8;
case 0xD6: //SET 2,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H |= 0x4;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xD7: //SET 2,A
	AF.B.H |= 0x4;
	return 8;
case 0xD8: //SET 3,B
	BC.B.H |= 0x8;
	return 8;
case 0xD9: //SET 3,C
	BC.B.L |= 0x8;
	return 8;
case 0xDA: //SET 3,D
	DE.B.H |= 0x8;
	return 8;
case 0xDB: //SET 3,E
	DE.B.L |= 0x8;
	return 8;
case 0xDC: //SET 3,H
	HL.B.H |= 0x8;
	return 8;
case 0xDD: //SET 3,L
	HL.B.L |= 0x8;
	return 8;
case 0xDE: //SET 3,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H |= 0x8;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xDF: //SET 3,A
	AF.B.H |= 0x8;
	return 8;
case 0xE0: //SET 4,B
	BC.B.H |= 0x10;
	return 8;
case 0xE1: //SET 4,C
	BC.B.L |= 0x10;
	return 8;
case 0xE2: //SET 4,D
	DE.B.H |= 0x10;
	return 8;
case 0xE3: //SET 4,E
	DE.B.L |= 0x10;
	return 8;
case 0xE4: //SET 4,H
	HL.B.H |= 0x10;
	return 8;
case 0xE5: //SET 4,L
	HL.B.L |= 0x10;
	return 8;
case 0xE6: //SET 4,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H |= 0x10;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xE7: //SET 4,A
	AF.B.H |= 0x10;
	return 8;
case 0xE8: //SET 5,B
	BC.B.H |= 0x20;
	return 8;
case 0xE9: //SET 5,C
	BC.B.L |= 0x20;
	return 8;
case 0xEA: //SET 5,D
	DE.B.H |= 0x20;
	return 8;
case 0xEB: //SET 5,E
	DE.B.L |= 0x20;
	return 8;
case 0xEC: //SET 5,H
	HL.B.H |= 0x20;
	return 8;
case 0xED: //SET 5,L
	HL.B.L |= 0x20;
	return 8;
case 0xEE: //SET 5,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H |= 0x20;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xEF: //SET 5,A
	AF.B.H |= 0x20;
	return 8;
case 0xF0: //SET 6,B
	BC.B.H |= 0x40;
	return 8;
case 0xF1: //SET 6,C
	BC.B.L |= 0x40;
	return 8;
case 0xF2: //SET 6,D
	DE.B.H |= 0x40;
	return 8;
case 0xF3: //SET 6,E
	DE.B.L |= 0x40;
	return 8;
case 0xF4: //SET 6,H
	HL.B.H |= 0x40;
	return 8;
case 0xF5: //SET 6,L
	HL.B.L |= 0x40;
	return 8;
case 0xF6: //SET 6,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H |= 0x40;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xF7: //SET 6,A
	AF.B.H |= 0x40;
	return 8;
case 0xF8: //SET 7,B
	BC.B.H |= 0x80;
	return 8;
case 0xF9: //SET 7,C
	BC.B.L |= 0x80;
	return 8;
case 0xFA: //SET 7,D
	DE.B.H |= 0x80;
	return 8;
case 0xFB: //SET 7,E
	DE.B.L |= 0x80;
	return 8;
case 0xFC: //SET 7,H
	HL.B.H |= 0x80;
	return 8;
case 0xFD: //SET 7,L
	HL.B.L |= 0x80;
	return 8;
case 0xFE: //SET 7,(HL)
	TR.B.H = MBC_read(HL.W);
	TR.B.H |= 0x80;
	MBC_write(HL.W, TR.B.H);
	return 16;
case 0xFF: //SET 7,A
	AF.B.H |= 0x80;
	return 8;
	} //switch
}
case 0xCC: //CALL Z, a16
	if(AF.B.L & ZF) {
		TR.B.L = MBC_read(PC.W++);
		TR.B.H = MBC_read(PC.W++);
		MBC_write(--SP.W, PC.B.H);
		MBC_write(--SP.W, PC.B.L);
		PC.W = TR.W;
		return 24;
	} else {
		PC.W += 2;
		return 12;
	}
case 0xCD: //CALL a16v
{
	TR.B.L = MBC_read(PC.W++);
	TR.B.H = MBC_read(PC.W++);
		
	/*
	//TEMPTEMPTEMP
	if(TR.W == 0x0211) { //wait for vblank 
		printf("Wait for VBLANK called.\n");
		//DISPLAY_OPCODES = true;
		//getchar();
	}
	*/
	
	MBC_write(--SP.W, PC.B.H);
	MBC_write(--SP.W, PC.B.L);
	PC.W = TR.W;
	return 24;
}
case 0xCE: //ADC A, d8
	TR.B.H = MBC_read(PC.W++) + ((AF.B.L & CF) >> 4);
	TR.B.L = AF.B.H + TR.B.H;
	AF.B.L = (TR.B.L ? 0 : ZF) | (((AF.B.H ^ TR.B.H ^ TR.B.L) & 0x10) ? HF : 0) | (TR.B.L < TR.B.H ? CF : 0);
	AF.B.H = TR.B.L;
	return 8;
case 0xCF: //RST 08H
	MBC_write(--SP.W, PC.B.H);
	MBC_write(--SP.W, PC.B.L);
	PC.B.H = 0;
	PC.B.L = 0x8;
	return 16;
case 0xD0: //RET NC
	if(AF.B.L & CF) {
		return 8;
	} else {
		PC.B.L = MBC_read(SP.W++);
		PC.B.H = MBC_read(SP.W++);
		return 20;
	}
case 0xD1: //POP DE
	DE.B.L = MBC_read(SP.W++);
	DE.B.H = MBC_read(SP.W++);
	return 12;
case 0xD2: //JP NC, a16
	if(AF.B.L & CF) {
		PC.W += 2;
		return 12;
	} else {
		TR.B.L = MBC_read(PC.W++);
		TR.B.H = MBC_read(PC.W);
		PC.W = TR.W;
		return 16;
	}
case 0xD3:
	printf("Invalid opcode!\n");
	display_cpu_values();
	getchar();
	exit(1);
case 0xD4: //CALL NC, a16
	if(AF.B.L & CF) {
		PC.W += 2;
		return 12;
	} else {
		TR.B.L = MBC_read(PC.W++);
		TR.B.H = MBC_read(PC.W++);
		MBC_write(--SP.W, PC.B.H);
		MBC_write(--SP.W, PC.B.L);
		PC.W = TR.W;
		return 24;
	}
case 0xD5: //PUSH DE
	MBC_write(--SP.W, DE.B.H);
	MBC_write(--SP.W, DE.B.L);
	return 16;
case 0xD6: //SUB d8
	TR.B.H = MBC_read(PC.W++);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 8;
case 0xD7: //RST 10H
	MBC_write(--SP.W, PC.B.H);
	MBC_write(--SP.W, PC.B.L);
	PC.B.H = 0;
	PC.B.L = 0x10;
	return 16;
case 0xD8: //RET C
	if(AF.B.L & CF) {
		PC.B.L = MBC_read(SP.W++);
		PC.B.H = MBC_read(SP.W++);
		return 20;
	} else {
		return 8;
	}
case 0xD9: //RETI
	IME = 1;
	PC.B.L = MBC_read(SP.W++);
	PC.B.H = MBC_read(SP.W++);
	return 16;
case 0xDA: //JP C, a16
	if(AF.B.L & CF) {
		TR.B.L = MBC_read(PC.W++);
		TR.B.H = MBC_read(PC.W);
		PC.W = TR.W;
		return 16;
	} else {
		PC.W += 2;
		return 12;
	}
case 0xDB:
	printf("Invalid opcode!\n");
	display_cpu_values();
	getchar();
	exit(1);
case 0xDC: //CALL C, a16
	if(AF.B.L & CF) {
		TR.B.L = MBC_read(PC.W++);
		TR.B.H = MBC_read(PC.W++);
		MBC_write(--SP.W, PC.B.H);
		MBC_write(--SP.W, PC.B.L);
		PC.W = TR.W;
		return 24;
	} else {
		PC.W += 2;
		return 12;
	}
case 0xDE: //SBC A, d8
	TR.B.H = MBC_read(PC.W++) - ((AF.B.L & CF) >> 4);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	AF.B.H -= TR.B.H;
	return 8;
case 0xDF: //RST 18H
	MBC_write(--SP.W, PC.B.H);
	MBC_write(--SP.W, PC.B.L);
	PC.B.H = 0;
	PC.B.L = 0x18;
	return 16;
case 0xE0: //LDH (a8), A
	TR.B.H = 0xFF;
	TR.B.L = MBC_read(PC.W++);
	MBC_write(TR.W, AF.B.H);
	return 12;
case 0xE1: //POP HL
	HL.B.L = MBC_read(SP.W++);
	HL.B.H = MBC_read(SP.W++);
	return 12;
case 0xE2: //LD (C), A
	TR.B.H = 0xFF;
	TR.B.L = BC.B.L;
	MBC_write(TR.W, AF.B.H);
	return 8;
case 0xE3:
case 0xE4:
	printf("Invalid opcode!\n");
	display_cpu_values();
	getchar();
	exit(1);
case 0xE5: //PUSH HL
	MBC_write(--SP.W, HL.B.H);
	MBC_write(--SP.W, HL.B.L);
	return 16;
case 0xE6: //AND d8
	TR.B.H = MBC_read(PC.W++);
	AF.B.H &= TR.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF) | HF;
	return 8;
case 0xE7: //RST 20H
	MBC_write(--SP.W, PC.B.H);
	MBC_write(--SP.W, PC.B.L);
	PC.B.H = 0;
	PC.B.L = 0x20;
	return 16;
case 0xE8: //ADD SP, r8
    offset = MBC_read(PC.W++);
	TR.W = SP.W + offset;
    if(offset >= 0) {
        AF.B.L = (((SP.W ^ TR.W ^ offset) & 0x1000) ? HF : 0) | (SP.W > TR.W ? CF : 0);
    } else {
        AF.B.L = (((SP.W ^ TR.W ^ offset) & 0x1000) ? HF : 0) | (SP.W < TR.W ? CF : 0);
    }	
	SP.W = TR.W;
	return 16;
case 0xE9: //JP (HL)
	PC.W = HL.W;
	return 4;
case 0xEA: //LD(a16), A
	TR.B.L = MBC_read(PC.W++);
	TR.B.H = MBC_read(PC.W++);
	MBC_write(TR.W, AF.B.H);
	return 16;
case 0xEB:
case 0xEC:
case 0xED:
	printf("Invalid opcode!\n");
	display_cpu_values();
	getchar();
	exit(1);
case 0xEE: //XOR d8
	TR.B.H = MBC_read(PC.W++);
	AF.B.H ^= TR.B.H;
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 8;
case 0xEF: //RST 28H
	MBC_write(--SP.W, PC.B.H);
	MBC_write(--SP.W, PC.B.L);
	PC.B.H = 0;
	PC.B.L = 0x28;
	return 16;
case 0xF0: //LDH A, (a8)
	TR.B.H = 0xFF;
	TR.B.L = MBC_read(PC.W++);
	AF.B.H = MBC_read(TR.W);
	return 12;
case 0xF1: //POP AF
	AF.B.L = MBC_read(SP.W++);
	AF.B.H = MBC_read(SP.W++);
	return 12;
case 0xF2: //LD A, (C)
	TR.B.H = 0xFF;
	TR.B.L = BC.B.L;
	AF.B.H = MBC_read(TR.W);
	return 8;
case 0xF3: //DI
	IME = 0;
	return 4;
case 0xF4:
	printf("Invalid opcode!\n");
	display_cpu_values();
	getchar();
	exit(1);
case 0xF5: //PUSH AF
	MBC_write(--SP.W, AF.B.H);
	MBC_write(--SP.W, AF.B.L);
	return 16;
case 0xF6: //OR d8
	AF.B.H |= MBC_read(PC.W++);
	AF.B.L = (AF.B.H ? 0 : ZF);
	return 8;
case 0xF7: //RST 30H
	MBC_write(--SP.W, PC.B.H);
	MBC_write(--SP.W, PC.B.L);
	PC.B.H = 0;
	PC.B.L = 0x30;
	return 16;
case 0xF8: //LD HL, SP + r8
	offset = MBC_read(PC.W++);
    TR.W = SP.W + offset;
    if(offset >= 0) {
        AF.B.L = (((SP.W ^ TR.W ^ offset) & 0x1000 ? HF : 0) | (SP.W > TR.W ? CF : 0));
    } else {
        AF.B.L = (((SP.W ^ TR.W ^ offset) & 0x1000 ? HF : 0) | (SP.W < TR.W ? CF : 0));
    }
    HL.W = TR.W;
case 0xF9: //LD SP, HL
	SP.W = HL.W;
	return 8;
case 0xFA: //LD A, (a16)
	TR.B.L = MBC_read(PC.W++);
	TR.B.H = MBC_read(PC.W++);
	AF.B.H = MBC_read(TR.W);
	return 16;
case 0xFB: //EI
	IME = 1;
	return 4;
case 0xFC:
case 0xFD:
	printf("Invalid opcode!\n");
	display_cpu_values();
	getchar();
	exit(1);
case 0xFE: //CP d8
	TR.B.H = MBC_read(PC.W++);
	AF.B.L = ((AF.B.H == TR.B.H) ? ZF : 0) | NF | (((AF.B.H & 0xF) < (TR.B.H & 0xF)) ? HF : 0) | ((AF.B.H < TR.B.H) ? CF : 0);
	return 8;
case 0xFF: //RST 38H
	MBC_write(--SP.W, PC.B.H);
	MBC_write(--SP.W, PC.B.L);
	PC.B.H = 0;
	PC.B.L = 0x38;
	return 16;
	
#endif // OPCODES_H_INCLUDED