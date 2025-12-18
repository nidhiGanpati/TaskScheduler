cmake_minimum_required(VERSION 3.15)
project(TaskScheduler VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(LIBRARY_SOURCES
    src/Task.cpp
    src/TaskScheduler.cpp
    src/ThreadPool.cpp
)

add_library(TaskSchedulerLib STATIC ${LIBRARY_SOURCES})
add_executable(TaskSchedulerDemo examples/main.cpp)
target_link_libraries(TaskSchedulerDemo TaskSchedulerLib)