set(NET_SRC
        Channel.cpp
        EventLoop.cpp
        Poller.cpp
        EventLoopThread.cpp
        EventLoopThread.h
        InetAddress.cpp
        InetAddress.h
        SocketsOps.cpp
        SocketsOps.h
        Socket.cpp
        Socket.h
        Acceptor.cpp
        Acceptor.h)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

add_library(es_net ${NET_SRC})
