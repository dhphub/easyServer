//
// Created by ¶­º£Åô on 2021/2/20.
//

#ifndef EASYSERVER_TIMER_H
#define EASYSERVER_TIMER_H

#include "base/noncopyable.h"
#include "base/Timestamp.h"
#include "base/CallBack.h"

namespace es {

class Timer : noncopyable{
public:
  Timer(const TimerCallBack& cb, Timestamp when, double interval) :
    timerCallBack_(cb),
    expiration_(when),
    interval_(interval),
    repeat_(interval > 0.0) {}
    
  ~Timer() = default;
  
  void run() const {
    timerCallBack_();
  }
  
  void restart(Timestamp now);
  
  Timestamp expiration() const { return expiration_; }
  
  bool repeat() const { return repeat_; }
  
private:
  TimerCallBack timerCallBack_;
  Timestamp expiration_;
  const double interval_;
  const bool repeat_;
};

}

#endif //EASYSERVER_TIMER_H
