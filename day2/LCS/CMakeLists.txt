cmake_minimum_required(VERSION 3.0)
project(LCS)

set(src user.cpp)

file(COPY sample_input.txt DESTINATION ${PROJECT_BINARY_DIR})
add_executable(${PROJECT_NAME} ${src})
