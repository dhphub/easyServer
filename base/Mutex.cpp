//
// Created by ∂≠∫£≈Ù on 2021/2/21.
//

#include "Mutex.h"
#include "base/CurrentThread.h"

namespace es {

MutexLock::MutexLock() : pid_(0) {
  CHECKZERO(pthread_mutex_init(&mutex_, NULL))
}

MutexLock::~MutexLock() {
  assert(pid_ == 0);
  CHECKZERO(pthread_mutex_destroy(&mutex_))
}

void MutexLock::lock() {
  CHECKZERO(pthread_mutex_lock(&mutex_))
  pid_ = CurrentThread::pid();
}

void MutexLock::unlock() {
  pid_ = 0;
  CHECKZERO(pthread_mutex_unlock(&mutex_))
}

MutexLockGuard::~MutexLockGuard() {
  lock_.unlock();
}

MutexLockGuard::MutexLockGuard(MutexLock &lock) : lock_(lock) {
  lock_.lock();
}


}