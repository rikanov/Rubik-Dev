cmake_minimum_required(VERSION 2.6)
project(rubik)

add_executable(rubik main.cpp)

install(TARGETS rubik RUNTIME DESTINATION bin)
