//
// Created by ¶­º£Åô on 2021/2/21.
//

#include "net/EventLoop.h"

es::EventLoop* gLoop = nullptr;
int flag = 0;

void run4() {
  printf("run4(), pid=%d, flag=%d\n", getpid(), flag);
  gLoop->quit();
}

void run3() {
  printf("run3(), pid=%d, flag=%d\n", getpid(), flag);
  gLoop->runAfter(3, run4);
  flag = 3;
}

void run2() {
  printf("run2(), pid=%d, flag=%d\n", getpid(), flag);
  gLoop->queueInLoop(run3);
}

void run1() {
  flag = 1;
  printf("run1(), pid=%d, flag=%d\n", getpid(), flag);
  gLoop->runInLoop(run2);
  flag = 2;
}

int main(int argc, char **argv) {
  printf("main(), pid=%d, flag=%d\n", getpid(), flag);
  
  es::EventLoop loop;
  gLoop = &loop;
  
  loop.runAfter(2.0, run1);
  loop.loop();
  printf("main(), pid=%d, flag=%d\n", getpid(), flag);
  return 0;
}