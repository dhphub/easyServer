//
// Created by onirii on 2021/2/6.
//
#include "net/EventLoop.h"
#include "base/TimerQueue.h"
#include <sys/eventfd.h>

static int createEventfd() {
  int evfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evfd < 0) {
    //TODO log
    abort();
  }
  return evfd;
}

namespace es {

__thread EventLoop *gLoopThreadPointer = nullptr;

const int kPollTimeMs = 10000;

EventLoop::EventLoop() :
  looping_(false),
  quit_(false),
  callingPendingFunctors_(false),
  pid_(CurrentThread::pid()),
  pollReturnTime_(Timestamp::invalid()),
  poller_(new Poller(this)),
  timerQueue_(new TimerQueue(this)),
  wakeupFd_(createEventfd()),
  wakeupChannel_(new Channel(this, wakeupFd_)) {
  if (gLoopThreadPointer) {
    //TODO: 说明已经该线程已经创建过eventloop
    //需要打印Fatal log
  } else {
    gLoopThreadPointer = this;
  }
  
  wakeupChannel_->setReadCallBack(std::bind(&EventLoop::hanleWakeupRead, this));
  wakeupChannel_->enableReading();
}

EventLoop::~EventLoop() {
  gLoopThreadPointer = nullptr;
  //析构时需要关闭eventfd
  ::close(wakeupFd_);
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
    //
    doPendingFunctors();
  }
  looping_ = false;
}

bool EventLoop::isInLoopThread() {
  return pid_ == CurrentThread::pid();
}

void EventLoop::quit() {
  //assert(looping_);
  quit_ = true;
  //不在loop线程退出loop,需要唤醒loop线程处理
  if (!isInLoopThread()) {
    wakeup();
  }
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
  abort();
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

void EventLoop::wakeup() {
  uint64_t one = 1;
  //唤醒只需要在wakeupFd_中随便写个uint64_t的值就行
  ssize_t n = ::write(wakeupFd_, &one, sizeof(one));
  if (n != sizeof(one)) {
    //TODO log
    abort();
  }
}

void EventLoop::hanleWakeupRead() {
  uint64_t val;
  ssize_t n = ::read(wakeupFd_, &val, sizeof(val));
  if (n != sizeof(val)) {
    // TODO log
    abort();
  }
}

void EventLoop::doPendingFunctors() {
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;
  //这里用swap的意义是减小临界区长度
  //避免阻塞其他线程调用queueInLoop
  {
    MutexLockGuard lock(mutex_);
    functors.swap(pendingFunctors_);
  }
  
  for (size_t i=0; i<functors.size(); ++i) {
    functors[i]();
  }
  
  callingPendingFunctors_ = false;
}

void EventLoop::queueInLoop(const Functor& cb) {
  {
    MutexLockGuard lock(mutex_);
    pendingFunctors_.push_back(cb);
  }
  if (!isInLoopThread() || callingPendingFunctors_) {
    wakeup();
  }
}

void EventLoop::runInLoop(const Functor& cb) {
  if (isInLoopThread()) {
    cb();
  } else {
    queueInLoop(cb);
  }
}

}