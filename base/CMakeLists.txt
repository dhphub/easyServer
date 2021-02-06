set(BASE_SRC noncopyable.h
        thread.h
        thread.cpp
        CurrentThread.h
        ../net/Channel.h
        ../net/EventLoop.h
        ../net/Poller.h)

add_library(base ${BASE_SRC})
target_link_libraries(base pthread)