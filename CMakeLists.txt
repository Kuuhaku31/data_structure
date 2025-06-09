cmake_minimum_required(VERSION 3.10)

project(DataStructure)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)

include_directories("./include")

set(SOURCES
    "./src/algraph.cpp"
    "./src/main.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
