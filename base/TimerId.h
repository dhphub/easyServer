//
// Created by ∂≠∫£≈Ù on 2021/2/20.
//

#ifndef EASYSERVER_TIMERID_H
#define EASYSERVER_TIMERID_H

namespace es {

class Timer;

class TimerId {
public:
  explicit TimerId(Timer* timer) : value_(timer) {}
private:
  Timer* value_;
};

}

#endif //EASYSERVER_TIMERID_H
