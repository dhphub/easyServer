//
// Created by onirii on 2021/2/21.
//

#include "EventLoopThread.h"
#include "net/EventLoop.h"

namespace es {

EventLoop* EventLoopThread::startLoop() {
  assert(!thread_.started());
  thread_.start();
  {
    MutexLockGuard lock(mutex_);
    while (!loop_) {
      cond_.wait();
    }
  }
  return loop_;
}

EventLoopThread::~EventLoopThread() {
  exiting_ = true;
  loop_->quit();
  thread_.join();
}

EventLoopThread::EventLoopThread() :
  loop_(nullptr),
  exiting_(false),
  thread_(std::bind(&EventLoopThread::threadFunc, this)),
  mutex_(),
  cond_(mutex_) {}

void EventLoopThread::threadFunc() {
  EventLoop loop;
  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notifyOne();
  }
  loop_->loop();
}

}