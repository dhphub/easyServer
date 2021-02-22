//
// Created by onirii on 2021/2/21.
//

#ifndef EASYSERVER_EVENTLOOPTHREAD_H
#define EASYSERVER_EVENTLOOPTHREAD_H

#include "base/Mutex.h"
#include "base/Condition.h"
#include "base/thread.h"

namespace es {

class EventLoop;

class EventLoopThread : noncopyable {
public:
  EventLoopThread();
  ~EventLoopThread();
  //Ϊʲô����Ҫ����EventLoop��ָ��
  //EventLoopThread�ı������eventloop per thread
  //�߳�����֮�� loop��Ҫִ�еĺ����ǿյ� ��Ϊ��֪���߳�ӵ�е�loop
  //û�취��loop����ִ������
  //��˱��뷵��EventLoop��ָ��
  EventLoop* startLoop();
private:
  //threadFunc��ͷִ��loop_->loop();
  void threadFunc();
  EventLoop* loop_;
  Thread thread_;
  bool exiting_;
  MutexLock mutex_;
  Condition cond_;
};

}

#endif //EASYSERVER_EVENTLOOPTHREAD_H
