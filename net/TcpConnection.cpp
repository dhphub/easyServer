//
// Created by onirii on 2021/2/24.
//

#include "net/TcpConnection.h"
#include "net/SocketsOps.h"
#include "net/Socket.h"
#include "net/Channel.h"
#include "net/EventLoop.h"

namespace es {


TcpConnection::TcpConnection(EventLoop *loop,
                             const std::string &name,
                             int sockfd,
                             const InetAddress &localAddr,
                             const InetAddress &peerAddr)
  : loop_(loop),
  name_(name),
  state_(kConnecting),
  socket_(new Socket(sockfd)),
  channel_(new Channel(loop, sockfd)),
  localAddr_(localAddr),
  peerAddr_(peerAddr) {
  //channel_->enableReading();
  channel_->setReadCallBack(std::bind(&TcpConnection::handleRead, this));
}

TcpConnection::~TcpConnection() {
  //TODO log
}

void TcpConnection::connectEstablished() {
  loop_->assertInLoopThread();
  assert(state_ == kConnecting);
  setState(kConnected);
  //为什么在这里enableRead??
  channel_->enableReading();
  connectionCallbck_(shared_from_this());
}

void TcpConnection::handleRead() {
  char buf[1024];
  //先做io,在进行用户回调
  ssize_t n = ::read(socket_->fd(), buf, sizeof(buf));
  messageCallback_(shared_from_this(), buf, n);
}

}