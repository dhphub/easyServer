//
// Created by dhp on 2021/2/6.
//

#ifndef EASYSERVER_CHANNEL_H
#define EASYSERVER_CHANNEL_H

#include "base/noncopyable.h"
#include "net/Poller.h"
#include <functional>
#include <poll.h>

namespace es {

class EventLoop;

class Channel : noncopyable {
public:
  using CallBackFunc = std::function<void()>;
  Channel(EventLoop* loop, int fd);
  ~Channel();
  void handleEvent();
  void setReadCallBack(CallBackFunc readCallBack);
  void setWriteCallBack(CallBackFunc writeCallBack);
  void setErrorCallBack(CallBackFunc errorCallBack);
  
  int fd() const { return fd_; }
  int events() const { return events_; }
  int revents() const { return revents_; }
  void set_revents(int revent) { revents_ = revent; }
  bool isNoneEvent() const { return events_ == kNoneEvent; }
  
  void enableReading() { events_ |= kReadEvent; update(); }
  
  int index() const { return index_; }
  void set_index(int index) { index_ = index; }

  EventLoop* ownerLoop() const { return loop_; }
  
private:
  void update();
  EventLoop* loop_;
  const int fd_;
  int events_;
  int revents_;
  int index_;  // use by Poller
  CallBackFunc readCallBack_;
  CallBackFunc writeCallBack_;
  CallBackFunc errorCallBack_;
  
  static const int kNoneEvent;
  static const int kReadEvent;
  static const int kWriteEvent;
  
};

}

#endif //EASYSERVER_CHANNEL_H
