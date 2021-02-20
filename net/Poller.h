//
// Created by dhp on 2021/2/6.
//

#ifndef EASYSERVER_POLLER_H
#define EASYSERVER_POLLER_H

#include "base/noncopyable.h"
#include "net/EventLoop.h"
#include "base/Timestamp.h"
#include <map>

namespace es {

class Channel;
class EventLoop;

class Poller : noncopyable{
public:
  typedef std::vector<Channel*> ChannelList;
  void updateChannel(Channel* channel);
  explicit Poller(EventLoop* loop);
  Timestamp poll(int timeoutMs, ChannelList* activeChannels);
  ~Poller();
  void assertInLoopThread();
private:
  void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
  EventLoop* loop_;
  
  typedef std::vector<struct pollfd> PollfdList;
  typedef std::map<int, Channel*> ChannelMap;
  PollfdList pollfds_;
  ChannelMap channels_;
};
}

#endif //EASYSERVER_POLLER_H
