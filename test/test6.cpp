//
// Created by onirii on 2021/2/22.
//

#include "net/EventLoopThread.h"
#include "net/EventLoop.h"

void run() {
  printf("run(): pid=%d, tid=%d\n", getpid(), es::CurrentThread::pid());
}

int main(int argc, char *argv[]) {
  printf("main(): pid=%d, tid=%d\n", getpid(), es::CurrentThread::pid());
  
  es::EventLoopThread loopThread;
  es::EventLoop* loop = loopThread.startLoop();
  loop->runInLoop(run);
  
  sleep(1);
  loop->runAfter(2, run);
  sleep(3);
  loop->quit();
  
  printf("exit main()\n");
  return 0;
}
