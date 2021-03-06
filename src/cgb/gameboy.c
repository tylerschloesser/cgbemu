#include "gameboy.h"
#include "cpu.h"
#include "joypad.h"
#include "memory.h"
#include "graphics.h"
#include "../tools.h"

#include <SDL.h>
#include <SDL_thread.h>


SDL_Surface *surface;

static void key_pressed(SDLKey key);
static void key_unpressed(SDLKey key);

bool gameboy_verify_cartridge() {
	
	char title[0x10];
	title[0x10 - 1] = '\0';
	int i;
	for(i = 0x134; i < 0x144; ++i) {
		title[i - 0x134] = cartridge_rom[i];
	}
	printf("Title: %s\n", title);
	printf("CartridgeType: %X\n", cartridge_rom[0x147]);
	printf("Rom Size: %X\n", cartridge_rom[0x148]);
	
	// calculate checksum
	u16 sum;
	for(i = 0; i < CARTRIDGE_ROM_SIZE; ++i) {
		sum += cartridge_rom[i];
	}
	sum -= (cartridge_rom[0x14e] + cartridge_rom[0x14f]);
	
	printf("Sum: %X\n", sum);
	
	printf("checksum: %X %X\n", cartridge_rom[0x14e], cartridge_rom[0x14f]);
	
	return true;
}

void gameboy_load_cartridge(char* cartridge_filepath) {
	dprintf("Opening %s...", cartridge_filepath);
	int bytes_read = binary_read_file(cartridge_filepath, cartridge_rom, CARTRIDGE_ROM_SIZE);
	if(bytes_read == 0) {
		fprintf(stderr, "Failed (%s)\n", get_last_error());
		fatal_error();
	}
	
	gameboy_verify_cartridge();
	
	dprintf("Done (%s)\n", size_to_string(bytes_read));
}

void gameboy_load_bios(char *bios_filepath) {
	dprintf("Opening %s...", bios_filepath);
    int bytes_read = binary_read_file(bios_filepath, bios, BIOS_SIZE);
	if(bytes_read == 0) {
		fprintf(stderr, "Failed (%s)\n", get_last_error());
		fatal_error();
	}
    dprintf("Done (%s)\n", size_to_string(bytes_read));
}


void gameboy_power_on() 
{

	initialize_cpu();
	initialize_memory();
	initialize_joypad();
	initialize_screen();
	initialize_graphics();
	
#ifdef _WIN32
	char game_filepath[] = "resources\\roms\\pokemon_yellow.gbc";
	//char game_filepath[] = "resources\\roms\\pokemon_red.gb";
	char bios_filepath[] = "resources\\bios\\gbc_bios.bin";
#else

	char game_filepath[] = "resources/roms/pokemon_yellow.gbc";
	//char game_filepath[] = "resources\\roms\\pokemon_red.gb";
	char bios_filepath[] = "resources/bios/gbc_bios.bin";
#endif
	
    gameboy_load_bios(bios_filepath);
    gameboy_load_cartridge(game_filepath);
	
	cpu_start();
}

void gameboy_power_off()
{
	cpu_stop();
}

//temp for debugging
void cpu_debug() {
	//display_cpu_values();
	//show_opcodes = true;
}

void gameboy_toggle_button( enum BUTTON button, bool pressed )
{
	// temporary to allow for the new interface
	int gb_key = -1;
	switch( button )
	{
		case BUTTON_UP:
			gb_key = JOYPAD_UP;
			break;
		case BUTTON_DOWN:
			gb_key = JOYPAD_DOWN;
			break;
		case BUTTON_LEFT:
			gb_key = JOYPAD_LEFT;
			break;
		case BUTTON_RIGHT:
			gb_key = JOYPAD_RIGHT;
			break;
		case BUTTON_START:
			gb_key = JOYPAD_START;
			break;
		case BUTTON_SELECT:
			gb_key = JOYPAD_SELECT;
			break;
		case BUTTON_A:
			gb_key = JOYPAD_A;
			break;
		case BUTTON_B:
			gb_key = JOYPAD_B;
			break;
	}
	
	if( gb_key != -1 )
	{
		if( pressed == true ) {
			joypad_down( gb_key );
		} else {
			joypad_up( gb_key );
		}
	}
}

