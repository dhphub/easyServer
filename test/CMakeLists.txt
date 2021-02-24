add_executable(test_pid test_pid.cpp)
target_link_libraries(test_pid pthread)

add_executable(test_printf test_printf.cpp)
#target_link_libraries(test_printf rt)

add_executable(test_cond test_cond.cpp)
target_link_libraries(test_cond pthread)

link_directories(${PROJECT_SOURCE_DIR}/lib)

add_executable(test1 test1.cpp)
target_link_libraries(test1 es_net es_base)

add_executable(test2 test2.cpp)
target_link_libraries(test2 es_net es_base)

add_executable(test3 test3.cpp)
target_link_libraries(test3 es_net es_base)

add_executable(test4 test4.cpp)
target_link_libraries(test4 es_net es_base)

add_executable(test5 test5.cpp)
target_link_libraries(test5 es_net es_base)

add_executable(test6 test6.cpp)
target_link_libraries(test6 es_net es_base)

add_executable(test7 test7.cpp)
target_link_libraries(test7 es_net es_base)

add_executable(test8 test8.cpp)
target_link_libraries(test8 es_net es_base)

add_executable(test_eventfd test_eventfd.cpp)
target_link_libraries(test_eventfd es_base)