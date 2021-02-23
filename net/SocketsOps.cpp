//
// Created by onirii on 2021/2/23.
//

#include "SocketsOps.h"
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

namespace es {

namespace {

typedef struct sockaddr SA;
const SA* sockaddr_cast(const struct sockaddr_in* addr) {
  return static_cast<const SA*>(reinterpret_cast<const void *>(addr));
}

SA* sockaddr_cast(struct sockaddr_in* addr) {
  return static_cast<SA*>(reinterpret_cast<void *>(addr));
}

}

namespace sockets {

int createNonblockingOrDie() {
  int sockfd = ::socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK|SOCK_CLOEXEC, IPPROTO_TCP);
  if (sockfd < 0) {
    //TODO log
    abort();
  }
  return sockfd;
}

void bindOrDie(int sockfd, const struct sockaddr_in& addr) {
  int ret = ::bind(sockfd, sockaddr_cast(&addr), sizeof(addr));
  if (ret < 0) {
    //TODO log
    abort();
  }
}

void listenOrDie(int sockfd) {
  int ret = ::listen(sockfd, SOMAXCONN);
  if (ret < 0) {
    //TODO log
    abort();
  }
}

int accept(int sockfd, struct sockaddr_in *addr) {
  socklen_t addrlen = sizeof(*addr);
  int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
  if (connfd < 0) {
    int saveErrno = errno;
    switch (saveErrno) {
      case EAGAIN:
      case ECONNABORTED:
      case EINTR:
      case EPROTO:
      case EPERM:
      case EMFILE:
        errno = saveErrno;
        break;
      case EBADF:
      case EFAULT:
      case EINVAL:
      case ENFILE:
      case ENOBUFS:
      case ENOMEM:
      case ENOTSOCK:
      case EOPNOTSUPP:
        // LOG_FATAL << "unexpected error of ::accept " << savedErrno;
        abort();
        break;
      default:
        abort();
        break;
    }
  }
  return connfd;
}

void close(int sockfd) {
  int ret = ::close(sockfd);
  if (ret < 0) {
    //TODO log
    abort();
  }
}

void toHostPort(char *buf, size_t size, const struct sockaddr_in& addr) {
  char host[INET_ADDRSTRLEN] = "INVALID";
  ::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof host);
  uint16_t port = sockets::networkToHost16(addr.sin_port);
  snprintf(buf, size, "%s:%u", host, port);
}

void fromHostPort(const char* ip, uint16_t port, struct sockaddr_in* addr) {
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16(port);
  int ret = ::inet_pton(AF_INET, ip, &addr->sin_addr);
  if (ret < 0) {
    //TODO log
    abort();
  }
}

}

}