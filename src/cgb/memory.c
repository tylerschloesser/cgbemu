#include "memory.h"

#include "joypad.h"

extern bool show_opcodes;

void MBC_write(u16 location, u8 data)
{

	if(location == 0xD840) {
//		printf("Updating pallete 0? PC:%X, HL:%X, data:%X\n", PC.W, HL.W, data);
		//display_cpu_values();
	}
	
	

	//#ifdef DEBUG_TIMER
	/*
		switch(location) {
			case 0xFF05:
				printf("HI");
				break;
			case 0xFF06:
				printf("HI");
				break;
			case 0xFF07:
				fprintf(stderr, "Timer control set to %d\n", data);
				break;
		}
	//#endif
	*/
	
    if(location < 0x2000) {

        //0000-1FFF
        //external RAM bank enable

        mbc_control[RAM_ENABLE] = data;

        #ifdef DEBUG_MEMORY
            printf("WRITE:RAM_ENABLE=%02X\n", data);
        #endif

    } else if(location < 0x3000) {
        //2000-2FFF
        //ROM bank select LSB

        mbc_control[ROM_BANK_LOW] = data;

        #ifdef DEBUG_MEMORY
			printf("WRITE:ROM_BANK_LOW=%02X\n", data);
		#endif

    } else if(location < 0x4000) {
        //3000-3FFF
        //ROM bank select MSB
        mbc_control[ROM_BANK_HIGH] = data;

        #ifdef DEBUG_MEMORY
            printf("WRITE:ROM_BANK_HIGH=%02X\n", data);
        #endif
    } else if(location < 0x6000) {
        //4000-5FFF
        //external RAM bank select

        mbc_control[RAM_BANK] = data;

        #ifdef DEBUG_MEMORY
            printf("WRITE:RAM_BANK=%02X\n", data);
        #endif
    } else if(location < 0x8000) {
        //6000-7FFF
        //ram/rom select (mbc1) ?
		
		//fprintf(stderr, "This is not implemented\n");
		fprintf(stderr, "RAM/ROM select? Valid only on MBC1\n");
		
		//getchar();
		return;
		
    } else if(location < 0xA000) {
		//8000-9FFF
		//VRAM
       
		u16 vram_bank = hardware_registers[VBK];
		
		if(vram_bank > 1) {
			fprintf(stderr, "READ:INVALID_VRAM_BANK=%02X\n", vram_bank);
			return;
		}
		
		u32 offset = 0x8000 - (vram_bank * 0x2000);
		u32 vram_location = location - offset;
		gameboy_vram[vram_location] = data;

	} else if(location < 0xC000) {
	
		//A000-BFFF
		//Switchable Cartridge RAM
		
		u8 ram_bank = mbc_control[RAM_BANK];
		u32 ram_location = ram_bank * 0x2000;
		cartridge_ram[ram_location] = data;
		
		#ifdef DEBUG_MEMORY
			printf("WRITE:CARTRAM%i[%04X]=%02X\n", (int)ram_bank, location, data); 
        #endif

	} else if(location < 0xD000) {
		//C000-CFFF
		//Internal RAM bank 0
		
		int offset = 0xC000;
		int ram_location = location - offset;
		gameboy_ram[ram_location] = data;
		
		#ifdef DEBUG_MEMORY
			printf("WRITE:GBRAM0[%04X]=%02X\n", location, data); 
        #endif
		
	} else if(location < 0xE000) {
		//D000-DFFF
		//Internal RAM bank 1-7
		
		int offset = 0xD000;
		u8 ram_bank = hardware_registers[SVBK];
		u32 ram_location = ram_bank * 0x2000 + location - offset;
		gameboy_ram[ram_location] = data;
		
		#ifdef DEBUG_MEMORY
			printf("WRITE:GBRAM%i[%04X]=%02X\n",(int)ram_bank, location, data); 
        #endif
		
    } else if(location < 0x10000) {
        //E000-FFFF
        if(location < 0xFE00) {
            //E000-FDFF
            //echo RAM (not usable)

            fprintf(stderr, "WRITE:INVALID_LOCATION=%04X\n", location);
			getchar();
			exit(1);
			return;

        } else if(location < 0xFEA0) {
		
            //FE00-FE9F
            //oject attribute memory

            int offset = 0xFE00;
            gameboy_oam[location - offset] = data;

            #ifdef DEBUG_MEMORY
				printf("WRITE:OAM[%02X]=%02X\n", location - offset, data);
            #endif

        } else if (location < 0xFF00) {
		
            //unusable memory
			
            fprintf(stderr, "WRITE:INVALID_LOCATION=%04X\n", location);
			getchar();
			return;

        } else if(location < 0xFF80)  {

            //FF00-FF7F
            //hardware registers

            int offset = 0xFF00;
			
			switch(location - offset) {
                case P1:
                {
                    data &= 0x30;
                    //assert(data != 0);
                    
                    hardware_registers[P1] = data;
                    if((data & 0x10) == 0) {
                        gb_select_direction_keys();
                    } else { // data & 0x20
                        gb_select_button_keys();
                    }
                    break;
                }
				case HDMA5:
				{
					u32 dma_source, dma_destination;
					u8 *dma_registers = &hardware_registers[HDMA1];
					dma_source = (*dma_registers) << 8;
					dma_source |= (*(++dma_registers));
					dma_destination = (*(++dma_registers)) << 8;
					dma_destination |= (*(++dma_registers));
					
					u32 dma_transfer_length = ((data & 0x7F) + 1) * 0x10;
					
					printf("DMA Transfer: source=%X destination=%X length:%d Byte(s)\n", dma_source, dma_destination, dma_transfer_length);
					
					
					
					int i;
					for(i = 0; i < dma_transfer_length; ++i) {
							MBC_write(dma_destination + i, MBC_read(dma_source + i));
					}
					hardware_registers[HDMA5] |= 0x80;
					return;
					
				}
				case DMA:
				{
					//printf("DMA transfer!\n");
					//DMA transfer
					int dma_source = data << 8;
					int i;
					for(i = 0; i < 0xA0; ++i) {
							MBC_write(0xFE00 + i, MBC_read(dma_source + i));
					}
					return;
				}
				case DIV:
					//attempting to  write to this registers will reset the value
					//http://www.codeslinger.co.uk/pages/projects/gameboy/timers.html
					hardware_registers[DIV] = 0;
					return;
				case LY:
					//writing to this register (current scnaline) resets the value
					hardware_registers[LY] = 0;
					return;
				case TAC:
				{
					//update timer control
					if((hardware_registers[TAC] & 0x3) != (data & 0x3)) {
						//frequency has changed

						switch(data & 0x3) {
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
					}
				}	
				case SVBK: 
					//printf("switching ram bank: %i\n", data);
					break;
				case BCPS:
					//printf("BCPS:%x\n", data);
					break;
				case BCPD:
				{
				
					//background pallete data
					int pallete_index = hardware_registers[BCPS] & 0x3F;
					if(pallete_index == 0) {
						//printf("Updating pallete 0. PC:%X HL:%X data:%X\n", PC.W, HL.W, data);
					}
					pallete[pallete_index] = data;
					if(hardware_registers[BCPS] & 0x80) {
						//Auto Increment
						++hardware_registers[BCPS];
					}
					hardware_registers[BCPS] &= 0xBF;
					//break; 5/15/2012
					return;
				}
				case BLCK:
					#ifdef DEBUG_MEMORY
						printf("WRITE:BIOS_DISABLED\n");
					#endif
					break;
			}
			
			hardware_registers[location - offset] = data;
			
			#ifdef DEBUG_MEMORY
				printf("WRITE:HR[%04X]=%02X\n", location, data);
            #endif
			
			//TEMP
			if(location - offset == BLCK) {
				printf("WRITE:BIOS_DISABLED\n");
				printf("Now booting cartridge...\n");
				//display_startup_values();
				//show_opcodes = true;
				//getchar();
			}

			
        } else if(location < 0xFFFF) {
            //FF80-FFFE
            //zero page

            int offset = 0xFF80;
            zero_page[location - offset] = data;

            #ifdef DEBUG_MEMORY
				printf("WRITE:ZP[%04X]=%02X\n", location, data);
            #endif

        } else if(location == 0xFFFF) {
            //FFFF
            //interrupt enable
			//bit	when 0		when 1
			// 0	vblank OFF	vblank ON
			// 1	LCDstat OFF LCD stat ON
			// 2	Timer OFF	Timer ON
			// 3	Serial OFF	Serial ON
			// 4	Joypad OFF	Joypad ON

            interrupt_enable = data;

            #ifdef DEBUG_MEMORY
                printf("WRITE:INTERRUPT_ENABLE=%02X\n", data);
            #endif
        } else {
            //not possible
        }

    } else {
        fprintf(stderr, "WRITE:INVALID_LOCATION=%04X\n", location);
		getchar();
    }
}

u8 MBC_read(u16 location)
{
	
    if(location < 0x4000) {

        //0000-3FFF
        //ROM Bank #0
		/*
        if(hardware_registers[BLCK] != 0x11 && IME) //check for IME, temp fix, interrupt handled by rom
        {
		*/
		if(hardware_registers[BLCK] != 0x11) {
            //BIOS is still active
            if(location < 0x100 || (location > 0x1FF && location < 0x900))
            {
                return bios[location];
            }
		}
		/*
        }
		*/
        return cartridge_rom[location];

    } else if(location < 0x8000) {

        //4000-7FFF
        //ROM Bank #1 to n

        u32 rom_bank = mbc_control[ROM_BANK_LOW];
        rom_bank |= mbc_control[ROM_BANK_HIGH] << 8;

		if(rom_bank == 0) {
			printf("ROM BANK is 0, should this be 1?");
			getchar();
		}
		
        u32 rom_location = (rom_bank * 0x4000) + (location - 0x4000);

		u8 data = cartridge_rom[rom_location];
        #ifdef DEBUG_MEMORY
			printf("READ:GBR0M%i[%04X]=%02X\n", (int)rom_bank, location, data); 
        #endif

        return data;

    } else if (location < 0xA000) {
        //VRAM 8000-9FFF

		u8 vram_bank = hardware_registers[VBK];
		
		if(vram_bank > 1) {
			fprintf(stderr, "READ:INVALID_VRAM_BANK=%02X\n", vram_bank);
			return 0;
		}
		
		u32 offset = 0x8000 - (vram_bank * 0x2000);
		u32 vram_location = location - offset;
		u8 data = gameboy_vram[vram_location];
		
		#ifdef DEBUG_MEMORY
			printf("READ:VRAM%i[%04X]=%02X\n", vram_bank, vram_location, data);
		#endif		
		
		return data;
		
    } else if (location < 0xC000) {

        //A000-BFFF
        //Switchable Cartridge RAM

		if(mbc_control[RAM_ENABLE] == 0) {
			fprintf(stderr, "READ:DISABLED_RAM\n");
			display_cpu_values();
			getchar();
			exit(0);
		}
		
        u8 ram_bank = mbc_control[RAM_BANK];
        u32 ram_location = ram_bank * 0x2000;
		u8 data = cartridge_ram[ram_location];
        #ifdef DEBUG_MEMORY
			printf("READ:CARTRAM%i[%04X]=%02X\n", (int)ram_bank, location, data); 
        #endif

        return data;

    } else if (location < 0xD000) {

        //C000-CFFF
        //Internal RAM bank 0

        int offset = 0xC000;
        int ram_location = location - offset;
		u8 data = gameboy_ram[ram_location];
        #ifdef DEBUG_MEMORY
			printf("READ:GBRAM0[%04X]=%02X\n", location, data); 
        #endif

        return data;

    } else if (location < 0xE000) {

        //D000-DFFF
        //Internal RAM bank 1-7

        int offset = 0xD000;
        u8 ram_bank = hardware_registers[SVBK];
        u32 ram_location = ram_bank * 0x2000 + location - offset;
		u8 data = gameboy_ram[ram_location];
        #ifdef DEBUG_MEMORY
			printf("READ:GBRAM%i[%04X]=%02X\n",(int)ram_bank, location, data); 
        #endif

        return data;

    } else if (location < 0x10000) {
        //E000-FFFF
        if(location < 0xFE00) {
            //E000-FDFF
            //Echo RAM (not usable)

            fprintf(stderr, "READ:INVALID_LOCATION=%04X\n", location);
			getchar();
			exit(0);
			return 0;

        } else if(location < 0xFEA0) {

            //FE00-FE9F
            //object attribute memory

            int offset = 0xFE00;

            #ifdef DEBUG_MEMORY
                printf("MBC: %02x read from oam_ram at %02x\n",
                       gameboy_oam[location - offset], location - offset);
            #endif

            return gameboy_oam[location - offset];

        } else if(location < 0xFF00) {
		
            //unusable memory

            fprintf(stderr, "READ:INVALID_LOCATION=%04X\n", location);
			return 0;

        } else if(location < 0xFF80) {
		
            //FF00-FF7F
            //hardware registers

            int offset = 0xFF00;

			switch(location - offset) {
				case P1: //joypad
					//temp. set 4LSB to 1, indicating that no buttons are pressed
					return (hardware_registers[P1] | 0xFF);
			}
			
            #ifdef DEBUG_MEMORY
                //printf("MBC: %02x read from hardware_registers %02x\n",
                       //hardware_registers[location - offset], location - offset);
            #endif

            return hardware_registers[location - offset];

        } else if(location < 0xFFFF) {
            //FF80-FFFE
            //zero page

            int offset = 0xFF80;

            #ifdef DEBUG_MEMORY
                printf("MBC: %02x read from zero_page %02x\n",
                       zero_page[location - offset], location - offset);
            #endif

            return zero_page[location - offset];

        } else if (location == 0xFFFF) {
            //FFFF
            //interrupt enable

            #ifdef DEBUG_MEMORY
				printf("READ:INTERRUPT_ENABLE=%02X\n", interrupt_enable);
            #endif

            return interrupt_enable;

        } else {
            //not possible
        }
    } else {
        fprintf(stderr, "READ:INVALID_LOCATION=%04X\n", location);
    }
	return 0;
}
