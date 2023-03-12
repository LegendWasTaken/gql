cmake_minimum_required(VERSION 3.23)
project(gql)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)

add_library(gql gql/window.cpp)
target_include_directories(gql PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(gql PUBLIC ImGui glad fmt)

add_executable(example_basic_window example/basic_window.cpp)
target_link_libraries(example_basic_window PRIVATE gql)

add_executable(example_keyboard_input example/keyboard_input.cpp)
target_link_libraries(example_keyboard_input PRIVATE gql)

add_executable(example_mouse_input example/mouse_input.cpp)
target_link_libraries(example_mouse_input PRIVATE gql)

add_executable(example_imgui_non_docking example/imgui_non_docking.cpp)
target_link_libraries(example_imgui_non_docking PRIVATE gql)

add_executable(example_hello_triangle example/hello_triangle.cpp)
target_link_libraries(example_hello_triangle PRIVATE gql)

