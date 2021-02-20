//
// Created by dhp on 2021/2/6.
//

#ifndef EASYSERVER_EVENTLOOP_H
#define EASYSERVER_EVENTLOOP_H

#include <pthread.h>
#include <vector>
#include <memory>
#include <assert.h>
#include "net/Channel.h"
#include "net/Poller.h"
#include "base/CurrentThread.h"
#include "base/Timestamp.h"
#include "base/CallBack.h"
#include "base/TimerId.h"

namespace es {

class Channel;
class Poller;
class TimerQueue;

class EventLoop {
public:
  typedef std::vector<Channel*> ChannelList;
  EventLoop();
  void loop();
  void quit();
  ~EventLoop();
  void assertInLoopThread();
  
  //定时任务
  TimerId runAt(const Timestamp& when, const TimerCallBack& cb);
  TimerId runAfter(double delay, const TimerCallBack& cb);
  TimerId runEvery(double interval, const TimerCallBack& cb);
  void cancel(TimerId timerId);
  
  // 内部使用
  void updateChannel(Channel* channel);

private:
  
  bool isInLoopThread();
  void abortNotInLoopThread();
  
  bool looping_;
  bool quit_;
  pid_t pid_;
  Timestamp pollReturnTime_;
  std::shared_ptr<Poller> poller_;
  std::shared_ptr<TimerQueue> timerQueue_;
  ChannelList activeChannels_;
};
}

#endif //EASYSERVER_EVENTLOOP_H
