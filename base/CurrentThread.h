//
// Created by onirii on 2021/2/6.
//

#ifndef EASYSERVER_CURRENTTHREAD_H
#define EASYSERVER_CURRENTTHREAD_H

#include <pthread.h>
#include <unistd.h>

namespace es {
namespace CurrentThread {

extern __thread int cachePid;

void cache();

inline pid_t pid() {
  if (cachePid == 0) {
    cache();
  }
  return cachePid;
}

inline bool isMainThread() {
  return pid() == ::getpid();
}

}
}

#endif //EASYSERVER_CURRENTTHREAD_H
