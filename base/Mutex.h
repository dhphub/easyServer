//
// Created by 董海鹏 on 2021/2/21.
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

private:
  pthread_mutex_t mutex_;
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
