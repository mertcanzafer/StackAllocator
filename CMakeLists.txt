cmake_minimum_required(VERSION 3.12)
project(StackAllocator)

# Set Language standards
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    main.cpp
    StackAllocator.cpp
)

set(HEADERS
    StackAllocator.h
    Defines.h
)

add_executable(StackAllocator ${SOURCES} ${HEADERS})