void gameboy_save_state( char* save_state_filepath )
{
	FILE* file = fopen( save_state_filepath, "wb+" );
	
	if( file == NULL ) {
		fprintf( stderr, "unable to open file for save state: %s\n", save_state_filepath );
		return;
	}
	
	cpu_pause();
	
	CpuState state = get_cpu_state();
	int state_size = sizeof( CpuState );
	
	fprintf(stdout,"Saving state...\n");
	//cpu_print_state();
	
	u8* state_raw = (u8*)&state;
	
	fwrite( state_raw, sizeof( state_raw[0] ), state_size, file );
	fwrite( cartridge_ram, sizeof( cartridge_ram[0] ), CARTRIDGE_RAM_SIZE, file );
	fwrite( cartridge_rom, sizeof( cartridge_rom[0] ), CARTRIDGE_ROM_SIZE, file );
	fwrite( pallete, sizeof( pallete[0] ), 0x40, file );
	fwrite( sprite_pallete, sizeof( sprite_pallete[0] ), 0x40, file );
	fwrite( gameboy_ram, sizeof( gameboy_ram[0] ), GAMEBOY_RAM_SIZE, file );
	fwrite( gameboy_vram, sizeof( gameboy_vram[0] ), GAMEBOY_VRAM_SIZE, file );
	fwrite( gameboy_oam, sizeof( gameboy_oam[0] ), GAMEBOY_OAM_SIZE, file );
	fwrite( bios, sizeof( bios[0] ), BIOS_SIZE, file );
	fwrite( zero_page, sizeof( zero_page[0] ), 0x7F, file );
	fwrite( &interrupt_enable, sizeof( interrupt_enable ), 1, file );
	fwrite( hardware_registers, sizeof( hardware_registers[0] ), 0x80, file );
	fwrite( &IME, sizeof( IME ), 1, file );
	fwrite( mbc_control, sizeof( mbc_control[0] ), 4, file );
	
	fclose( file );
	
	cpu_start();
}

void gameboy_load_state( char* save_state_filepath )
{
	FILE* file = fopen( save_state_filepath, "rb" );
	
	if( file == NULL ) {
		fprintf( stderr, "unable to open file for save state: %s\n", save_state_filepath );
		return;
	}
	
	cpu_pause();
	
	reinitialize_cpu();
	reinitialize_memory();
	reinitialize_joypad();
	reinitialize_screen();
	reinitialize_graphics();
	
	CpuState state;
	int state_size = sizeof( CpuState );
	u8* state_raw = (u8*)&state;
	
	fread( state_raw, sizeof( state_raw[0] ), state_size, file );
	fread( cartridge_ram, sizeof( cartridge_ram[0] ), CARTRIDGE_RAM_SIZE, file );
	fread( cartridge_rom, sizeof( cartridge_rom[0] ), CARTRIDGE_ROM_SIZE, file );
	fread( pallete, sizeof( pallete[0] ), 0x40, file );
	fread( sprite_pallete, sizeof( sprite_pallete[0] ), 0x40, file );
	fread( gameboy_ram, sizeof( gameboy_ram[0] ), GAMEBOY_RAM_SIZE, file );
	fread( gameboy_vram, sizeof( gameboy_vram[0] ), GAMEBOY_VRAM_SIZE, file );
	fread( gameboy_oam, sizeof( gameboy_oam[0] ), GAMEBOY_OAM_SIZE, file );
	fread( bios, sizeof( bios[0] ), BIOS_SIZE, file );
	fread( zero_page, sizeof( zero_page[0] ), 0x7F, file );
	fread( &interrupt_enable, sizeof( interrupt_enable ), 1, file );
	fread( hardware_registers, sizeof( hardware_registers[0] ), 0x80, file );
	fread( &IME, sizeof( IME ), 1, file );
	fread( mbc_control, sizeof( mbc_control[0] ), 4, file );
	
	fclose( file );
	
	set_cpu_state( state );
	
	fprintf(stdout,"Loading state...\n");
	//cpu_print_state();
	
	cpu_start();
}
