cmake_minimum_required(VERSION 3.27)
project(Task06)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(Task06 src/main.cpp
        include/hash_set.hpp
)
