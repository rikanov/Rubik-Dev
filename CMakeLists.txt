cmake_minimum_required(VERSION 2.6)
project(rubik)

add_executable(rubik side_marks.cpp rubik3d_visualization.cpp rubik3d.cpp global_variables.cpp def_colors.cpp _intro_screen.cpp rotation_range.cpp editor.cpp editor_buffer.cpp rubik_heuristic_cluster.cpp rubik_heuristic_start.cpp rubik_engines.cpp rotation_groups.cpp rubik_heuristic.cpp teszt.lisp rubik_init.cpp repl_autocomp.cpp repl.cpp rubik_parser.cpp rubik_server.cpp auxiliary_merge.cpp rubik_print.cpp rubik_alg.cpp rubik_ui.cpp auxiliary.cpp rubik.cpp cube_topology.cpp main.cpp)

install(TARGETS rubik RUNTIME DESTINATION bin)
target_link_libraries(rubik "-lncurses")
