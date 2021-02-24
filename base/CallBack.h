//
// Created by onirii on 2021/2/20.
//

#ifndef EASYSERVER_CALLBACK_H
#define EASYSERVER_CALLBACK_H

#include <functional>
#include <memory>

namespace es {

class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr ;

typedef std::function<void()> TimerCallBack;
typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallbck;
typedef std::function<void(const TcpConnectionPtr&, const char*, ssize_t)> MessageCallback;

}

#endif //EASYSERVER_CALLBACK_H
