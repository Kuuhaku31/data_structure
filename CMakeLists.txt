cmake_minimum_required(VERSION 3.10)

project(DataStructure)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 11)

# 设置 bin 目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin/")

include_directories("./include")

set(SOURCES
    "./src/algraph.cpp"
    "./src/main.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})

# dev
project(dev)
add_executable(${PROJECT_NAME} "./ignore/main.cpp")

# puzzle
project(puzzle)
include_directories("./puzzle")
add_executable(${PROJECT_NAME}

    "./puzzle/main.cpp"
    "./puzzle/puzzle.cpp"

    # "./ignore/puzzle.cpp"
)

# hash
project(hash)
add_executable(${PROJECT_NAME}

    "./ignore/main.cpp"
    "./src/linkhash.cpp"
)

# dij
project(dij)
add_executable(${PROJECT_NAME}
    "./src/mgraph.cpp"
    "./src/sqstack.cpp"
    "./src/sqqueue.cpp"
    "./src/algorithm/dij.cpp"
)
