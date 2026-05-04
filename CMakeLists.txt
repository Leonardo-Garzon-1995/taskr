cmake_minimum_required(VERSION 3.15)
project(taskr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(taskr
    src/main.cpp
    src/TaskManager.cpp
)
target_include_directories(taskr PRIVATE src)

add_executable(run_tests
    tests/test_taskmanager.cpp
    src/TaskManager.cpp
)
target_include_directories(run_tests PRIVATE src)