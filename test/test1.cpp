//
// Created by onirii on 2021/2/12.
//

#include "net/EventLoop.h"
#include "base/thread.h"
#include "base/CurrentThread.h"



es::EventLoop* gLoop;

//void threadFunc() {
//  printf("threadFunc : pid = %d, tid = %d", getpid(), es::CurrentThread::pid());
//  es::EventLoop eventLoop;
//  eventLoop.loop();
//}

void threadFunc() {
  printf("threadFunc : pid = %d, tid = %d", getpid(), es::CurrentThread::pid());
  gLoop->loop();
}

int main(int argc, char** argv) {
  
  es::EventLoop loop;
  gLoop = &loop;
  
  es::Thread thread(threadFunc);
  thread.start();
  
  loop.loop();
  
  pthread_exit(NULL);
  
}
