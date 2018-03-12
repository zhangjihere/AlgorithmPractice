cmake_minimum_required(VERSION 3.0)
project(MaximumSubarray)

set(src MaximumSubarray.cpp)

file(COPY input.txt DESTINATION ${PROJECT_BINARY_DIR})
add_executable(${PROJECT_NAME} ${src})
