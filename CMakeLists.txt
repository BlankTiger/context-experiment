cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_COMPILER clang++)

project(arena LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cc" "src/**/*.cc")

add_executable(main ${SOURCES})
