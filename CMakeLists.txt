cmake_minimum_required(VERSION 3.21)
project(Todoplusplus)

set(CMAKE_CXX_STANDARD 17)

add_executable(Application "Main.cpp" "Event.hpp" "Helpers.cpp")
