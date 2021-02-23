//
// Created by onirii on 2021/2/23.
//

#include "net/Socket.h"
#include "net/SocketsOps.h"
#include <strings.h>

namespace es {

void Socket::setReuseAddr(bool on) {
  int optval = on ? 1 : 0;
  ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

int Socket::accept(InetAddress *peeraddr) {
  struct sockaddr_in addr;
  bzero(&addr, sizeof(addr));
  int connfd = sockets::accept(fd_, &addr);
  if (connfd >= 0) {
    peeraddr->setSockAddrInet(addr);
  }
  return connfd;
}

void Socket::listen() {
  sockets::listenOrDie(fd_);
}

void Socket::bindAddress(const InetAddress &localaddr) {
  sockets::bindOrDie(fd_, localaddr.getSockAddrInet());
}

Socket::~Socket() {
  sockets::close(fd_);
}

}