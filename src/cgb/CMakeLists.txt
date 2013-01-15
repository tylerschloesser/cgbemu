cmake_minimum_required(VERSION 2.8)
#project(cgbemu)

include_directories (
	${GTK2_INCLUDE_DIRS}
	${SDL_INCLUDE_DIR}
)

set(cgb_source_files 
	cpu.c 
	cpu.h
	gameboy.c 
	gameboy.h
	graphics.c 
	graphics.h
	joypad.c 
	joypad.h
	memory.c
	memory.h
	screen.c
	screen.h
	opcodes.h
)

add_library(cgb ${cgb_source_files})