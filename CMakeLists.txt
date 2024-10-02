
cmake_minimum_required(VERSION 3.13)
project(langaje)
FILE(GLOB_RECURSE SOURCE_FILES *.c)
add_executable(langaje ${SOURCE_FILES})