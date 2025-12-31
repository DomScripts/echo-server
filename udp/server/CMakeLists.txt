set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 4.2.1)
project(server VERSION 1.0 LANGUAGES CXX)

add_library(fraction INTERFACE)
target_include_directories(fraction INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/../include
)

add_executable(server src/main.cpp)

target_link_libraries(server PRIVATE fraction)
