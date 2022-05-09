cmake_minimum_required(VERSION 3.21)
project(WindowsProgramming)

set(CMAKE_CXX_STANDARD 17)

add_executable(WindowsProgramming WIN32 main.cpp)
target_link_libraries(WindowsProgramming WINMM.LIB)
