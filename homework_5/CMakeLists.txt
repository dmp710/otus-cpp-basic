cmake_minimum_required(VERSION 3.5)

project(05.homework)

add_executable(05.homework
    statistics statistics.cpp
)

set_target_properties(05.homework PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
