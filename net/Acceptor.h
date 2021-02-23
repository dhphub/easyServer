//
// Created by onirii on 2021/2/23.
//

#ifndef EASYSERVER_ACCEPTOR_H
#define EASYSERVER_ACCEPTOR_H

#include "net/Socket.h"
#include "net/Channel.h"
#include <functional>

namespace es {

class EventLoop;

class Acceptor {
public:
  typedef std::function<void(int sockfd, const InetAddress&)> NewConnectionCallback;
  Acceptor(EventLoop *loop, const InetAddress& listenAddr);
  void setNewConnectionCallback(const NewConnectionCallback& cb);
  bool listening() const { return listening_; }
  void listen();
private:
  void handleRead();
  EventLoop *loop_;
  Socket acceptSocket_;
  Channel acceptChannel_;
  bool listening_;
  NewConnectionCallback newConnectionCallback_;
};

}

#endif //EASYSERVER_ACCEPTOR_H
