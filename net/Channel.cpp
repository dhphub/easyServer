//
// Created by onirii on 2021/2/6.
//

#include "Channel.h"

namespace es {

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd) :
        loop_(loop),
        fd_(fd),
        events_(0),
        revents_(0),
        index_(-1),
        readCallBack_(nullptr),
        writeCallBack_(nullptr),
        errorCallBack_(nullptr) {}

Channel::~Channel() {}

void Channel::setReadCallBack(CallBackFunc readCallBack) {
  readCallBack_ = readCallBack;
}

void Channel::setWriteCallBack(CallBackFunc writeCallBack) {
  writeCallBack_ = writeCallBack;
}

void Channel::setErrorCallBack(CallBackFunc errorCallBack) {
  errorCallBack_ = errorCallBack;
}

void Channel::handleEvent() {
  if (revents_ & POLLNVAL) {
    //TODO log
  }
  
  if (revents_ & (POLLERR | POLLNVAL)) {
    if (errorCallBack_)
      errorCallBack_();
  }
  
  if (revents_ & (POLLIN | POLLPRI | POLLHUP)) {
    if (readCallBack_)
      readCallBack_();
  }
  
  if(revents_ & POLLOUT) {
    if (writeCallBack_)
      writeCallBack_();
  }
}

void Channel::update() {
  loop_->updateChannel(this);
}

}
