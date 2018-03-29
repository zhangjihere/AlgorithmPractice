cmake_minimum_required(VERSION 3.0)
project(AccountOperation)

set(src main.cpp
    user.cpp)
#set(src main.cpp
#    user_another_solution.cpp)

file(COPY sample_input.txt DESTINATION ${PROJECT_BINARY_DIR})
file(COPY my_input.txt DESTINATION ${PROJECT_BINARY_DIR})
add_executable(${PROJECT_NAME} ${src})
