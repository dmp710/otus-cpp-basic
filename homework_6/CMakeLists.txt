cmake_minimum_required(VERSION 3.5)

project(06.homework)

include_directories(${CMAKE_SOURCE_DIR})  # Добавляем путь к заголовочным файлам

add_executable(06.homework
    main.cpp Array.hpp
)

set_target_properties(06.homework PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
