#include "cpu.h"



z80reg PC, SP, BC, DE, HL, AF, IR, TR;

u32 DEFAULT_CLOCK_SPEED = 4194304;
//u32 VBLANK_CYCLES =  69905; //speed MHZ / 60 fps
u32 VBLANK_CYCLES =  69905;

bool cgb_mode = true;
extern bool show_opcodes;

/* 
 * Returns time difference between "timee" and "times" in micro-seconds (usec)
 */
unsigned long comp_time (struct timeval times, struct timeval timee) {

    double elap = 0.0;

    if (timee.tv_sec > times.tv_sec) {
        elap += (((double)(timee.tv_sec - times.tv_sec -1))*1000000.0);
        elap += timee.tv_usec + (1000000-times.tv_usec);
    }
    else {
        elap = timee.tv_usec - times.tv_usec;
    }
    
    return ((unsigned long)(elap));
}


int timer_counter = 1024;


bool cpu_running = false;

void cpu_start() {
	
	
	PC.W = 0;
	// enable bios
	hardware_registers[BLCK] = 0;
	
	cpu_run();
	
}

void cpu_stop() {

}

s32 scanline_counter = 456;

void cpu_interrupt(int interrupt_type) {
    switch(interrupt_type) {
        case VBLANK_INTERRUPT:
            break;
        case LCD_STAT_INTERRUPT:
            break;
        case TIMER_INTERRUPT:
            break;
        case SERIAL_INTERRUPT:
            break;
        case JOYPAD_INTERRUPT:
            hardware_registers[IF] |= 0x10;
            break;
    }
}

void cpu_run() {

	u32 cycles = 0, mode_cycles = 0, divider_counter = 0;
	
	while(cpu_running = true) {
		
		u32 current_cycles = cpu_execute();
		
		cycles += current_cycles;
		mode_cycles += current_cycles;
		
		// divider registers
		//divider_counter += cycles;
		divider_counter += current_cycles;
		
		if(divider_counter >= 255) {
			// increment divider register
			divider_counter = 0;
			++hardware_registers[DIV];
		}
		
		// update LCD status
		u8 lcd_status = hardware_registers[STAT] & 0xFC; // clear 2 LSB
		if((hardware_registers[LCDC] & LCD_DISPLAY_ENABLE) == 0) {
			// LCD is disabled
			scanline_counter = 456;
			hardware_registers[LY] = 0;
			lcd_status |= 1;
		} else {
			// LCD is enabled
			u8 lcd_mode = hardware_registers[STAT] & 0x3;
			u8 new_lcd_mode = 0;
			bool lcd_interrupt = false;
			
			if(hardware_registers[LY] > 144) {
				// in vblank, set mode to 1
				new_lcd_mode = 1;
				lcd_status |= 1;
			} else {
				if(scanline_counter >= (456 - 80)) {
					// mode 2
					new_lcd_mode = 2;
					lcd_status |= 2;
					if(lcd_status & 0x20) {
						lcd_interrupt = true;
					}
				} else if(scanline_counter >= ((456 - 80) - 172)) {
					// mode 3
					new_lcd_mode = 3;
					lcd_status |= 3;
				} else {
					// mode 0
					new_lcd_mode = 0;
					if(lcd_status & 0x8) {
						lcd_interrupt = true;
					}
				}
			}
			
			if(lcd_interrupt && (lcd_mode != new_lcd_mode)) {
				hardware_registers[IF] |= 2;
			}
		}
		
		hardware_registers[STAT] = lcd_status;
		
		// update graphics
		if(hardware_registers[LCDC] & LCD_DISPLAY_ENABLE) {
			//scanline_counter -= cycles;
			scanline_counter -= current_cycles;
			
			if(scanline_counter <= 0) {
				// increment the current scanline
				++hardware_registers[LY];
				
				// reset the scanline counter
				scanline_counter = 456;
				
				if(hardware_registers[LY] == 144) {
					// enter vertical blank
					//hardware_registers[IF] |= 0x1;
		
				} else if(hardware_registers[LY] > 153) {
					// scanline back to 0 (vblank over)
					hardware_registers[LY] = 0;
					
				} else if(hardware_registers[LY] < 144) {
					// render current scanline
					//render_scanline();
				}
			}
		}
		
		// update timer
		if(hardware_registers[TAC] & 0x4) {
			//timer_counter -= cycles;
			timer_counter -= current_cycles;
			
			if(timer_counter <= 0) {
				//reset the counter
				switch(hardware_registers[TAC] & 0x3) {
					case 0x0:
						timer_counter = 1024;
						break;
					case 0x1:
						timer_counter = 16;
						break;
					case 0x2:
						timer_counter = 64;
						break;
					case 0x3:
						timer_counter = 256;
						break;
				}
				
				if(hardware_registers[TIMA] == 255) {
					hardware_registers[TIMA] = hardware_registers[TMA];
					//trigger interrupt
					hardware_registers[IF] |= 0x4;
				} else {
					++hardware_registers[TIMA];
				}
			}
		}
		
		
		//Check for vertical blank
		if(cycles > VBLANK_CYCLES) {
			hardware_registers[IF] |= 0x1;
			cycles = 0;
			
			// testing
			//update_screen();
			
			//this works
			
			render_background_old();
			render_sprites();

			/*
			gettimeofday(&time_end, (void*)NULL);
			int sleep_usec = (1000000 - comp_time(time_start, time_end) - 1)/60;
			//usleep(sleep_usec);
			
			gettimeofday(&time_start, (void*)NULL);
			*/
		}
		
		
		
		//Check for interrupts
		if(IME && interrupt_enable && hardware_registers[IF]) {
			u8 interrupt = interrupt_enable & hardware_registers[IF];
			if(interrupt & 0x1) { 
				// VBlank interrupt

				//disable interrupts
				IME = 0;
				hardware_registers[IF] &= ~0x1;
				//Push PC onto stack
				MBC_write(--SP.W, PC.B.H);
				MBC_write(--SP.W, PC.B.L);
				PC.W = 0x40;
				cycles += 16;
			} else if(interrupt & 0x2) {
				//LCD interrupt
				IME = 0;
				hardware_registers[IF] &= ~0x2;
				MBC_write(--SP.W, PC.B.H);
				MBC_write(--SP.W, PC.B.L);
				PC.W = 0x48;
				cycles += 16;
			} else if(interrupt & 0x4) {
				//Timer interrupt
				IME = 0;
				hardware_registers[IF] &= ~0x4;
				MBC_write(--SP.W, PC.B.H);
				MBC_write(--SP.W, PC.B.L);
				PC.W = 0x50;
				cycles += 16;
			} else if(interrupt & 0x8) {
				//Joypad Interrupt
				IME = 0;
				hardware_registers[IF] &= ~0x8;
				MBC_write(--SP.W, PC.B.H);
				MBC_write(--SP.W, PC.B.L);
				PC.W = 0x60;
				cycles += 16;
			}
		}
		
	}

}

