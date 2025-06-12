cmake_minimum_required(VERSION 3.14)
project(WorkScheduler)

set(CMAKE_CXX_STANDARD 17)

# === OpenSSL и pqxx ===
find_package(OpenSSL REQUIRED)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# === FetchContent для FTXUI ===
include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
  GIT_TAG v2.0.0 # или последняя стабильная версия
)

FetchContent_MakeAvailable(ftxui)

# === pqxx ===
find_path(PQXX_INCLUDE_DIR pqxx/pqxx)
find_library(PQXX_LIBRARY NAMES pqxx)

if(NOT PQXX_INCLUDE_DIR OR NOT PQXX_LIBRARY)
  message(FATAL_ERROR "pqxx не найден. Убедитесь, что он установлен.")
endif()

include_directories(${PQXX_INCLUDE_DIR})
link_libraries(${PQXX_LIBRARY})

# === Основной исполняемый файл ===
add_executable(work_scheduler
  main.cpp
  Database.h
  Postgres.h
  Postgres_auth.cpp
  Postgres_tasks.cpp
  WorkScheduler.h
  WorkScheduler.cpp
  WorkScheduler_menu.cpp
  WorkScheduler_draw.cpp
  utils.h
  utils_time.h
)

target_link_libraries(work_scheduler
  pqxx
  pq
  OpenSSL::Crypto
  ftxui::component
  ftxui::dom
)

# === GoogleTest через FetchContent ===
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 6910c9d # release-1.16.0
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


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-fsanitize=address -g -O1)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
else()
  target_compile_options(work_scheduler PRIVATE -O${OPT_LEVEL})
endif()
