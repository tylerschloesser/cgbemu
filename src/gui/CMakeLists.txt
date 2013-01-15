cmake_minimum_required(VERSION 2.8)
#project(cgbemu)


find_package (PkgConfig REQUIRED)
find_package (SDL REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0)

include_directories (
	${GTK2_INCLUDE_DIRS}
	${SDL_INCLUDE_DIR}
)
link_directories (
	${GTK2_LIBRARY_DIRS}
)

add_definitions (${GTK2_CFLAGS_OTHER})


set(gui_source_files 
	gtksdl.c 
	gtksdl.h
	main.c
)

#add_library(gui ${gui_source_files})

add_executable(cgbemu ${gui_source_files})

target_link_libraries(cgbemu ${GTK2_LIBRARIES} ${SDL_LIBRARY} cgb src)
