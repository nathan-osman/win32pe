cmake_minimum_required(VERSION 3.2.0 FATAL_ERROR)
project(win32pe)

set(PROJECT_NAME "win32pe")

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

find_package(Boost 1.58 REQUIRED)

add_subdirectory(win32pe)

option(BUILD_TESTS "Build test suite" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
