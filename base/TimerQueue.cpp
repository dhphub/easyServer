//
// Created by onirii on 2021/2/20.
//

#include "base/TimerQueue.h"
#include <sys/timerfd.h>
#include <strings.h>

namespace es {

namespace detail {

  int createTimerfd() {
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if (timerfd < 0) {
      //TODO log
    }
    return timerfd;
  }

  struct timespec howMuchTimeFromNow(Timestamp when) {
    int64_t microseconds = when.microSecondFromEpoch() - Timestamp::now().microSecondFromEpoch();
    if (microseconds < 100) {
      microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(microseconds % Timestamp::kMicroSecondsPerSecond * 1000);
    return ts;
  }
  
  void resetTimerfd(int timerfd, Timestamp expiration) {
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof(newValue));
    bzero(&oldValue, sizeof(oldValue));
    newValue.it_value = detail::howMuchTimeFromNow(expiration);
    //TODO 返回值判断和log
    timerfd_settime(timerfd, 0, &newValue, &oldValue);
  }
  
  void readTimerfd(int timerfd) {
    uint64_t howmany;
    ::read(timerfd, &howmany, sizeof(howmany));
  }

}

TimerQueue::TimerQueue(EventLoop *loop) :
  loop_(loop),
  timerfd_(detail::createTimerfd()),
  timerChannel_(loop, timerfd_),
  timers_() {
  
  timerChannel_.setReadCallBack(std::bind(&TimerQueue::handleRead, this));
  timerChannel_.enableReading();
}

TimerQueue::~TimerQueue() {
  ::close(timerfd_);
}

void TimerQueue::cancelTimer() {

}

void TimerQueue::addTimerInLoop(Timer *timer) {
  //TODO: why?
  loop_->assertInLoopThread();
  bool earliestChanged = insert(timer);
  //说明是所有定时器任务中最早会被触发执行的
  //这时肯定需要调用timerfd_settime来设置定时器
  //其实是这套api的用法的原因
  if (earliestChanged) {
    detail::resetTimerfd(timerfd_, timer->expiration());
  }
}

bool TimerQueue::insert(Timer *timer) {
  bool earliestChanged = false;
  Timestamp when = timer->expiration();
  TimerList::const_iterator it = timers_.begin();
  if (it == timers_.end() || when < it->first) {
    earliestChanged = true;
  }
  std::pair<TimerList::const_iterator, bool> result =
          timers_.insert(std::make_pair(when, timer));
  assert(result.second);
  return earliestChanged;
}

void TimerQueue::handleRead() {
  loop_->assertInLoopThread();
  detail::readTimerfd(timerfd_);
  
  Timestamp now = Timestamp::now();
  std::vector<Entry> expired = getExpired(now);
  
  //处理所有已经到期的定时器事件
  for (std::vector<Entry>::iterator it = expired.begin();
        it != expired.end(); ++it) {
    it->second->run();
  }
  
  //处理完后删除已经不需要再次执行的定时器事件
  reset(expired, now);
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now) {
  std::vector<Entry> expired;
  Entry flag = std::make_pair(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
  TimerList::iterator it = timers_.lower_bound(flag);
  assert(it == timers_.end() || it->first > now);
  //找到已经到期的定时器返回,并且从set中删除
  std::copy(timers_.begin(), it, std::back_inserter(expired));
  timers_.erase(timers_.begin(), it);
  return expired;
}

void TimerQueue::reset(const std::vector<Entry> expired, Timestamp now) {
  for (std::vector<Entry>::const_iterator it = expired.begin();
      it != expired.end(); ++it) {
    if (it->second->repeat()) {
      it->second->restart(now);
      insert(it->second);
    } else {
      delete it->second;
    }
  }
  
  //这里的模型是只用第一个到期的定时器来触发可读时间
  //然后再处理可读时间(第一个定时器到期)的时候,检查并处理所有已经到期的定时器事件
  //所以需要把第一个到期的定时器事件再次重置,下次会触发成可读事件
  Timestamp nextExpire;
  if (!timers_.empty()) {
    //TODO: 为什么不用timers_.begin()->first ??
    nextExpire = timers_.begin()->second->expiration();
  }
  if (nextExpire.valid()) {
    detail::resetTimerfd(timerfd_, nextExpire);
  }
}

TimerId TimerQueue::addTimer(const TimerCallBack& cb, Timestamp when, double interval) {
  Timer *timer = new Timer(cb, when, interval);
  loop_->runInLoop(std::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId(timer);
}

}