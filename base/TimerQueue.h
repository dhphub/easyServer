//
// Created by onirii on 2021/2/20.
//

#ifndef EASYSERVER_TIMERQUEUE_H
#define EASYSERVER_TIMERQUEUE_H

#include "net/Channel.h"
#include "base/Timestamp.h"
#include "base/CallBack.h"
#include "base/Timer.h"
#include <set>

namespace es {

class TimerId;
class EventLoop;

class TimerQueue : noncopyable{
public:
  explicit TimerQueue(EventLoop* loop);
  ~TimerQueue();
  TimerId addTimer(const TimerCallBack& cb, Timestamp when, double interval);
  void cancelTimer();
private:
  
  typedef std::pair<Timestamp, Timer*> Entry;
  typedef std::set<Entry> TimerList;
  
  //addTimer会调用这里 目的是为了加强线程安全性
  //这里调用EventLoop::runInLoop来保证只在loop线程调用
  void addTimerInLoop(Timer* timer);
  
  //timer到期,对应fd可读
  void handleRead();
  
  //获取所有到期的定时器
  std::vector<Entry> getExpired(Timestamp now);
  
  //插入定时器,返回值表示是否是第一个被执行的定时器时间
  bool insert(Timer* timer);
  
  //重置,当所有到期的定时器时间被执行完成后,一是需要删除一些不会再执行的定时器
  //二是需要把一些周期性的定时器重置一下,参数now的作用就是为了重置定时器
  void reset(const std::vector<Entry> expired, Timestamp now);
  EventLoop* loop_;
  const int timerfd_;
  Channel timerChannel_;
  TimerList timers_;
};

}

#endif //EASYSERVER_TIMERQUEUE_H
