
# 创建静态库
add_library(stream_copy STATIC bidirectional_stream_copy.cc) # create static library named stream_copy.

# 定义宏
macro(add_app exec_name) # define a macro
    add_executable("${exec_name}" "${exec_name}.cc")
endmacro(add_app)

# 使用宏
add_app(webget)

