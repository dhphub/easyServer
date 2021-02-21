//
// Created by onirii on 2021/2/6.
//

#include "thread.h"
#include "CurrentThread.h"

namespace es {

namespace detail {

pid_t gettid() {
  return static_cast<pid_t>(::syscall(SYS_gettid));
}

using ThreadFunc = es::Thread::ThreadFunc;
  struct threadParam {
    pid_t *pid_;
    std::string name_;
    ThreadFunc func_;
  
    threadParam(pid_t *pid, const std::string& name, ThreadFunc func) {
      pid_ = pid;
      name_ = name;
      func_ = func;
    }
  
    void runInThread() {
      *pid_ = es::CurrentThread::pid();
      pid_ = nullptr;
      try {
        func_();
      } catch (const std::exception& e) {
        // TODO: log
      }
    }
  };
  
  void *startThread(void *data) {
    threadParam *obj = static_cast<threadParam *>(data);
    obj->runInThread();
    delete obj;
    return nullptr;
  }
  
}

namespace CurrentThread {
  __thread int cachePid = 0;
  void cache() {
    if (cachePid == 0) {
      cachePid = detail::gettid();
    }
  }
}

Thread::Thread(const ThreadFunc func, const std::string& name) :
  name_(name),
  tid_(0),
  pid_(-1),
  func_(func),
  started_(false),
  joined_(false) {}
  
  pthread_t Thread::tid() const {
    return tid_;
  }
  
  pid_t Thread::pid() const {
    return pid_;
  }
  
  void Thread::start() {
    assert(!started_);
    started_ = true;
    detail::threadParam* data = new detail::threadParam(&pid_, name_, func_);
    if (pthread_create(&tid_, NULL, &detail::startThread, data)) {
      // TODO log
      started_ = false;
      delete data;
      data = NULL;
    }
  }
  
  void Thread::join() {
    assert(started_);
    assert(!joined_);
    joined_ = true;
    pthread_join(tid_, NULL);
  }

}

