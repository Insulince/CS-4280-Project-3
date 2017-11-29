cmake_minimum_required(VERSION 3.8)
project(P3)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp node.cpp node.h parser.cpp parser.h scanner.cpp scanner.h token.cpp token.h util.cpp util.h)
add_executable(comp ${SOURCE_FILES})