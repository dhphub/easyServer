//
// Created by onirii on 2021/2/21.
//

#ifndef EASYSERVER_EVENTLOOPTHREAD_H
#define EASYSERVER_EVENTLOOPTHREAD_H

#include "base/Mutex.h"
#include "base/Condition.h"
#include "base/thread.h"

namespace es {

class EventLoop;

class EventLoopThread : noncopyable {
public:
  EventLoopThread();
  ~EventLoopThread();
  //为什么这里要返回EventLoop的指针
  //EventLoopThread的本意就是eventloop per thread
  //线程起了之后 loop里要执行的函数是空的 因为不知道线程拥有的loop
  //没办法给loop设置执行任务
  //因此必须返回EventLoop的指针
  EventLoop* startLoop();
private:
  //threadFunc里头执行loop_->loop();
  void threadFunc();
  EventLoop* loop_;
  Thread thread_;
  bool exiting_;
  MutexLock mutex_;
  Condition cond_;
};

}

#endif //EASYSERVER_EVENTLOOPTHREAD_H
