//
// Created by onirii on 2021/2/23.
//

#include "net/Acceptor.h"
#include "net/EventLoop.h"
#include "net/InetAddress.h"
#include "net/SocketsOps.h"
#include <stdio.h>

void newConnection(int sockfd, const es::InetAddress& peerAddr) {
  printf("accept a new connection from %s\n", peerAddr.toHostPort().c_str());
  ::write(sockfd, "Hello, world!\n", 14);
  es::sockets::close(sockfd);
}

int main(int argc, char **argv) {
  printf("main(), pid=%d\n", getpid());
  es::InetAddress listenAddr(9991);
  es::EventLoop loop;
  es::Acceptor acceptor(&loop, listenAddr);
  acceptor.setNewConnectionCallback(newConnection);
  acceptor.listen();
  
  loop.loop();
}