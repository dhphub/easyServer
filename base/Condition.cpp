//
// Created by onirii on 2021/2/21.
//

#include "Condition.h"
#include <errno.h>
#include <stdlib.h>

bool es::Condition::waitSeconds(double seconds) {
  struct timespec time;
  clock_gettime(CLOCK_MONOTONIC, &time);
  
  
  const int64_t kNanoSecondsPerSecond = 1000000000;
  int64_t nanoseconds = static_cast<int64_t>(seconds * kNanoSecondsPerSecond);
  
  time.tv_sec += static_cast<time_t>((time.tv_nsec + nanoseconds) / kNanoSecondsPerSecond);
  time.tv_nsec = static_cast<long>((time.tv_nsec + nanoseconds) % kNanoSecondsPerSecond);
  
  MutexLock::UnassignGuard ug(mutex_);
  return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.getPthreadMutex(), &time);
}
