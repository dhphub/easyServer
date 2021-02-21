//
// Created by onirii on 2021/2/20.
//

#ifndef EASYSERVER_TIMERQUEUE_H
#define EASYSERVER_TIMERQUEUE_H

#include "net/Channel.h"
#include "base/Timestamp.h"
#include "base/CallBack.h"
#include "base/Timer.h"
#include <set>

namespace es {

class TimerId;
class EventLoop;

class TimerQueue : noncopyable{
public:
  explicit TimerQueue(EventLoop* loop);
  ~TimerQueue();
  TimerId addTimer(const TimerCallBack& cb, Timestamp when, double interval);
  void cancelTimer();
private:
  
  typedef std::pair<Timestamp, Timer*> Entry;
  typedef std::set<Entry> TimerList;
  
  //addTimer��������� Ŀ����Ϊ�˼�ǿ�̰߳�ȫ��
  //�������EventLoop::runInLoop����ֻ֤��loop�̵߳���
  void addTimerInLoop(Timer* timer);
  
  //timer����,��Ӧfd�ɶ�
  void handleRead();
  
  //��ȡ���е��ڵĶ�ʱ��
  std::vector<Entry> getExpired(Timestamp now);
  
  //���붨ʱ��,����ֵ��ʾ�Ƿ��ǵ�һ����ִ�еĶ�ʱ��ʱ��
  bool insert(Timer* timer);
  
  //����,�����е��ڵĶ�ʱ��ʱ�䱻ִ����ɺ�,һ����Ҫɾ��һЩ������ִ�еĶ�ʱ��
  //������Ҫ��һЩ�����ԵĶ�ʱ������һ��,����now�����þ���Ϊ�����ö�ʱ��
  void reset(const std::vector<Entry> expired, Timestamp now);
  EventLoop* loop_;
  const int timerfd_;
  Channel timerChannel_;
  TimerList timers_;
};

}

#endif //EASYSERVER_TIMERQUEUE_H
