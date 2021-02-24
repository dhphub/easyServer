//
// Created by onirii on 2021/2/24.
//

#ifndef EASYSERVER_TCPCONNECTION_H
#define EASYSERVER_TCPCONNECTION_H

#include "base/noncopyable.h"
#include "base/noncopyable.h"
#include "base/CallBack.h"
#include "net/InetAddress.h"
#include <memory>

namespace es {

class EventLoop;
class Channel;
class Socket;

class TcpConnection : noncopyable, public std::enable_shared_from_this<TcpConnection> {
public:
  TcpConnection(EventLoop* loop, const std::string& name, int sockfd,
                const InetAddress& localAddr, const InetAddress& peerAddr);
  
  ~TcpConnection();
  
  EventLoop* getLoop() const { return loop_; }
  const std::string& name() const { return name_; }
  const InetAddress& localAddress() const { return localAddr_; }
  const InetAddress& peerAddress() const { return peerAddr_; }
  bool connected() const { return state_ == kConnected; }
  
  void setConnectionCallback(const ConnectionCallbck& cb) { connectionCallbck_ = cb; }
  void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
  
  //why ??
  void connectEstablished();

private:
  enum StateE { kConnecting, kConnected };
  void setState(StateE state) { state_ = state; }
  
  void handleRead();
  
  EventLoop* loop_;
  std::string name_;
  StateE state_;
  std::shared_ptr<Socket> socket_;
  std::shared_ptr<Channel> channel_;
  InetAddress localAddr_;
  InetAddress peerAddr_;
  ConnectionCallbck connectionCallbck_;
  MessageCallback messageCallback_;
};

}

#endif //EASYSERVER_TCPCONNECTION_H
