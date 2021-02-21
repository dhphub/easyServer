//
// Created by onirii on 2021/2/21.
//

#ifndef EASYSERVER_CONDITION_H
#define EASYSERVER_CONDITION_H

#include "base/noncopyable.h"
#include "base/Mutex.h"

namespace es {

class Condition : noncopyable {
public:
  explicit Condition(MutexLock& mutex) : mutex_(mutex) {
    pthread_cond_init(&cond_, NULL);
  }
  ~Condition() {
    pthread_cond_destroy(&cond_);
  }
  
  void wait() {
    MutexLock::UnassignGuard ug(mutex_);
    pthread_cond_wait(&cond_, mutex_.getPthreadMutex());
  }
  
  void notifyOne() {
    pthread_cond_signal(&cond_);
  }
  
  void notifyAll() {
    pthread_cond_broadcast(&cond_);
  }
  
  //超时返回true 否则返回false
  bool waitSeconds(double seconds);
  
private:
  MutexLock& mutex_;
  pthread_cond_t cond_;
};

}

#endif //EASYSERVER_CONDITION_H
