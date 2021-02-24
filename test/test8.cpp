//
// Created by onirii on 2021/2/24.
//

#include "net/TcpServer.h"
#include "net/EventLoop.h"
#include "net/TcpConnection.h"
#include "net/InetAddress.h"
#include "stdio.h"

void onConnection(const es::TcpConnectionPtr & conn) {
  if (conn->connected()) {
    printf("%s is connected\n", conn->peerAddress().toHostPort().c_str());
  } else {
    printf("%s is not connected\n", conn->peerAddress().toHostPort().c_str());
  }
}

void onMessage(const es::TcpConnectionPtr & conn, const char *data, ssize_t len) {
  printf("onMessage: receive a %zd bytes message: %s\n", len, data);
}

int main(int argc, char **argv) {
  es::EventLoop loop;
  es::InetAddress listenAddr(9981);
  es::TcpServer server(&loop, listenAddr);
  
  server.setConnectionCallback(onConnection);
  server.setMessageCallback(onMessage);
  
  server.start();
  loop.loop();
}