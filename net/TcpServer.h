//
// Created by onirii on 2021/2/24.
//

#ifndef EASYSERVER_TCPSERVER_H
#define EASYSERVER_TCPSERVER_H

#include "base/noncopyable.h"
#include "net/TcpConnection.h"
#include "base/CallBack.h"
#include <map>

namespace es {

class Acceptor;

class TcpServer : noncopyable {
public:
  TcpServer(EventLoop* loop, const InetAddress& listenAddr);
  ~TcpServer();
  void start();
  void setConnectionCallback(const ConnectionCallbck& cb) { connectionCallbck_ = cb; }
  void setMessageCallback(const MessageCallback & cb) { messageCallback_ = cb; }
private:
  void newConnection(int sockfd, const InetAddress& peerAddr);
  typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;
  
  EventLoop *loop_;
  std::string name_;
  bool started_;
  std::shared_ptr<Acceptor> acceptor_;
  int nextConnId_;
  ConnectionCallbck connectionCallbck_;
  MessageCallback messageCallback_;
  ConnectionMap connectionMap_;
};

}

#endif //EASYSERVER_TCPSERVER_H
