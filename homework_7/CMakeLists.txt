cmake_minimum_required(VERSION 3.2)

project(GTestDownloadFetchContentCmake)

#download googletest sources at configure time to build directory
#build from sources at build time
include(FetchContent)
#describe how to get content
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
#it is advisable to use a hash for GIT_TAG rather than a branch or tag name.
#A commit hash is more secure and helps to confirm that the downloaded contents are what you expected.
  GIT_TAG        6910c9d   # release-1.16.0
)
#make sure that all the dependencies are available
FetchContent_MakeAvailable(googletest)

#no add_subdirectory to googletest requires!

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Main target
add_executable(
    containers
    src/main.cpp
)

target_include_directories(containers PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Test target
add_executable(
    containers_tests
    test/test_list.cpp
    test/test_array.cpp
)

# Подключаем GTest к test_list
target_link_libraries(
    containers_tests PRIVATE
    gtest_main
)

# Добавляем include-пути для gtest
target_include_directories(
    containers_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    ${GTest_INCLUDE_DIRS}
)

# Включаем Google Test
enable_testing()
add_test(NAME containers_tests COMMAND tests)

set_target_properties(containers PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
set_target_properties(containers_tests PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

if (MSVC)
    target_compile_options(
        containers PRIVATE
        /W4
    )
    target_compile_options(
        containers_tests PRIVATE
        /W4
    )
else ()
    target_compile_options(
        containers PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(
        containers_tests PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
