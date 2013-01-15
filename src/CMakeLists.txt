cmake_minimum_required(VERSION 2.8)
#project(cgbemu)


set(cgbemu_source_files 
	debug.c 
	debug.h
	tools.c
	tools.h
	globals.h
)


#add_executable(cgbemu ${cgbemu_source_files})

add_library(src ${cgbemu_source_files})

add_subdirectory(cgb)
add_subdirectory(gui)
