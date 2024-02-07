cmake_minimum_required(VERSION 3.10.0)
project(gtglfw VERSION 0.1.0 LANGUAGES C)

aux_source_directory(src SRC_FILES)

add_library(gtglfw ${SRC_FILES})
target_include_directories(gtglfw PUBLIC include)

add_subdirectory(utils/hc)