cmake_minimum_required(VERSION 3.15)
project(FinalProjExamples)

# filesystem.h require C++17
set(CMAKE_CXX_STANDARD 17)

file(COPY sample_data/ DESTINATION sample_data/)

# show compiler output and enable warnings
set(CMAKE_VERBOSE_MAKEFILE ON)
add_compile_options(-Wall -Wextra -pedantic)

add_executable(supersearch main.cpp)

add_executable(tests_AVL_Tree test_AVL_Tree.cpp)
add_executable(tests_Index_Handler test_Index_Handler.cpp)
add_executable(tests_Query_Processor test_Query_Processor.cpp)

set(CMAKE_VERBOSE_MAKEFILE OFF)
