# 1. Minimum CMake Version (Standard for modern environments)
cmake_minimum_required(VERSION 3.20)

# 2. Project Name and Language
project(LimitOrderBook CXX)

# 3. Language Standard (Using C++20 for modern features and filesystem support)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 4. Include Directories (Tells the compiler where to find .hpp files)
include_directories(include)

# 5. Executable Definition
# Defines the output binary name and the source files to compile
add_executable(MotorHFT 
    src/main.cpp
    src/OrderBook.cpp
)