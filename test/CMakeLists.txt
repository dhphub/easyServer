add_executable(test_pid test_pid.cpp)
target_link_libraries(test_pid pthread)

add_executable(test_printf test_printf.cpp)
#target_link_libraries(test_printf rt)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(test1 test1.cpp)
target_link_libraries(test1 es_net es_base)

add_executable(test2 test2.cpp)
target_link_libraries(test2 es_net es_base)

add_executable(test3 test3.cpp)
target_link_libraries(test3 es_net es_base)