//
// Created by dhp on 2021/2/6.
//
#include "net/EventLoop.h"
#include "base/TimerQueue.h"

namespace es {

__thread EventLoop *gLoopThreadPointer = nullptr;

const int kPollTimeMs = 10000;

EventLoop::EventLoop() :
  looping_(false),
  quit_(false),
  pid_(CurrentThread::pid()),
  poller_(new Poller(this)),
  timerQueue_(new TimerQueue(this)) {
  if (gLoopThreadPointer) {
    //TODO: 说明已经该线程已经创建过eventloop
    //需要打印Fatal log
  } else {
    gLoopThreadPointer = this;
  }
}

EventLoop::~EventLoop() {
  gLoopThreadPointer = nullptr;
  pid_ = -1;
}

void EventLoop::loop() {
  assertInLoopThread();
  assert(!looping_);
  //TODO: muduo中这个标志的作用是什么
  looping_ = true;
  quit_ = false;
  
  while (!quit_) {
    activeChannels_.clear();
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
    for (ChannelList::const_iterator ch = activeChannels_.begin();
        ch != activeChannels_.end(); ++ch) {
      (*ch)->handleEvent();
    }
  }
  looping_ = false;
}

bool EventLoop::isInLoopThread() {
  return pid_ == CurrentThread::pid();
}

void EventLoop::quit() {
  //assert(looping_);
  quit_ = true;
}

void EventLoop::assertInLoopThread() {
  if (!isInLoopThread()) {
    abortNotInLoopThread();
  }
}

void EventLoop::abortNotInLoopThread() {
  //TODO: 加入fatal log, fatal log退出或者抛异常
  printf("EventLoop::abortNotInLoopThread - EventLoop %d \
        was created in thread %d, current thread %d\n", this, pid_, CurrentThread::pid());
  exit(-1);
}

void EventLoop::updateChannel(Channel *channel) {
  assert(this == channel->ownerLoop());
  assertInLoopThread();
  poller_->updateChannel(channel);
}

TimerId EventLoop::runAt(const Timestamp &when, const TimerCallBack &cb) {
  return timerQueue_->addTimer(cb, when, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallBack &cb) {
  Timestamp time = addTime(Timestamp::now(), delay);
  return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runEvery(double interval, const TimerCallBack &cb) {
  Timestamp time = addTime(Timestamp::now(), interval);
  return timerQueue_->addTimer(cb, time, interval);
}

void EventLoop::cancel(TimerId timerId) {

}

}