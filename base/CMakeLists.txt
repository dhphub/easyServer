set(BASE_SRC noncopyable.h
        thread.h
        thread.cpp
        CurrentThread.h
        Timestamp.cpp
        Timestamp.h
        Timer.cpp
        Timer.h
        TimerId.h
        TimerQueue.cpp
        TimerQueue.h
        CallBack.h
        Mutex.cpp
        Mutex.h
        Condition.cpp
        Condition.h
        copyable.h)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_library(es_base ${BASE_SRC})
target_link_libraries(es_base pthread)