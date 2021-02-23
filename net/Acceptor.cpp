//
// Created by onirii on 2021/2/23.
//

#include "Acceptor.h"
#include "net/EventLoop.h"
#include "net/SocketsOps.h"

namespace es {

Acceptor::Acceptor(EventLoop *loop, const InetAddress &listenAddr) :
  loop_(loop),
  acceptSocket_(sockets::createNonblockingOrDie()),
  acceptChannel_(loop, acceptSocket_.fd()),
  listening_(false) {
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.bindAddress(listenAddr);
  acceptChannel_.enableReading();
  acceptChannel_.setReadCallBack(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::setNewConnectionCallback(const Acceptor::NewConnectionCallback &cb) {
  newConnectionCallback_ = cb;
}

void Acceptor::listen() {
  loop_->assertInLoopThread();
  listening_ = true;
  acceptSocket_.listen();
  // muduoÔÚÕâÀïenable reading
  // acceptChannel_.enableReading();
}

void Acceptor::handleRead() {
  loop_->assertInLoopThread();
  InetAddress peerAddr(0);
  int connfd = acceptSocket_.accept(&peerAddr);
  if (connfd >= 0) {
    if (newConnectionCallback_) {
      newConnectionCallback_(connfd, peerAddr);
    } else {
      sockets::close(connfd);
    }
  }
}

}