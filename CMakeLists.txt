cmake_minimum_required(VERSION 3.5)
project(PRoz_Portal)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp common.cpp communicator.cpp globals.h communicator.h mainProgram.cpp mainProgram.h receiver.cpp receiver.h utils.cpp utils.h portal.cpp portal.h)
add_executable(PRoz_Portal ${SOURCE_FILES})