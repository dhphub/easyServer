//
// Created by onirii on 2021/2/21.
//

#ifndef EASYSERVER_MUTEX_H
#define EASYSERVER_MUTEX_H

#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include "base/noncopyable.h"

#define CHECKZERO(x)                      \
do {                                      \
  __typeof(x) ret = (x);                  \
  if (ret != 0) {                         \
    perror("ret is not zeor\n");          \
    printf("%s:%d", __FILE__, __LINE__);  \
    assert(ret == 0);                     \
  }                                       \
} while(0);

namespace es {

// 互斥锁简单封装 加入了谁持有锁的信息 需要注意维护此信息
class MutexLock : noncopyable {
public:
  MutexLock();
  
  ~MutexLock();
  
  void lock();
  
  void unlock();
  
  pid_t pid() const {
    return pid_;
  }
  
  pthread_mutex_t *getPthreadMutex() {
    return &mutex_;
  }

  //配合条件变量使用
  //cond_wait的时候锁会释放
  //wait返回的时候重新加锁
  //用这个类来维护pid_这个信息的正确性
  class UnassignGuard {
  public:
    explicit UnassignGuard(MutexLock& mutex) : mutex_(mutex) {
      mutex_.unassignOwner();
    }
    ~UnassignGuard() {
      mutex_.assignOwner();
    }
  private:
    MutexLock& mutex_;
  };
  
private:
  void assignOwner();
  void unassignOwner();
  pthread_mutex_t mutex_;
  //持有者的global pid,析构时要求没有线程持有锁
  pid_t pid_;
};

//RAII技法封装mutex,用于栈上变量
//构造时获取资源,析构时销毁资源
//注意RAII的基本语义, shared_ptr等算是RAII的扩展
class MutexLockGuard : noncopyable {
public:
  explicit MutexLockGuard(MutexLock& lock);
  ~MutexLockGuard();
private:
  //注意这里只能是引用 MutexLock不可拷贝
  //TODO 引用改为值试试
  MutexLock& lock_;
};

}

#endif //EASYSERVER_MUTEX_H
