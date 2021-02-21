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

// �������򵥷�װ ������˭����������Ϣ ��Ҫע��ά������Ϣ
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

  //�����������ʹ��
  //cond_wait��ʱ�������ͷ�
  //wait���ص�ʱ�����¼���
  //���������ά��pid_�����Ϣ����ȷ��
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
  //�����ߵ�global pid,����ʱҪ��û���̳߳�����
  pid_t pid_;
};

//RAII������װmutex,����ջ�ϱ���
//����ʱ��ȡ��Դ,����ʱ������Դ
//ע��RAII�Ļ�������, shared_ptr������RAII����չ
class MutexLockGuard : noncopyable {
public:
  explicit MutexLockGuard(MutexLock& lock);
  ~MutexLockGuard();
private:
  //ע������ֻ�������� MutexLock���ɿ���
  //TODO ���ø�Ϊֵ����
  MutexLock& lock_;
};

}

#endif //EASYSERVER_MUTEX_H
