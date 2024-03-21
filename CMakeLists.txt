cmake_minimum_required(VERSION 3.22.1)

project(tcp_ip_cpp)


# 设置 C++ 标准为 C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置头文件目录
include_directories("${PROJECT_SOURCE_DIR}/util")
include_directories("${PROJECT_SOURCE_DIR}/src")

# 子编译目录
add_subdirectory("${PROJECT_SOURCE_DIR}/apps")