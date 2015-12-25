cmake_minimum_required(VERSION 2.6)
project(rubik)

add_executable(rubik rubik_init.cpp repl_autocomp.cpp repl.cpp rubik_parser.cpp rubik_server.cpp auxiliary_merge.cpp rubik_print.cpp lift.cpp rubik_alg.cpp rubik_ui.cpp auxiliary.cpp rubik.cpp cube_topology.cpp main.cpp)

install(TARGETS rubik RUNTIME DESTINATION bin)
target_link_libraries(rubik "-lncurses")