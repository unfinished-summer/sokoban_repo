cmake_minimum_required(VERSION 3.10)

project(Sokoban LANGUAGES CXX)

# C++ 标准（代码用到 thread/chrono，C++11 起；与其他项目统一用 20）
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源码含中文注释（UTF-8）：MSVC 下统一按 UTF-8 解读，避免乱码
if(MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(sokoban sokoban.cpp levels.h levels.cpp)
