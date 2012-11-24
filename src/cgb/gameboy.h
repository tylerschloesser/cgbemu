#ifndef GAMEBOY_H_
#define GAMEBOY_H_

void gameboy_power_on();
void gameboy_load_cartridge(char* cartridge_filepath);
void gameboy_load_bios(char *bios_filepath);

#endif // GAMEBOY_H_