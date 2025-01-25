cmake_minimum_required(VERSION 3.12)

project(gues_number)

add_executable(gues_number
    main.cpp
)

set_target_properties(gues_number PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)