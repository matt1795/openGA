cmake_minimum_required(VERSION 2.8.12)
project(PackageTest CXX)

find_package(openga REQUIRED)
message(STATUS OPENGA: ${openga_INCLUDE_DIRS})

add_executable(example example.cpp)
target_link_libraries(example PRIVATE openga::openga)
