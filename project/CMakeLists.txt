cmake_minimum_required(VERSION 3.14)
project(WorkScheduler)

set(CMAKE_CXX_STANDARD 17)

# === OpenSSL и pqxx ===
find_package(OpenSSL REQUIRED)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# === Основной исполняемый файл ===
add_executable(work_scheduler  
    main.cpp
    WorkScheduler.h
    WorkScheduler.cpp
    WorkScheduler_menu.cpp
    WorkScheduler_auth.cpp
    utils.h
)

target_link_libraries(work_scheduler
    pqxx
    pq
    OpenSSL::Crypto
)

# === GoogleTest через FetchContent ===
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        6910c9d   # release-1.16.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# === Тесты ===
add_executable(test_utils
    utils_tests.cpp
    utils.h
)

target_link_libraries(test_utils
    GTest::gtest_main
    OpenSSL::Crypto
)

include(GoogleTest)
gtest_discover_tests(test_utils)