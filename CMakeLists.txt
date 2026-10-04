cmake_minimum_required(VERSION 3.10)

project(Sokoban LANGUAGES CXX)

# C++ 标准（代码用到 thread/chrono，C++11 起；与其他项目统一用 20）
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 源码含中文注释（UTF-8）：MSVC 下统一按 UTF-8 解读，避免乱码
if(MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(sokoban sokoban.cpp level_loader.cpp level_loader.h)

# 拉取 nlohmann-json（单头文件 JSON 库，header-only，无需编译）
include(FetchContent)
FetchContent_Declare(nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(sokoban PRIVATE nlohmann_json::nlohmann_json)

# 每次编译后自动把 levels 拷到 exe 旁边（复刻 minesweeper 的 res 拷贝）
add_custom_command(
    TARGET sokoban POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/levels"
        "$<TARGET_FILE_DIR:sokoban>/levels"
)
