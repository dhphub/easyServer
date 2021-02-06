//
// Created by dhp on 2021/2/6.
//

#ifndef EASYSERVER_EVENTLOOP_H
#define EASYSERVER_EVENTLOOP_H

#include <pthread.h>

namespace es {
class EventLoop {
public:
  EventLoop();
  void loop();
  ~EventLoop();

private:
  pthread_t tid_;
};
}

#endif //EASYSERVER_EVENTLOOP_H
