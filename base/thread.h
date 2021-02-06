//
// Created by dhp on 2021/2/6.
//

#ifndef EASYSERVER_THREAD_H
#define EASYSERVER_THREAD_H

#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <functional>
#include <string>
#include <assert.h>
#include "noncopyable.h"

namespace es {

class Thread : public noncopyable {
public:
  using ThreadFunc = std::function<void()>;
  explicit Thread(const ThreadFunc func, const std::string& name=std::string());
  void start();
  void join();
  pthread_t tid() const;  // 进程内唯一的thread id
  pid_t pid() const;      // 系统内唯一的pid
private:
  std::string name_;
  pthread_t tid_;
  pid_t pid_;
  
  ThreadFunc func_;
  bool started_;
  bool joined_;
};

}
#endif //EASYSERVER_THREAD_H
