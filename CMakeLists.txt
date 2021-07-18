cmake_minimum_required(VERSION 3.16)
project(Playground CXX)

add_executable(Playground WIN32 main.cpp)
target_link_libraries(Playground WINMM.LIB)