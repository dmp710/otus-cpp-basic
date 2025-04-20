cmake_minimum_required(VERSION 3.5)

project(08.homework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR})  # Добавляем путь к заголовочным файлам

include(FetchContent)

# Загружаем zlib через FetchContent
FetchContent_Declare(
  zlib
  URL https://zlib.net/zlib-1.3.1.tar.gz
)

FetchContent_MakeAvailable(zlib)

add_executable(08.homework
    src/main.cpp 
    src/threads_benchmark.cpp
    src/threads_benchmark.hpp
    src/worker.cpp
    src/worker.hpp
)

# куда линкуемся
target_link_libraries(08.homework PRIVATE zlibstatic)
# заголовочные файлы
target_include_directories(08.homework PRIVATE ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})

# =======================
# Google Test + Unit Test
# =======================

# GTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

# Тесты
add_executable(worker_test
    tests/test_worker.cpp
    src/worker.cpp
)

target_link_libraries(worker_test
    PRIVATE
    gtest_main
    zlibstatic
)

target_include_directories(worker_test PRIVATE
    ${zlib_SOURCE_DIR}
    ${zlib_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

include(GoogleTest)
gtest_discover_tests(worker_test)