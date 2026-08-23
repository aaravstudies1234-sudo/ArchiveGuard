cmake_minimum_required(VERSION 3.20)

project(ArchiveGuard)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ArchiveGuard
        main.cpp
        policy.hpp
        policy.cpp
)

find_library(LIBZIP_LIBRARY
        NAMES zip
        REQUIRED
)

target_link_libraries(ArchiveGuard
        PRIVATE
        ${LIBZIP_LIBRARY}
)

target_compile_options(ArchiveGuard
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)