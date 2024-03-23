cmake_minimum_required(VERSION 3.22.1)

project(tcp_ip_cpp)


# 设置 C++ 标准为 C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
message("CMake runtime output dir:   " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# 开启调试模式
set(CMAKE_BUILD_TYPE "Debug")
message("CMake built type is: " ${CMAKE_BUILD_TYPE} " -> U can debug the exec-file in build/bin directly.")

# 设置头文件目录
include_directories("${PROJECT_SOURCE_DIR}/util")
include_directories("${PROJECT_SOURCE_DIR}/src")

# 子编译目录
add_subdirectory("${PROJECT_SOURCE_DIR}/apps")
add_subdirectory("${PROJECT_SOURCE_DIR}/test")