cmake_minimum_required(VERSION 2.8)
#project(cgbemu)


find_package (PkgConfig REQUIRED)
find_package (SDL REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0)

include_directories (
	${GTK2_INCLUDE_DIRS}
	${SDL_INCLUDE_DIR}
	${X11_INCLUDE_DIR}
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

if(UNIX)
	find_package (X11 REQUIRED)
	include_directories (${X11_INCLUDE_DIR})	
	target_link_libraries(cgbemu ${X11_LIBRARIES})
endif(UNIX)

target_link_libraries(cgbemu ${GTK2_LIBRARIES} ${SDL_LIBRARY} ${X11_LIBRARIES} cgb src)
