add_executable(test_pid test_pid.cpp)
target_link_libraries(test_pid pthread)

add_executable(test_printf test_printf.cpp)
#target_link_libraries(test_printf rt)