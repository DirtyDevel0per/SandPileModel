cmake_minimum_required(VERSION 3.20)

project(
        labwork3-DirtyDevel0per
        LANGUAGES CXX
)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(bin)
add_subdirectory(lib)
add_subdirectory(parser)