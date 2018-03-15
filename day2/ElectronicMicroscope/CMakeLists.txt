cmake_minimum_required(VERSION 3.0)
project(ElectronicMicroscope)

set(src main.cpp
    user.cpp)

file(COPY sample_input.txt DESTINATION ${PROJECT_BINARY_DIR})
file(COPY my_input.txt DESTINATION ${PROJECT_BINARY_DIR})
add_executable(${PROJECT_NAME} ${src})
