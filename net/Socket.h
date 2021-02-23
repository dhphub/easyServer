//
// Created by onirii on 2021/2/23.
//

#ifndef EASYSERVER_SOCKET_H
#define EASYSERVER_SOCKET_H

#include "base/noncopyable.h"
#include "net/InetAddress.h"

namespace es {

class Socket {
public:
  explicit Socket(int fd) : fd_(fd) {}
  ~Socket();
  int fd() const { return fd_; }
  void bindAddress(const InetAddress& localaddr);
  void listen();
  int accept(InetAddress* peeraddr);
  void setReuseAddr(bool on);
private:
  const int fd_;
};

}

#endif //EASYSERVER_SOCKET_H
