cmake_minimum_required(VERSION 2.8)
project(cgbemu)

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

MESSAGE("OUTPUT: ${EXECUTABLE_OUTPUT_PATH}")

add_subdirectory(src)