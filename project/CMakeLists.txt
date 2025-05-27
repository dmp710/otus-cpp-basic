cmake_minimum_required(VERSION 3.14)
project(WorkScheduler)

set(CMAKE_CXX_STANDARD 17)

# Добавляем исполняемый файл
add_executable(work_scheduler  
    main.cpp
    WorkScheduler.h
    WorkScheduler.cpp
    WorkScheduler_menu.cpp
    WorkScheduler_auth.cpp
)

# Ищем OpenSSL
find_package(OpenSSL REQUIRED)

# Если вручную устанавливали pqxx
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

# Подключаем библиотеки один раз
target_link_libraries(work_scheduler
    pqxx
    pq
    OpenSSL::Crypto
)
