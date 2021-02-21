//
// Created by onirii on 2021/2/6.
//

#include "Poller.h"

namespace es {

Poller::Poller(EventLoop *loop) : loop_(loop) {}

Poller::~Poller() {}

void Poller::updateChannel(Channel *channel) {
  assertInLoopThread();
  if (channel->index() < 0) {
    assert(channels_.find(channel->fd()) == channels_.end());
    struct pollfd pfd;
    pfd.fd = channel->fd();
    pfd.events = channel->events();
    pfd.revents = 0;
    pollfds_.push_back(pfd);
    int idx = static_cast<int>(pollfds_.size())-1;
    channel->set_index(idx);
    channels_[pfd.fd] = channel;
  } else {
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    int idx = channel->index();
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
    struct pollfd &pfd = pollfds_[idx];
    assert(pfd.fd == channel->fd() || pfd.fd == -1);
    pfd.events = static_cast<short int>(channel->events());
    pfd.revents = 0;
    if (channel->isNoneEvent()) {
      pfd.fd = -1;
    }
  }
}

Timestamp Poller::poll(int timeoutMs, ChannelList *activeChannels) {
  int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
  Timestamp now = Timestamp::now();
  if (numEvents > 0) {
    //TODO log
    fillActiveChannels(numEvents, activeChannels);
  } else if (numEvents == 0) {
    //TODO log
    printf("nothing happended\n");
  } else {
    //TODO err log
  }
  return now;
}

void Poller::fillActiveChannels(int numEvents, ChannelList *activeChannels) const {
  for (PollfdList::const_iterator pfd = pollfds_.begin();
      pfd != pollfds_.end() && numEvents > 0; ++pfd) {
    if (pfd->revents > 0) {
      --numEvents;
      ChannelMap::const_iterator ch = channels_.find(pfd->fd);
      assert(ch != channels_.end());
      Channel *channel = ch->second;
      assert(channel->fd() == pfd->fd);
      channel->set_revents(pfd->revents);
      activeChannels->push_back(channel);
    }
  }
}

void Poller::assertInLoopThread() {
  loop_->assertInLoopThread();
}

}