set(target test)

# project name 
project(${target})

# include the head file in  this directory.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# using GLOB to collect a list of source files from temp dir.
file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_executable(${target} ${sources})

# link nothing
# target_link_libraries(${target})
