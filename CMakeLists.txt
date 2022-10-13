cmake_minimum_required(VERSION 2.8.12)

project(calculator)
set(CMAKE_CXX_STANDARD 20)
include_directories(include)

set(SOURCE 
    src/lexer.cpp
    src/parser.cpp
    src/main.cpp
)
add_executable(calculator ${SOURCE})