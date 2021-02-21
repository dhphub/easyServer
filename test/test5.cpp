//
// Created by onirii on 2021/2/21.
//

#include "net/EventLoop.h"
#include "base/thread.h"

es::EventLoop* gLoop;

void run() {
  printf("run(), global pid=%d\n", es::CurrentThread::pid());
  gLoop->quit();
}

void threadFunc() {
  printf("threadFunc(), global pid=%d\n", es::CurrentThread::pid());
  gLoop->runInLoop(run);
}

int main() {
  printf("main(), global pid=%d\n", es::CurrentThread::pid());
  es::EventLoop loop;
  gLoop = &loop;
  es::Thread thread(threadFunc);
  thread.start();
  thread.join();
  sleep(1);
  printf("child thread done\n");
  loop.loop();
  
  printf("main loop exit\n");
  return 0;
}