int cpu_execute() {

	// load the next intruction
	IR.W = MBC_read(PC.W++);	
	
	// execute the instruction and return cpu cycles
	switch(IR.W) {
		#include "opcodes.h"
	}
	
	// program will not reach this point
	return -1;
}

void initialize_cpu() {

	struct timeval time_start, time_end;

	// start with the bios
    PC.W = 0;
	hardware_registers[BLCK] = 0x00;

	u32 cycles = 0;	
	u32 mode_cycles = 0;
	
	int divider_counter = 0;
	
	gettimeofday(&time_start, (void*)NULL);
	
	while(1) {
	
		u32 current_cycles = execute();
		cycles += current_cycles;
		mode_cycles += current_cycles;
		
		// divider registers
		divider_counter += cycles;
		if(divider_counter >= 255) {
			// increment divider register
			divider_counter = 0;
			++hardware_registers[DIV];
		}
		
		
		// update LCD status
		u8 lcd_status = hardware_registers[STAT] & 0xFC; // clear 2 LSB
		if((hardware_registers[LCDC] & LCD_DISPLAY_ENABLE) == 0) {
			// LCD is disabled
			scanline_counter = 456;
			hardware_registers[LY] = 0;
			lcd_status |= 1;
		} else {
			//LCD is enabled
			u8 lcd_mode = hardware_registers[STAT] & 0x03;
			u8 new_lcd_mode = 0;
			bool lcd_interrupt = false;
			if(hardware_registers[LY] >= 144) {
				//vblank, set mode to 1
				new_lcd_mode = 1;
				lcd_status |= 1;
			} else {
				//mode 2
				if(scanline_counter >= (456-80)) {
					new_lcd_mode = 2;
					lcd_status |= 2;
					if(lcd_status & 0x20) {
						lcd_interrupt = true;
					}
				//mode 3
				} else if(scanline_counter >= ((456 - 80) - 172)) {
					new_lcd_mode = 3;
					lcd_status |= 3;
				//mode 0
				} else {
					new_lcd_mode = 0;
					if(lcd_status & 0x8) {
						lcd_interrupt = true;
					}
				}
			}
			
			if(lcd_interrupt && (lcd_mode != new_lcd_mode)) {
				hardware_registers[IF] |= 2;
			}
			
		}
		hardware_registers[STAT] = lcd_status;
		
		
		// update graphics
		if(hardware_registers[LCDC] & LCD_DISPLAY_ENABLE) {
			scanline_counter -= cycles;
			
			if(scanline_counter <= 0) {
				// increment the current scanline
				++hardware_registers[LY];
				
				// reset the scanline counter
				scanline_counter = 456;
				
				if(hardware_registers[LY] == 144) {
					// enter vertical blank
					hardware_registers[IF] |= 0x1;
					
				} else if(hardware_registers[LY] > 153) {
					// scanline back to 0 (vblank over)
					hardware_registers[LY] = 0;
					
				} else if(hardware_registers[LY] < 144) {
					// render current scanline
					render_scanline();
				}
			}
		}
		
		// update timer
		if(hardware_registers[TAC] & 0x4) {
			timer_counter -= cycles;
			if(timer_counter <= 0) {
				//reset the counter
				switch(hardware_registers[TAC] & 0x3) {
					case 0x0:
						timer_counter = 1024;
						break;
					case 0x1:
						timer_counter = 16;
						break;
					case 0x2:
						timer_counter = 64;
						break;
					case 0x3:
						timer_counter = 256;
						break;
				}
				
				if(hardware_registers[TIMA] == 255) {
					hardware_registers[TIMA] = hardware_registers[TMA];
					//trigger interrupt
					hardware_registers[IF] |= 0x4;
				} else {
					++hardware_registers[TIMA];
				}
			}
			
		}
		
		//Check for vertical blank
		if(cycles > VBLANK_CYCLES) {
			hardware_registers[IF] |= 0x1;
			cycles = 0;
			
			// testing
			update_screen();
			
			/*this works
			render_background_old();
			render_sprites();
			*/
			
			gettimeofday(&time_end, (void*)NULL);
			int sleep_usec = (1000000 - comp_time(time_start, time_end) - 1)/60;
			//usleep(sleep_usec);
			
			gettimeofday(&time_start, (void*)NULL);

		}
		
		//Check for interrupts
		if(IME && interrupt_enable && hardware_registers[IF]) {
			u8 interrupt = interrupt_enable & hardware_registers[IF];
			if(interrupt & 0x1) { 
				// VBlank interrupt
				//disable interrupts
				IME = 0;
				hardware_registers[IF] &= ~0x1;
				//Push PC onto stack
				MBC_write(--SP.W, PC.B.H);
				MBC_write(--SP.W, PC.B.L);
				PC.W = 0x40;
				cycles += 16;
			} else if(interrupt & 0x2) {
				//LCD interrupt
				IME = 0;
				hardware_registers[IF] &= ~0x2;
				MBC_write(--SP.W, PC.B.H);
				MBC_write(--SP.W, PC.B.L);
				PC.W = 0x48;
				cycles += 16;
			} else if(interrupt & 0x4) {
				//Timer interrupt
				IME = 0;
				hardware_registers[IF] &= ~0x4;
				MBC_write(--SP.W, PC.B.H);
				MBC_write(--SP.W, PC.B.L);
				PC.W = 0x50;
				cycles += 16;
			} else if(interrupt & 0x8) {
				//Joypad Interrupt
				IME = 0;
				hardware_registers[IF] &= ~0x8;
				MBC_write(--SP.W, PC.B.H);
				MBC_write(--SP.W, PC.B.L);
				PC.W = 0x60;
				cycles += 16;
			}
		}	
	}
}


int execute()
{	
	// load the next intruction
	IR.W = MBC_read(PC.W++);	
	
	// execute the instruction and return cpu cycles
	switch(IR.W) {
		#include "opcodes.h"
	}
	
	printf("Program should not reach this point!\n");
	getchar();
	exit(1);
}