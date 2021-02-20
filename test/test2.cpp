//
// Created by ∂≠∫£≈Ù on 2021/2/19.
//

#include "net/Channel.h"
#include "net/EventLoop.h"

#include <stdio.h>
#include <sys/timerfd.h>
#include <strings.h>

es::EventLoop* gLoop;

void timeout() {
  printf("time out!\n");
  gLoop->quit();
}


int main(int argc, char **argv) {
  es::EventLoop loop;
  gLoop = &loop;
  
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
  
  es::Channel channel(&loop, timerfd);
  channel.setReadCallBack(timeout);
  channel.enableReading();
  
  struct itimerspec howlong;
  bzero(&howlong, sizeof howlong);
  howlong.it_value.tv_sec = 5;
  ::timerfd_settime(timerfd, 0, &howlong, NULL);
  
  loop.loop();
  
  ::close(timerfd);
}