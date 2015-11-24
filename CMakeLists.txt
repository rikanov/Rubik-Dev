cmake_minimum_required(VERSION 2.6)
project(rubik)

add_executable(rubik rubik_alg.cpp rubik_ui.cpp auxiliary.cpp rubik.cpp cube_topology.cpp main.cpp)

install(TARGETS rubik RUNTIME DESTINATION bin)
