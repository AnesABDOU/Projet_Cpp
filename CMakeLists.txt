cmake_minimum_required(VERSION 3.10)
project(MarioShab)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_PREFIX_PATH})

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

include_directories(include)

find_package(Raylib REQUIRED)

add_executable(MarioShab ${SOURCES} ${HEADERS}
        include/Enemy.h
        include/Score.h)
target_link_directories(MarioShab PRIVATE include)

target_link_libraries(MarioShab PRIVATE raylib)
