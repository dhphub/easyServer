cmake_minimum_required(VERSION 3.8)
project(easyServer)

add_definitions(-std=c++11)

add_subdirectory(base)
add_subdirectory(net)
add_subdirectory(test)
# add_subdirectory(examples)