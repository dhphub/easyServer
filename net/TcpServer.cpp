//
// Created by onirii on 2021/2/24.
//

#include "TcpServer.h"
#include "net/Acceptor.h"
#include "net/SocketsOps.h"

namespace es {

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr) :
  loop_(loop),
  name_(listenAddr.toHostPort()),
  acceptor_(new Acceptor(loop, listenAddr)),
  started_(false),
  nextConnId_(1) {
  acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this,
          std::placeholders::_1,
          std::placeholders::_2));
}

TcpServer::~TcpServer() { }

void TcpServer::start() {
  if (!started_) {
    started_ = true;
  }
  if (!acceptor_->listening()) {
    loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
  }
}

void TcpServer::newConnection(int sockfd, const InetAddress &peerAddr) {
  loop_->assertInLoopThread();
  char buf[32] = {0};
  snprintf(buf, sizeof(buf), "_%d", nextConnId_);
  ++nextConnId_;
  std::string connName = name_ + buf;
  InetAddress localAddr(sockets::getLocalAddr(sockfd));
  TcpConnectionPtr conn(new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));
  connectionMap_[connName] = conn;
  conn->setConnectionCallback(connectionCallbck_);
  conn->setMessageCallback(messageCallback_);
  conn->connectEstablished();
}

}